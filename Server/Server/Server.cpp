/*
 * Copyright 2016 Collapsed Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Main.h"
#include "Server.h"
#include "MySQL.h"

Server* server = nullptr;

Server::Server()
	: _queriesFlushInterval(0u), _lastQueriesFlushUpdateTime(0u)
{
	Jinra::Random::initSeed();
	Console::init();
	FileLogger::instance.open("Data/log.txt");

	// The highest packet ID can't be higher than maximum value of unsigned char.
	if (static_cast<s32>(Shared::PacketID::COUNT) > UCHAR_MAX)
		throw(Error("Server", "Server()", __LINE__, "Too much indexes of packets!"));
}

void Server::initialize() 
{
	CSimpleIni configFile;
	loadConfigFile(&configFile);

	MySQL::connectToDatabase(configFile.GetValue("MySQL", "Host"), 
							 configFile.GetValue("MySQL", "User"), 
							 configFile.GetValue("MySQL", "Password"), 
							 configFile.GetValue("MySQL", "Database"));
	
	// For debug purposes maxConnections is set to UCHAR_MAX. 
	// Higher maxConnections = longer initialization & release time.
#if defined(_DEBUG)
	u32 maxConnections = UCHAR_MAX;
#else // defined(_DEBUG)
	u32 maxConnections = USHRT_MAX;
	//u32 maxConnections = UINT_MAX;
#endif // defined(_DEBUG)

	// Initialize peer. 
	auto result = NetServer::initialize(
		static_cast<u16>(atoi(configFile.GetValue("Server", "Port"))), maxConnections, "static");
	if (result != Jinra::NetPeer::StartupResult::STARTED) {
		throw Error("Network", "initializeSocket()", __LINE__,
					"Cannot initialize socket! Startup Result: %d",
					static_cast<s8>(result));
	}

	// Gets queries flush interval from config file.
	_queriesFlushInterval = _lastQueriesFlushUpdateTime = atoi(configFile.GetValue(
		"Server", "QueriesFlush"));

	MySQL::loadClasses();
	// It's optional requirement. You can delete this if you don't want to have classes system.
	if (_classes.empty()) {
		throw(Error("Server", "initialize()", __LINE__,
			"Server requires at least one class to start."));
	}

	loadDialogues();
	MySQL::loadBannedIPs();
}

void Server::addClass(Class* class_)
{
	if (_classes.find(class_->getID()) == _classes.end())
		_classes[class_->getID()] = class_;
}

void Server::addClient(Client* client)
{
	if (_clients.find(client->getGUID()) == _clients.end())
		_clients[client->getGUID()] = client;
}


void Server::addCharacter(Character* character)
{
	if (_characters.find(character->getID()) == _characters.end())
		_characters[character->getID()] = character;
}

void Server::addGuild(Guild* guild)
{
	if (_guilds.find(guild->getID()) == _guilds.end())
		_guilds[guild->getID()] = guild;
}

void Server::removeClient(const NetGUID& guid)
{
	auto it = _clients.find(guid);
	if (it == _clients.end())
		return;

	delete it->second;
	_clients.erase(it);
}

void Server::removeCharacter(u32 id)
{
	auto it = _characters.find(id);
	if (it == _characters.end())
		return;

	delete it->second;
	_characters.erase(it);
}

void Server::removeGuild(u32 id)
{
	auto it = _guilds.find(id);
	if (it == _guilds.end())
		return;

	delete it->second;
	_guilds.erase(it);
}

void Server::mainLoop()
{
	while (!Console::isQuitRequested()) {
		auto currentTime = Jinra::getTimeMS();

		if (currentTime - _lastQueriesFlushUpdateTime > _queriesFlushInterval) {
			flushQueries();

			// Flushing all queries could take some time, so we need to update current time.
			currentTime = Jinra::getTimeMS();

			_lastQueriesFlushUpdateTime = currentTime;
		}

		receive();
		Console::inputUpdate();

		_loopCounter.update(currentTime);

#if (USE_MAX_CPU == 0)
		Jinra::sleep(0u);
#endif // (USE_MAX_CPU == 0)
	}
}

void Server::flushQueries()
{
	// MySQL can close connection if for long time there wasn't any queries.
	// This prevents it.
	MySQL::doQueryData(QueryData(QueryData::ID::KEEP_ALIVE, nullptr));

	// Flushes queries of characters.
	for (const auto& character : _characters)
		character.second->getQueriesQueue()->flush();
	
	// Flushes queries of guilds.
	for (const auto& guild : _guilds) {
		guild.second->getQueriesQueue()->flush();

		// Flushes queries of guild members.
		auto members = guild.second->getMembers();
		for (auto& member : *members)
			member.getQueriesQueue()->flush();
	}
}

void Server::releaseGuilds()
{
	u32 guildsReleased = 0u;
	for (auto it = _guilds.begin(); it != _guilds.end(); ++it) {
		delete it->second;
		++guildsReleased;
	}
	_guilds.clear();

	if (guildsReleased > 0u) {
		Console::writeLine(Warn("Server", "releaseGuilds()", __LINE__,
			"%u gilds has been released. Guilds should be released when there's no active "
			"members and invites, but not while server quit.", guildsReleased));
	}
}

void Server::releaseClients()
{
	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear();
}

void Server::releaseClasses()
{
	for (auto it = _classes.begin(); it != _classes.end(); ++it)
		delete it->second;
	_classes.clear();
}

void Server::releaseDialogues()
{
	for (auto it = _dialogues.begin(); it != _dialogues.end(); ++it)
		delete it->second;
	_dialogues.clear();
}

Client* Server::getClient(const String& email)
{
	for (const auto& it : _clients) {
		auto client = it.second;
		if (email == client->getEmail()) 
			return client;
	}
	return nullptr;
}


Character* Server::getCharacter(const String& name)
{
	for (const auto& it : _characters) {
		auto character = it.second;
		if (name == character->getName())
			return character;
	}
	return nullptr;
}

Guild* Server::getGuild(const String& name)
{
	for (const auto& it : _guilds) {
		auto guild = it.second;
		if (name == guild->getName())
			return guild;
	}
	return nullptr;
}

void Server::loadConfigFile(CSimpleIni* configFile)
{
	configFile->SetUnicode();
	if (configFile->LoadFile("Data/config.ini") != SI_OK) 
		throw Error("Server", "loadConfigFile()", __LINE__, "Cannot load config file!");
}

void Server::loadDialogues()
{
	// Dialog file name = id of dialog.
	// This method of loading dialogues is pretty good if there's no lacks like:
	// "0.lua", "1.lua", "3.lua"

	for (u32 id = 0u; true; ++id) {
		Dialogue* dialogue = nullptr;
		try {
			dialogue = new Dialogue(id);
		} catch (const Error& error) {
			delete dialogue;

			// Is this other exception than file not found was caught?
			// If yes, we need to throw it farther.
			if (strcmp(error.getFunction(), "runChunk()") == 0)
				throw error;

			// No more dialogues, so we can break a loop.
			break;
		}
		_dialogues[id] = dialogue;
	}
}


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

#pragma once

#include "Client.h"
#include "Dialogue.h"
#include "LoopCounter.h"

typedef std::map<u32, Class*> ClassesMap;
typedef std::map<NetGUID, Client*> ClientsMap;
typedef std::map<u32, Character*> CharactersMap;
typedef std::map<u32, Guild*> GuildsMap;
typedef std::map<u32, Dialogue*> DialoguesMap;

class Server : public Jinra::NetServer
{
public:
	Server();

	void initialize();

	void addClass(Class* class_);

	void addClient(Client* client);

	void addCharacter(Character* character);

	void addGuild(Guild* guild);

	void removeClient(const NetGUID& guid);

	void removeCharacter(u32 id);

	void removeGuild(u32 id);

	void mainLoop();

	void flushQueries();

	void releaseClients();

	void releaseGuilds();

	void releaseClasses();

	void releaseDialogues();

	void releaseSocket();

	ClassesMap* getClasses() {
		return &_classes;
	}

	u8 getClassesCount() const {
		return static_cast<u8>(_classes.size());
	}

	Class* getClass(u32 id) {
		return (_classes.find(id) != _classes.end()) ? _classes[id] : nullptr;
	}

	ClientsMap* getClients() {
		return &_clients;
	}

	Client* getClient(const NetGUID &guid) {
		return (_clients.find(guid) != _clients.end()) ? _clients[guid] : nullptr;
	}

	Client* getClient(const String& email);

	CharactersMap* getCharacters() {
		return &_characters;
	}

	Character* getCharacter(u32 id) {
		return (_characters.find(id) != _characters.end()) ? _characters[id] : nullptr;
	}

	Character* getCharacter(const String& name);

	GuildsMap* getGuilds() {
		return &_guilds;
	}

	Guild* getGuild(u32 id) {
		return (_guilds.find(id) != _guilds.end()) ? _guilds[id] : nullptr;
	}

	Guild* getGuild(const String& name);

	DialoguesMap* getDialogues() {
		return &_dialogues;
	}

	Dialogue* getDialogue(u32 id) {
		return (_dialogues.find(id) != _dialogues.end()) ? _dialogues[id] : nullptr;
	}

	LoopCounter* getLoopCounter() {
		return &_loopCounter;
	}

private:
	void loadConfigFile(CSimpleIni* configFile);

	void loadDialogues();

	void handlePacket(Packet* packet, BitStream* bitStream) override;
	
private:
	TimeMS _queriesFlushInterval;
	TimeMS _lastQueriesFlushUpdateTime;
	ClassesMap _classes;
	ClientsMap _clients;
	CharactersMap _characters;
	GuildsMap _guilds;
	DialoguesMap _dialogues;
	LoopCounter _loopCounter;
};
extern Server* server;

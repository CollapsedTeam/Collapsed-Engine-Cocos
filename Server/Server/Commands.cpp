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
#include "AccountModule.h"

void Console::executeCommand(const String& command, const std::vector<String>& args)
{
	if (command == "exit" || command == "quit") {
		Console::requestQuit();
	} else if (command == "stats") {
		executeStatsCommand();
	} else if (command == "lps") {
		executeLPSCommand();
	} else if (command == "online") {
		executeOnlineCommand();
	} else if (command == "who") {
		executeWhoCommand();
	} else if (command == "grant_super") {
		executeGrantSuperCommand(args);
	} else if (command == "kick") {
		executeKickCommand(args);
	} else if (command == "banip") {
		executeBanIPCommand(args);
	} else if (command == "unbanip") {
		executeUnbanIPCommand(args);
	} else if (command == "banacc") {
		executeBanAccountCommand(args);
	} else if (command == "unbanacc") {
		executeUnbanAccountCommand(args);
	} else {
		Console::writeLine(Info("Unknown command!"));
	}
}

void Console::executeStatsCommand()
{
	writeLine("==========");
	writeLine("Classes: %u", server->getClasses()->size());
	writeLine("Dialogues: %u", server->getDialogues()->size());
	writeLine("Number of connections: %u", server->getPeer()->numberOfConnections());
	writeLine("Players Online: %u", server->getClients()->size());
	writeLine("==========", true);

	std::vector<Jinra::SystemAddress> addresses;
	std::vector<NetGUID> guids;
	std::vector<Jinra::NetStatistics> statistics;
	server->getPeer()->getSystemList(addresses, guids);
	server->getPeer()->getStatisticsList(addresses, guids, statistics);

	u32 statisticsCount = statistics.size();
	if (statisticsCount < 1)
		return;

	writeLine("==========");
	writeLine("Client Peer:");
	Jinra::NetStatistics totalStatistics = statistics[0];

	for (u32 i = 1u; i < statisticsCount; i++)
		totalStatistics += statistics[i];

	// Buffer need to be huge to handle all statistics.
	char buffer[8192];
	Jinra::statisticsToString(&totalStatistics, buffer, 2);
	write(buffer);
	writeLine("==========");
}

void Console::executeLPSCommand() {
	server->getLoopCounter()->print();
}

void Console::executeOnlineCommand()
{
	writeLine(Info("Number of connections: %u\nPlayers Online: %u", 
		server->getPeer()->numberOfConnections(), server->getClients()->size()));
}

void Console::executeWhoCommand()
{
	writeLine(Info("Connected clients:"));
	auto clients = server->getClients();
	for (const auto& client : *clients)
		writeLine(client.second->getEmail(), true);
}


void Console::executeGrantSuperCommand(const std::vector<String>& args)
{
	if (args.size() != 1) {
		writeLine(Info("Command format: grant_super EMAIL"));
		return;
	}

	auto email = args[0];
	if (!AccountModule::isAccountExists(email)) {
		writeLine(Info("Account %s doesn't exists.", email.c_str()));
		return;
	}

	MySQL::grantSuper(email);

	// Client can be online.
	auto client = server->getClient(email);
	if (client != nullptr)
		client->setType(Account::Type::SUPER);

	Console::writeLine(Info("Account %s has been granted to SUPER.", email.c_str()));
}

void Console::executeKickCommand(const std::vector<String>& args)
{
	if (args.size() != 1) {
		writeLine(Info("Command format: kick CHARACTER_NAME"));
		return;
	}

	auto characterName = args[0];
	auto clients = server->getClients();
	for (const auto& it : *clients) {
		auto client = it.second;
		if (client->isPlaying() && characterName == client->getCharacter()->getName()) {
			server->getPeer()->closeConnection(client->getGUID(), true);
			writeLine(Info("%s has been kicked.", characterName.c_str()));
			return;
		}
	}

	writeLine(Info("%s is not playing.", characterName.c_str()));
}

void Console::executeBanIPCommand(const std::vector<String>& args)
{
	if (args.size() != 4) {
		writeLine(Info("Command format: banip IP REASON END_DATE END_TIME"));
		return;
	}

	auto ip = args[0];
	if (!Utils::isIPFormat(ip)) {
		writeLine(Info("%s is not valid ip.", ip.c_str()));
		return;
	}

	MySQL::banIP(ip, args[1], args[2], args[3]);
	server->getPeer()->AddToBanList(ip.c_str());

	Console::writeLine(Info("IP %s has been banned.", ip.c_str()));
}

void Console::executeUnbanIPCommand(const std::vector<String>& args)
{
	if (args.size() != 1) {
		writeLine(Info("Command format: unbanip IP"));
		return;
	}

	auto ip = args[0];
	if (!Utils::isIPFormat(ip)) {
		writeLine(Info("%s is not valid ip.", ip.c_str()));
		return;
	}

	MySQL::unbanIP(ip);
	server->getPeer()->RemoveFromBanList(ip.c_str());

	writeLine(Info("IP %s has been unbanned.", ip.c_str()));
}

void Console::executeBanAccountCommand(const std::vector<String>& args)
{
	if (args.size() != 1) {
		writeLine(Info("Command format: banacc EMAIL"));
		return;
	}

	auto email = args[0];
	if (!AccountModule::isAccountExists(email)) {
		writeLine(Info("Account %s doesn't exists.", email.c_str()));
		return;
	}

	MySQL::banAccount(email);

	auto client = server->getClient(email);
	if (client != nullptr)
		server->getPeer()->closeConnection(client->getGUID(), true);

	writeLine(Info("Account %s has been banned.", email.c_str()));
}


void Console::executeUnbanAccountCommand(const std::vector<String>& args)
{
	if (args.size() != 1) {
		writeLine(Info("Command format: unbanacc EMAIL"));
		return;
	}

	auto email = args[0];
	if (!AccountModule::isAccountExists(email)) {
		writeLine(Info("Account %s doesn't exists.", email.c_str()));
		return;
	}

	MySQL::unbanAccount(email);

	Console::writeLine(Info("Account %s has been unbanned.", email.c_str()));
}
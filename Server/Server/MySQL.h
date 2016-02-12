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
#include "QueryData.h"

class MySQL
{
public:
	static void connectToDatabase(cchar* host, cchar* user, cchar* password, 
								  cchar* database);

	static void execute(cchar* query);

	static void doQueryData(const QueryData& queryData);

	static void loadClasses();

	static void loadBannedIPs();

	static void loadAccountData(Account* account);

	static void loadAccountCharacter(Client* client);

	static void insertCharacter(Account* account, Character* character);
	
	static void grantSuper(const String& email);

	static void banIP(const String& ip, const String& reason, const String& endDate, 
					  const String& endTime);

	static void unbanIP(const String& ip);

	static void banAccount(const String& email);

	static void unbanAccount(const String& email);

	static void createGuild(Guild* guild);

	static void setDialogueFlag(Character* character, Dialogue* dialogue, u32 index, u8 value);

	static void insert(cchar* into, cchar* values);

	static void update(cchar* what, cchar* set, cchar* rest);

	static void delete_(cchar* from, cchar* rest);

	static void closeConnection();

	static bool isExists(cchar* from, cchar* rest);

	static u32 getLastGuestID();

	static u32 getGuildID(Character* character);

protected:
	static void keepAlive();

	static void loadCharacterGuild(Character* character);

	static void loadGuildData(Guild* guild);

	static void loadGuildMembers(Guild* guild);

	static void loadSavableDialogueFlags(Character* character);

protected:
	static soci::session _sql;
};

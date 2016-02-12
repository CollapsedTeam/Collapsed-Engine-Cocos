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
#include "MySQL.h"
#include "Server.h"
#include "sha1.h"
#include "CharacterModule.h"
#include "GuildModule.h"

soci::session MySQL::_sql;

void MySQL::connectToDatabase(cchar* host, cchar* user, cchar* password, cchar* database)
{
	char buffer[256];
	sprintf(buffer, "host=%s db=%s user=%s password='%s'", host, database, user, password);
	_sql.open(soci::mysql, buffer);
	_sql << "set names utf8";
}

void MySQL::execute(cchar* query) {
	_sql << query;
}

void MySQL::doQueryData(const QueryData& queryData)
{
	switch (queryData.getID()) {
		case QueryData::ID::KEEP_ALIVE:
			keepAlive();
			break;

		case QueryData::ID::CHARACTER_LEVEL_UP:
			CharacterModule::characterLevelUp(static_cast<Character*>(queryData.getObject()));
			break;

		case QueryData::ID::GUILD_SET_DESCRIPTION:
			GuildModule::setGuildDescription(static_cast<Guild*>(queryData.getObject()));
			break;

		case QueryData::ID::GUILD_SET_RANKS_NAMES:
			GuildModule::setGuildRanksNames(static_cast<Guild*>(queryData.getObject()));
			break;

		case QueryData::ID::GUILD_ADD_MEMBER: {
			GuildModule::insertGuildMember(static_cast<Guild*>(queryData.getObject()), 
										   static_cast<GuildMember*>(queryData.getObject2()));
		}
		break;

		case QueryData::ID::GUILD_SET_MEMBER_NOTE:
			GuildModule::setGuildMemberNote(static_cast<GuildMember*>(queryData.getObject()));
			break;

		case QueryData::ID::GUILD_SET_MEMBER_RANK:
			GuildModule::setGuildMemberRank(static_cast<GuildMember*>(queryData.getObject()));
			break;
	}
}

void MySQL::loadClasses()
{
	soci::rowset<soci::row> rows = (_sql.prepare << "SELECT * FROM classes");
	for (auto row = rows.begin(); row != rows.end(); ++row) {
		auto class_ = new Class(row->get<u32>(0), row->get<String>(1), row->get<s32>(2),
								row->get<s32>(3), row->get<s32>(4), row->get<s32>(5), 
								row->get<s32>(6), row->get<s32>(7), row->get<s32>(8), 
								row->get<s32>(9), row->get<s32>(10),
								row->get<s32>(10), row->get<s32>(11));
		server->addClass(class_);
	}
}

void MySQL::loadBannedIPs()
{
	auto timeNow = std::time(nullptr);

	soci::rowset<soci::row> rows = (_sql.prepare << "SELECT ip, end FROM bannedips");
	for (auto row = rows.begin(); row != rows.end(); ++row) {
		auto ip = row->get<String>(0);
		auto end = row->get<std::tm>(1);

		// If the difference is positive then it adds IP to the ban list. 
		// Otherwise it means end of ban time so we need to delete this IP from data base.
		if (std::difftime(mktime(&end), timeNow) > 0.0) {
			server->getPeer()->AddToBanList(ip.c_str());
		} else {
			unbanIP(ip);
		}
	}
}

void MySQL::loadAccountData(Account* account)
{
	char buffer[512];

	u32 accountID = 0u;
	s32 accountType = 0;
	sprintf(buffer, "SELECT a.id,a.type FROM accounts a WHERE a.email='%s'",
			account->getEmail());
	_sql << buffer, soci::into(accountID), soci::into(accountType);
	account->setID(accountID);
	account->setType(static_cast<Account::Type>(accountType));
}

void MySQL::loadAccountCharacter(Client* client)
{
	char buffer[512];
	sprintf(buffer, "SELECT c.* FROM characters c WHERE c.account=%u", client->getID());
	soci::rowset<soci::row> rows = (_sql.prepare << buffer);

	// Character data could be loaded without iteration, however this is a base for 
	// loading multiple characters of account.
	for (auto row = rows.begin(); row != rows.end(); ++row) {
		auto character = new Character(client, row->get<u32>(0), row->get<String>(2),
									   server->getClass(row->get<s32>(3)), row->get<s32>(4),
									   (row->get<s32>(5) == 1), row->get<u32>(6),
									   row->get<s32>(7), row->get<s32>(8), row->get<s32>(9), 
									   row->get<s32>(10), row->get<s32>(11));
		server->addCharacter(character);
		
		loadSavableDialogueFlags(character);
		loadCharacterGuild(character);
	}
}

void MySQL::insertCharacter(Account* account, Character* character)
{
	char buffer[2048];
	sprintf(buffer, "INSERT INTO characters(account,name,class,level,sex,cash,health,"
			"power,physique,dexterity,intelligence) VALUES(%u,'%s',%u,%u,%u,%u,%u,%u,%u,%u,"
			"%u)", account->getID(), character->getName(), character->getClass()->getID(), 
			character->getLevel(), character->isMale(), character->getCash(), 
			character->getHealth(), character->getPower(), character->getPhysique(), 
			character->getDexterity(), character->getIntelligence());
	_sql << buffer;

	long characterID = 0;
	if (!_sql.get_last_insert_id("characters", characterID)) {
		// Just in case if something fucked up.
		sprintf(buffer, "SELECT c.id FROM characters c WHERE c.name='%s'", 
				character->getName());
		_sql << buffer, soci::into(characterID);
	}
	character->setID(characterID);
}

void MySQL::grantSuper(const String& email)
{
	char buffer[256];
	sprintf(buffer, "UPDATE accounts a SET a.type=%u WHERE a.email='%s'", 
			Account::Type::SUPER, email.c_str());
	_sql << buffer;
}

void MySQL::banIP(const String& ip, const String& reason, const String& endDate,
				  const String& endTime)
{
	// Formats time to MySQL format.
	auto timeNow = std::time(nullptr);
	auto now = std::localtime(&timeNow);
	char timeNowBuffer[64];
	sprintf(timeNowBuffer, "%d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d", 
			(1900 + now->tm_year), now->tm_mon + 1, now->tm_mday, now->tm_hour, 
			now->tm_min, now->tm_sec);

	char buffer[512];
	sprintf(buffer, "INSERT INTO bannedips(ip,reason,start,end) VALUES('%s','%s','%s',"
			"'%s %s')", ip.c_str(), reason.c_str(), timeNowBuffer, endDate.c_str(), 
			endTime.c_str());
	_sql << buffer;
}

void MySQL::unbanIP(const String& ip)
{
	char buffer[128];
	sprintf(buffer, "DELETE FROM bannedips b WHERE b.ip='%s'", ip.c_str());
	_sql << buffer;
}

void MySQL::banAccount(const String& email)
{
	char buffer[256];
	sprintf(buffer, "UPDATE accounts a SET a.type=%u WHERE a.email='%s'", Account::Type::BANNED,
			email.c_str());
	_sql << buffer;
}

void MySQL::unbanAccount(const String& email)
{
	char buffer[256];
	sprintf(buffer, "UPDATE accounts a SET a.type=%u WHERE a.email='%s'", Account::Type::NORMAL,
			email.c_str());
	_sql << buffer;
}

void MySQL::createGuild(Guild* guild)
{
	char buffer[512];
	sprintf(buffer, "INSERT INTO guilds(name,tag) VALUES('%s','%s')", guild->getName(), 
			guild->getTag());
	_sql << buffer;

	long guildID = 0;
	if (!_sql.get_last_insert_id("guilds", guildID)) {
		// Just in case if something fucked up.
		sprintf(buffer, "SELECT g.id FROM guilds g WHERE g.name='%s'", 
				guild->getName());
		_sql << buffer, soci::into(guildID);
	}
	guild->setID(guildID);
}

void MySQL::setDialogueFlag(Character* character, Dialogue* dialogue, u32 index, u8 value)
{
	// I wish it could be only one query.

	char buffer[128];
	sprintf(buffer, "SELECT d.id FROM dialogues d WHERE d.dial_id=%u AND d.char_id=%u "
			"LIMIT %u, 1", dialogue->getID(), character->getID(),
			index);
	long id = 0;
	_sql << buffer, soci::into(id);

	sprintf(buffer, "UPDATE dialogues d SET d.value=%u WHERE d.id=%u", value,
			id);
	_sql << buffer;
}

void MySQL::insert(cchar* into, cchar* values)
{
	char buffer[512];
	sprintf(buffer, "INSERT INTO %s VALUES(%s)", into, values);
	_sql << buffer;
}

void MySQL::update(cchar* what, cchar* set, cchar* rest)
{
	char buffer[512];
	sprintf(buffer, "UPDATE %s SET %s WHERE %s", what, set, rest);
	_sql << buffer;
}

void MySQL::delete_(cchar* from, cchar* rest)
{
	char buffer[512];
	sprintf(buffer, "DELETE FROM %s WHERE %s", from, rest);
	_sql << buffer;
}

void MySQL::closeConnection() {
	_sql.close();
}

bool MySQL::isExists(cchar* from, cchar* rest)
{
	char buffer[512];
	sprintf(buffer, "SELECT COUNT(*) FROM %s WHERE %s", from, rest);

	s32 result = -1;
	_sql << buffer, soci::into(result);
	return (result > 0);
}

u32 MySQL::getLastGuestID()
{
	String lastGuestEmail;

	char buffer[512];
	sprintf(buffer, "SELECT a.email FROM accounts a WHERE a.email LIKE '%s%%' ORDER BY a.id "
			"DESC LIMIT 1", GUEST_PREFIX);

	_sql << buffer, soci::into(lastGuestEmail);

	u32 id = 0u;
	sprintf(buffer, GUEST_PREFIX "%%u");
	sscanf(lastGuestEmail.c_str(), buffer, &id);
	return id;
}

u32 MySQL::getGuildID(Character* character)
{
	char buffer[256];
	sprintf(buffer, "SELECT m.guild_id FROM guildmembers m WHERE m.char_id=%u", 
			character->getID());

	u32 guildID = 0u;
	_sql << buffer, soci::into(guildID);
	return guildID;
}

void MySQL::keepAlive() 
{
	_sql << "SELECT 0";
	Console::writeLine(Debug("Keep alive query has been executed."));
}

void MySQL::loadCharacterGuild(Character* character)
{
	u32 guildID = MySQL::getGuildID(character);

	// Checks if character has a guild.
	if (guildID > 0u) {
		auto guild = server->getGuild(guildID);

		// Checks if guild is not already in memory.
		if (guild == nullptr) {
			// Is not, so we create new object and load all required data.
			guild = new Guild(guildID);
			loadGuildData(guild);
			loadGuildMembers(guild);
			server->addGuild(guild);
			character->setGuild(guild);
		} else {
			character->setGuild(guild);
			guild->getMember(character->getName())->setOnline(character);
		}
	}
}

void MySQL::loadGuildData(Guild* guild)
{
	char buffer[512];

	String name, tag, description;
	String ranks[Shared::GUILD_RANKS_COUNT];
	sprintf(buffer, "SELECT g.name,g.tag,g.description,g.rank0,g.rank1,g.rank2,g.rank3 "
			"FROM guilds g WHERE g.id=%u", guild->getID());
	_sql << buffer, soci::into(name), soci::into(tag), soci::into(description), 
		soci::into(ranks[0]), soci::into(ranks[1]), soci::into(ranks[2]), soci::into(ranks[3]);
	
	guild->setName(name);
	guild->setTag(tag);
	guild->setDescription(description);
	
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i)
		guild->setRankName(i, ranks[i]);
}

void MySQL::loadGuildMembers(Guild* guild)
{
	char buffer[512];
	sprintf(buffer, "SELECT m.char_id,m.rank,m.note FROM guildmembers m WHERE m.guild_id=%u", 
			guild->getID());
	soci::rowset<soci::row> rows = (_sql.prepare << buffer);

	for (auto row = rows.begin(); row != rows.end(); ++row) {
		u32 characterID = row->get<u32>(0);
		auto rank = static_cast<Shared::GuildRanks>(row->get<s32>(1));
		auto note = row->get<String>(2);
		auto character = server->getCharacter(characterID);

		// Checks if character is not already in memory / offline.
		if (character == nullptr) {
			// Offline
			String characterName;
			u16 characterLevel = 0;
			sprintf(buffer, "SELECT c.name,c.level FROM characters c WHERE c.id=%u", 
					characterID);
			_sql << buffer, soci::into(characterName), soci::into(characterLevel);
			guild->addMember(characterName, characterLevel, rank, note);
		} else {
			// Online
			guild->addMember(character, rank, note);
		}
	}
}

void MySQL::loadSavableDialogueFlags(Character* character)
{
	char buffer[256];
	sprintf(buffer, "SELECT d.dial_id,d.value FROM dialogues d WHERE d.char_id=%u", 
			character->getID());

	soci::rowset<soci::row> rows = (_sql.prepare << buffer);
	for (auto row = rows.begin(); row != rows.end(); ++row)
		character->addSavableFlag(server->getDialogue(row->get<u32>(0)), row->get<s32>(1));
}
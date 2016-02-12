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
#include "CharacterModule.h"
#include "MySQL.h"
#include "Server.h"

void CharacterModule::createCharacter(Packet* packet, BitStream* bitStream)
{
	READ(bitStream, String, name);
	auto msg = Shared::Global::isValidName(name);
	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	} else if (isCharacterExists(name)) {
		sendMessagePacket(packet->guid, Shared::Message::NAME_EXISTS);
		return;
	}

	READ(bitStream, u32, classID);
	auto class_ = server->getClass(classID);
	if (class_ == nullptr) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto client = server->getClient(packet->guid);
	if (client == nullptr) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, bool, male);
	auto character = new Character(client, name, class_, male);
	MySQL::insertCharacter(client, character);
	server->addCharacter(character);

	client->sendLoginPacket();
}

void CharacterModule::characterLevelUp(Character* character)
{
	char set[256];
	sprintf(set, "c.level=%u,c.health=%u,c.power=%u,c.physique=%u,c.dexterity=%u,"
			"c.intelligence=%u", character->getLevel(), character->getHealth(), 
			character->getPower(), character->getPhysique(), character->getDexterity(), 
			character->getIntelligence());

	char rest[128];
	sprintf(rest, "c.id=%u", character->getID());

	MySQL::update("characters c", set, rest);
}

void CharacterModule::deleteCharacter(Character* character)
{
	char rest[128];
	sprintf(rest, "c.name='%s'", character->getName());
	MySQL::delete_("characters c", rest);
}

bool CharacterModule::isCharacterExists(const String& name) 
{
	char rest[128];
	sprintf(rest, "c.name='%s'", name.c_str());
	return MySQL::isExists("characters c", rest);
}
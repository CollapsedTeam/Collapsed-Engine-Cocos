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
#include "Character.h"
#include "Server.h"
#include "MySQL.h"
#include "DialogueModule.h"

OOLUA_EXPORT_FUNCTIONS_CONST(SharedCharacter, isMale, isFemale, getID, getName, getLevel,
							 getCash, getHealth, getPower, getPhysique, getDexterity, 
							 getIntelligence);
OOLUA_EXPORT_FUNCTIONS(SharedCharacter);

OOLUA_EXPORT_FUNCTIONS_CONST(Character, isBelongsToGuild, getClass);
OOLUA_EXPORT_FUNCTIONS(Character, levelUp, addFlags, addSavableFlags, getFlag, getSavableFlag,
					   setFlag, setSavableFlag);

Character::Character(Client* client, const String& name, Class* class_, bool male)
	: SharedCharacter(0, name, 1, male, class_->getStartCash(), class_->getHealth(), 
	class_->getPower(), class_->getPhysique(), class_->getDexterity(), 
	class_->getIntelligence()), _client(client), _class(class_), _guild(nullptr), 
	_dialogue(nullptr)
{
	client->setCharacter(this);
}

Character::Character(Client* client, u32 id, const String& name, Class* class_, u16 level, 
					 bool male, u32 cash, u16 health, u16 power, u16 physique, u16 dexterity, 
					 u16 intelligence)
	: SharedCharacter(id, name, level, male, cash, health, power, physique, dexterity, 
	intelligence), _client(client), _class(class_), _guild(nullptr), _dialogue(nullptr)
{
	client->setCharacter(this);
}

Character::~Character()
{
	_queriesQueue.flush();
	if (_guild == nullptr) {
		// When we got invitation to join guild and we left the game before we decide to accept
		// or not, the guild invitation is still in guild object. Guild object has no 
		// reference to non-members characters. So basically we need iterate through all 
		// guilds to remove invitation if exist.
		auto guilds = server->getGuilds();
		for (const auto &guild : *guilds)
			guild.second->removeInvite(_id);
	} else {
		_guild->setMemberToOffline(this);
	}
}

void Character::levelUp()
{
	++_level;
	_health += _class->getHealthGain();
	_power += _class->getPowerGain();
	_physique += _class->getPhysiqueGain();
	_dexterity += _class->getDexterityGain();
	_intelligence += _class->getIntelligenceGain();

	sendLevelUpPacket();
	_queriesQueue.addQueryData(QueryData(QueryData::ID::CHARACTER_LEVEL_UP, this));
}

void Character::addFlags(u32 numFlagsToAdd)
{
	for (u32 i = 0u; i < numFlagsToAdd; ++i)
		_dialoguesFlags.push_back(0);
}

void Character::clearFlags() {
	_dialoguesFlags.clear();
}

void Character::addSavableFlag(Dialogue* dialogue, u8 value)
{
	u32 dialogueID = dialogue->getID();

	// We don't want to override flag.
	if (_savableDialoguesFlags.find(dialogueID) != _savableDialoguesFlags.end())
		return;

	_savableDialoguesFlags[dialogueID].push_back(value);
}

void Character::addSavableFlags(Dialogue* dialogue, u32 numFlagsToAdd)
{
	u32 dialogueID = dialogue->getID();

	// We don't want to override flags.
	if (_savableDialoguesFlags.find(dialogueID) != _savableDialoguesFlags.end())
		return;

	auto& flags = _savableDialoguesFlags[dialogueID];
	for (u32 i = 0u; i < numFlagsToAdd; ++i) {
		flags.push_back(0);
		DialogueModule::insertDialogueFlag(this, dialogue, 0);
	}
}

void Character::setSavableFlag(Dialogue* dialogue, u32 index, u8 value)
{
	_savableDialoguesFlags[dialogue->getID()][index] = value;
	MySQL::setDialogueFlag(this, dialogue, index, value);
}

void Character::sendLevelUpPacket()
{
	BITSTREAM(bitStream, Shared::PacketID::CHARACTER_LEVEL_UP);
	bitStream.write(_level);
	bitStream.write(_health);
	bitStream.write(_power);
	bitStream.write(_physique);
	bitStream.write(_dexterity);
	bitStream.write(_intelligence);
	server->send(&bitStream, Packet::MEDIUM_PRIORITY, getClient()->getGUID());
}
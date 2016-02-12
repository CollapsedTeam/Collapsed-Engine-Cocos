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

#include "Class.h"
#include "Guild.h"
#include "Dialogue.h"

class Client;

class Character : public SharedCharacter
{
public:
	/**
	 * 
	 * This version of constructor is for completely new character.
	 * @param client
	 * @param name 
	 * @param class_ 
	 * @param male 
	 */
	Character(Client* client, const String& name, Class* class_, bool male);

	/**
	 * 
	 * This version of constructor is for character loaded from database.
	 * @param client
	 * @param id
	 * @param name
	 * @param class_ 
	 * @param level 
	 * @param male 
	 * @param cash
	 * @param health 
	 * @param power
	 * @param physique
	 * @param dexterity 
	 * @param intelligence 
	 */
	Character(Client* client, u32 id, const String& name, Class* class_, u16 level, bool male, 
			  u32 cash, u16 health, u16 power, u16 physique, u16 dexterity, u16 intelligence);
	
	/**
	 * 
	 */
	virtual ~Character();

	void levelUp();

	void addFlags(u32 numFlagsToAdd);

	void clearFlags();

	void addSavableFlag(Dialogue* dialogue, u8 value);

	void addSavableFlags(Dialogue* dialogue, u32 numFlagsToAdd);

	/**
	 * 
	 * @return
	 */
	inline bool isBelongsToGuild() const {
		return (_guild != nullptr);
	}

	/**
	 *
	 * @return
	 */
	inline Client* getClient() const {
		return _client;
	}

	/**
	 * 
	 * @return
	 */
	inline Class* getClass() const {
		return _class;
	}

	/**
	 * 
	 * @return
	 */
	inline Guild* getGuild() const {
		return _guild;
	}

	inline Dialogue* getDialogue() const {
		return _dialogue;
	}

	inline u8 getFlag(u32 index) {
		return _dialoguesFlags[index];
	}

	inline u8 getSavableFlag(Dialogue* dialogue, u32 index) {
		return _savableDialoguesFlags[dialogue->getID()][index];
	}

	inline QueriesQueue* getQueriesQueue() {
		return &_queriesQueue;
	}

	/**
	 * 
	 * @param guild
	 */
	inline void setGuild(Guild* guild) {
		_guild = guild;
	}
	
	inline void setDialogue(Dialogue* dialogue) {
		_dialogue = dialogue;
	}

	inline void setFlag(u32 index, u8 value) {
		_dialoguesFlags[index] = value;
	}

	void setSavableFlag(Dialogue* dialogue, u32 index, u8 value);

public:
	void sendLevelUpPacket();

protected:
	Client* _client;
	Class* _class;
	Guild* _guild;
	Dialogue* _dialogue;
	std::vector<u8> _dialoguesFlags;
	std::map<u32, std::vector<u8>> _savableDialoguesFlags;
	QueriesQueue _queriesQueue;
}; 

OOLUA_PROXY(SharedCharacter)
OOLUA_TAGS(OOLUA::No_default_constructor)
OOLUA_MFUNC_CONST(isMale)
OOLUA_MFUNC_CONST(isFemale)
OOLUA_MFUNC_CONST(getID)
OOLUA_MFUNC_CONST(getName)
OOLUA_MFUNC_CONST(getLevel)
OOLUA_MFUNC_CONST(getCash)
OOLUA_MFUNC_CONST(getHealth)
OOLUA_MFUNC_CONST(getPower)
OOLUA_MFUNC_CONST(getPhysique)
OOLUA_MFUNC_CONST(getDexterity)
OOLUA_MFUNC_CONST(getIntelligence)
OOLUA_PROXY_END

OOLUA_PROXY(Character, SharedCharacter)
OOLUA_TAGS(OOLUA::No_default_constructor)
OOLUA_MFUNC_CONST(isBelongsToGuild)
OOLUA_MFUNC_CONST(getClass)
OOLUA_MFUNC(levelUp)
OOLUA_MFUNC(addFlags)
OOLUA_MFUNC(addSavableFlags)
OOLUA_MFUNC(getFlag)
OOLUA_MFUNC(getSavableFlag)
OOLUA_MFUNC(setFlag)
OOLUA_MFUNC(setSavableFlag)
OOLUA_PROXY_END
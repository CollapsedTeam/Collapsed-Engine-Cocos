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

#include "QueriesQueue.h"

class Character;

class GuildMember
{
public:
	struct OfflineCharacter
	{
		String name;
		u16 level;
	};

public:
	GuildMember(Character* character, Shared::GuildRanks rank, const String& note);

	GuildMember(const String& characterName, u16 characterLevel, Shared::GuildRanks rank, 
				const String& note);

	~GuildMember();

	void promote();

	void degrade();

	bool isOnline() const {
		return (_onlineCharacter != nullptr);
	}

	bool isOwner() const {
		return (_rank == Shared::GuildRanks::OWNER);
	}

	bool isManager() const {
		return (_rank == Shared::GuildRanks::MANAGER);
	}

	Character* getOnlineCharacter() const {
		return _onlineCharacter;
	}

	cchar* getName() const;

	u16 getLevel() const;

	Shared::GuildRanks getRank() const {
		return _rank;
	}

	cchar* getNote() const {
		return _note.c_str();
	}

	QueriesQueue* getQueriesQueue() {
		return &_queriesQueue;
	}

	void setOnline(Character* onlineCharacter);

	void setRank(Shared::GuildRanks rank);

	void setNote(const String& note);

private:
	/**
	 * 
	 */
	void sendGuildMemberBecomeOnlinePacket();
	
	/**
	 * 
	 */
	void sendGuildMemberBecomeOfflinePacket();

protected:
	Character* _onlineCharacter;
	OfflineCharacter _offlineCharacter;
	Shared::GuildRanks _rank;
	String _note;
	QueriesQueue _queriesQueue;
};

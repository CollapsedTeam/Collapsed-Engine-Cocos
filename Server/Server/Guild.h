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

#include "GuildMember.h"
#include "QueriesQueue.h"

class Guild : public Shared::Guild
{
public:
	typedef std::map<u32 /* ID of invite = recipient's ID */, u32 /* sender's ID */> Invites;

public:
	Guild(u32 id);

	Guild(const String& name, const String& tag, Character* creator);

	~Guild();

	void addNewMember(Character* character, Shared::GuildRanks rank);

	void addMember(Character* character, Shared::GuildRanks rank, const String& note);

	void addMember(const String& characterName, u16 characterLevel, 
				   Shared::GuildRanks rank, const String& note);
	
	bool addInvite(u32 senderID, u32 recipientID);

	void removeMember(const String& memberName);

	void removeInvite(u32 recipientID);

	inline u32 getID() const {
		return _id;
	}

	inline cchar* getName() const {
		return _name.c_str();
	}

	inline std::vector<GuildMember>* getMembers() {
		return &_members;
	}

	inline u16 getMembersCount() {
		return static_cast<u16>(_members.size());
	}

	u32 getActiveMembersCount();

	GuildMember* getMember(u32 index) {
		return &_members[index];
	}

	GuildMember* getMember(const String& memberName);

	inline u32 getInvitesCount() const {
		return _invites.size();
	}

	Character* getSenderOfInvite(u32 inviteID);

	inline QueriesQueue* getQueriesQueue() {
		return &_queriesQueue;
	}

	inline void setID(u32 id) {
		_id = id;
	}

	inline void setName(const String& name) {
		_name = name;
	}

	void setDescription(const SString& description) override;

	void setRankName(u8 index, const String& rankName) override;

	void setMemberToOffline(const Character* memberCharacter);
	
public:
	/**
	 *
	 * @param bitStream
	 */
	void sendPacketToAllOnlineMembersInGuild(const BitStream* bitStream);

	/**
	 * 
	 * @param bitStream
	 * @param guid
	 */
	void sendPacketToAllOnlineMembersInGuildExcept(const BitStream* bitStream, 
												   const NetGUID& exceptGUID);

	/**
	 *
	 * @param character 
	 */
	void sendInviteCharacterPacket(Character* character);

	/**
	 * 
	 * @param guid 
	 */
	void sendJoinPacket(const NetGUID& guid);
	
	/**
	 * 
	 */
	void sendSetDescriptionPacket();
	
	/**
	 * 
	 */
	void sendSetRankNamePacket(u8 rankIndex, const String& rankName);

	/**
	 *
	 * @param member 
	 */
	void sendAddMemberPacket(GuildMember* member);

	/**
	 * 
	 * @param memberName
	 * @param memberNote
	 */
	void sendSetNotePacket(cchar* memberName, const String& memberNote);
	
	/**
	 * 
	 * @param member
	 */
	void sendSetMemberRankPacket(GuildMember* member);
	
	/**
	 * 
	 * @param memberName 
	 * @param disbandGuild
	 */
	void sendLeavePacket(cchar* memberName, bool disbandGuild);

protected:
	u32 _id;
	String _name;
	std::vector<GuildMember> _members;
	Invites _invites;
	QueriesQueue _queriesQueue;
};

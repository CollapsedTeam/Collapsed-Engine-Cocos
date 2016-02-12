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
#include "Guild.h"
#include "Server.h"

Guild::Guild(u32 id)
	: _id(id)
{ }

Guild::Guild(const String& name, const String& tag, Character* creator)
	: Shared::Guild(tag), _id(0), _name(name)
{ 
	_ranksNames[0] = "Recruit";
	_ranksNames[1] = "Member";
	_ranksNames[2] = "Moderator";
	_ranksNames[3] = "Owner";
	addNewMember(creator, Shared::GuildRanks::OWNER);
}

Guild::~Guild()
{
	_queriesQueue.flush();

	// When owner disband guild, we need to set pointer to guild to null for every online
	// member.
	for (auto& member : _members) {
		if (member.isOnline())
			member.getOnlineCharacter()->setGuild(nullptr);
	}
}

void Guild::addNewMember(Character* character, Shared::GuildRanks rank)
{
	addMember(character, rank, "");
	_queriesQueue.addQueryData(QueryData(QueryData::ID::GUILD_ADD_MEMBER, this, 
		&_members[_members.size() - 1]));
}

void Guild::addMember(Character* character, Shared::GuildRanks rank, const String& note)
{
	character->setGuild(this);
	_members.push_back(GuildMember(character, rank, note));
}

void Guild::addMember(const String& characterName, u16 characterLevel, 
					  Shared::GuildRanks rank, const String& note) {
	_members.push_back(GuildMember(characterName, characterLevel, rank, note));
}

bool Guild::addInvite(u32 senderID, u32 recipientID)
{
	if (_invites.find(recipientID) != _invites.end())
		return false;

	_invites[recipientID] = senderID;
	return true;
}

void Guild::removeMember(const String& memberName)
{
	for (auto it = _members.begin(); it != _members.end(); ++it) {
		if (memberName == it->getName()) {
			if (it->isOnline())
				it->getOnlineCharacter()->setGuild(nullptr);
			_members.erase(it);
			return;
		}
	}
}

void Guild::removeInvite(u32 recipientID)
{ 
	auto it = _invites.find(recipientID);
	if (it != _invites.end())
		_invites.erase(it);

	// Removes guild from memory if there's no active members and no invites.
	if (getActiveMembersCount() < 1 && _invites.empty())
		server->removeGuild(_id);
}

u32 Guild::getActiveMembersCount()
{
	u32 count = 0u;
	for (const auto& member : _members) {
		if (member.isOnline())
			++count;
	}
	return count;
}

GuildMember* Guild::getMember(const String& memberName)
{
	for (auto& member : _members) {
		if (memberName == member.getName())
			return &member;
	}
	return nullptr;
}

Character* Guild::getSenderOfInvite(u32 inviteID) {
	return server->getCharacter(_invites[inviteID]);
}

void Guild::setDescription(const SString& description)
{
	Shared::Guild::setDescription(description);
	_queriesQueue.addQueryData(QueryData(QueryData::ID::GUILD_SET_DESCRIPTION, this));
}

void Guild::setRankName(u8 index, const String& rankName)
{
	Shared::Guild::setRankName(index, rankName);
	_queriesQueue.addQueryData(QueryData(QueryData::ID::GUILD_SET_RANKS_NAMES, this));
}

void Guild::setMemberToOffline(const Character* memberCharacter)
{
	_queriesQueue.flush();

	for (auto& member : _members) {
		if (memberCharacter == member.getOnlineCharacter()) {
			member.setOnline(nullptr);
			break;
		}
	}

	// Removes guild from memory if there's no active members and no invites.
	if (getActiveMembersCount() < 1 && _invites.empty())
		server->removeGuild(_id);
}

void Guild::sendPacketToAllOnlineMembersInGuild(const BitStream* bitStream)
{
	for (u32 i = 0u; i < _members.size(); ++i) {
		auto member = getMember(i);
		if (member->isOnline()) {
			server->send(bitStream, Packet::LOW_PRIORITY,
							   member->getOnlineCharacter()->getClient()->getGUID());
		}
	}
}

void Guild::sendPacketToAllOnlineMembersInGuildExcept(const BitStream* bitStream,
													  const NetGUID& exceptGUID)
{
	for (u32 i = 0u; i < _members.size(); ++i) {
		auto member = getMember(i);
		if (member->isOnline()) {
			auto memberGUID = member->getOnlineCharacter()->getClient()->getGUID();
			if (memberGUID != exceptGUID)
				server->send(bitStream, Packet::LOW_PRIORITY, memberGUID);
		}
	}
}

void Guild::sendInviteCharacterPacket(Character* character)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_INVITE_CHARACTER);
	bitStream.write(character->getID()); // As ID of invite
	bitStream.write(getName());
	server->send(&bitStream, Packet::LOW_PRIORITY, character->getClient()->getGUID());
}

void Guild::sendJoinPacket(const NetGUID& guid)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_JOIN);
	bitStream.write(getName());
	bitStream.write(getTag());
	bitStream.write(getDescription());
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i)
		bitStream.write(getRankName(i));

	u16 membersCount = getMembersCount();
	bitStream.write(membersCount);
	for (u32 i = 0u; i < membersCount; ++i) {
		auto member = getMember(i);
		bitStream.write(member->getName());
		bitStream.write(member->isOnline());
		bitStream.write(member->getLevel());
		bitStream.write(static_cast<u8>(member->getRank()));
		bitStream.write(member->getNote());
	}
	server->send(&bitStream, Packet::LOW_PRIORITY, guid);
}

void Guild::sendSetDescriptionPacket()
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_SET_DESCRIPTION);
	bitStream.write(getDescription());
	sendPacketToAllOnlineMembersInGuild(&bitStream);
}

void Guild::sendSetRankNamePacket(u8 rankIndex, const String& rankName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_SET_RANK_NAME);
	bitStream.write(rankIndex);
	bitStream.write(rankName);
	sendPacketToAllOnlineMembersInGuild(&bitStream);
}

void Guild::sendAddMemberPacket(GuildMember* member)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_ADD_MEMBER);
	bitStream.write(member->getName());
	bitStream.write(member->getLevel());
	sendPacketToAllOnlineMembersInGuild(&bitStream);
}

void Guild::sendSetNotePacket(cchar* memberName, const String& memberNote)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_SET_MEMBER_NOTE);
	bitStream.write(memberName);
	bitStream.write(memberNote);
	sendPacketToAllOnlineMembersInGuild(&bitStream);
}

void Guild::sendSetMemberRankPacket(GuildMember* member)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_PROMOTE_MEMBER);
	bitStream.write(member->getName());
	bitStream.write(static_cast<u8>(member->getRank()));
	sendPacketToAllOnlineMembersInGuild(&bitStream);
}

void Guild::sendLeavePacket(cchar* memberName, bool disbandGuild)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_LEAVE);
	bitStream.write(disbandGuild);
	bitStream.write(memberName);
	sendPacketToAllOnlineMembersInGuild(&bitStream);
}

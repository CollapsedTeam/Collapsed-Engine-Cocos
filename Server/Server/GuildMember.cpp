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
#include "GuildMember.h"
#include "Character.h"
#include "Client.h"

GuildMember::GuildMember(Character* character, Shared::GuildRanks rank,
						 const String& note)
	: _rank(rank), _note(note)
{
	setOnline(character);
}

GuildMember::GuildMember(const String& characterName, u16 characterLevel, 
						 Shared::GuildRanks rank, const String& note)
	:  _onlineCharacter(nullptr), _rank(rank), _note(note)
{
	_offlineCharacter.name = characterName;
	_offlineCharacter.level = characterLevel;
}

GuildMember::~GuildMember() {
	_queriesQueue.flush();
}

void GuildMember::promote() {
	setRank(static_cast<Shared::GuildRanks>(static_cast<u8>(_rank) + 1u));
}

void GuildMember::degrade() {
	setRank(static_cast<Shared::GuildRanks>(static_cast<u8>(_rank) - 1u));
}

cchar* GuildMember::getName() const {
	return isOnline() ? _onlineCharacter->getName() : _offlineCharacter.name.c_str();
}

u16 GuildMember::getLevel() const {
	return isOnline() ? _onlineCharacter->getLevel() : _offlineCharacter.level;
}

void GuildMember::setOnline(Character* onlineCharacter)
{
	// Will character be offline?
	if (onlineCharacter == nullptr) {
		_queriesQueue.flush();
		_offlineCharacter.name = _onlineCharacter->getName();
		_offlineCharacter.level = _onlineCharacter->getLevel();
		sendGuildMemberBecomeOfflinePacket();
		_onlineCharacter = nullptr;
	} else {
		_offlineCharacter.name = "";
		_offlineCharacter.level = 0u;
		_onlineCharacter = onlineCharacter;
		sendGuildMemberBecomeOnlinePacket();
	}
}

void GuildMember::setRank(Shared::GuildRanks rank)
{
	_rank = rank;
	_queriesQueue.addQueryData(QueryData(QueryData::ID::GUILD_SET_MEMBER_RANK, this));
}

void GuildMember::setNote(const String& note)
{
	_note = note;
	_queriesQueue.addQueryData(QueryData(QueryData::ID::GUILD_SET_MEMBER_NOTE, this));
}

void GuildMember::sendGuildMemberBecomeOnlinePacket()
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_MEMBER_BECOME_ONLINE);
	bitStream.write(getName());
	_onlineCharacter->getGuild()->sendPacketToAllOnlineMembersInGuildExcept(
		&bitStream, _onlineCharacter->getClient()->getGUID());
}

void GuildMember::sendGuildMemberBecomeOfflinePacket()
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_MEMBER_BECOME_OFFLINE);
	bitStream.write(getName());
	_onlineCharacter->getGuild()->sendPacketToAllOnlineMembersInGuildExcept(
		&bitStream, _onlineCharacter->getClient()->getGUID());
}
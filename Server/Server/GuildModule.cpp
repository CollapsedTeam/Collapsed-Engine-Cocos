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
#include "GuildModule.h"
#include "Server.h"
#include "MySQL.h"
#include "CharacterModule.h"

void GuildModule::createGuild(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndNotInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, name);
	auto msg = Shared::Global::isValidGuildName(name);
	if (msg == Shared::Message::SUCCESS && isGuildNameExists(name))
		msg = Shared::Message::GUILD_NAME_EXISTS;

	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	}

	READ(bitStream, String, tag);
	msg = Shared::Global::isValidGuildTag(tag);
	if (msg == Shared::Message::SUCCESS && isGuildTagExists(tag))
		msg = Shared::Message::GUILD_TAG_EXISTS;

	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	}

	auto guild = new Guild(name, tag, client->getCharacter());
	MySQL::createGuild(guild);
	server->addGuild(guild);

	client->sendCreateGuildPacket(guild);
}

void GuildModule::setGuildDescription(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto character = client->getCharacter();
	auto guild = character->getGuild();
	if (!guild->getMember(character->getName())->isOwner()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, description);
	guild->setDescription(description);
	guild->sendSetDescriptionPacket();
}

void GuildModule::setGuildRankName(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto character = client->getCharacter();
	auto guild = character->getGuild();
	if (!guild->getMember(character->getName())->isOwner()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, u8, rankIndex);
	READ(bitStream, String, rankName);
	guild->setRankName(rankIndex, rankName);
	guild->sendSetRankNamePacket(rankIndex, rankName);
}

void GuildModule::inviteCharacterToGuild(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto senderCharacter = client->getCharacter();
	auto guild = senderCharacter->getGuild();
	auto member = guild->getMember(senderCharacter->getName());
	if (!member->isOwner() && !member->isManager()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, characterName);
	auto msg = Shared::Global::isValidName(characterName);
	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	} else if (!CharacterModule::isCharacterExists(characterName)) {
		sendMessagePacket(packet->guid, Shared::Message::CHARACTER_DIDNT_EXIST);
		return;
	}

	auto recipientCharacter = server->getCharacter(characterName);
	if (recipientCharacter == nullptr) {
		sendMessagePacket(packet->guid, Shared::Message::CHARACTER_OFFLINE);
		return;
	} else if (recipientCharacter->getGuild() != nullptr) {
		sendMessagePacket(packet->guid, Shared::Message::CHARACTER_ALREADY_IN_GUILD);
		return;
	}

	if (!guild->addInvite(senderCharacter->getID(), recipientCharacter->getID()))
		return;

	guild->sendInviteCharacterPacket(recipientCharacter);
}

void GuildModule::guildInvitationAnswer(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndNotInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, guildName);
	READ(bitStream, u32, inviteID);
	READ(bitStream, bool, accepted);

	auto guild = server->getGuild(guildName);
	if (accepted) {
		guild->sendJoinPacket(client->getGUID());
		guild->addNewMember(client->getCharacter(), Shared::GuildRanks::RECRUIT);
		guild->sendAddMemberPacket(guild->getMember(guild->getMembersCount() - 1));
	} else {
		auto sender = guild->getSenderOfInvite(inviteID);
		if (sender != nullptr) {
			sender->getClient()->sendInfoAboutRejectedGuildInvitationPacket(
				client->getCharacter()->getName());
		}
	}
	guild->removeInvite(inviteID);
}

void GuildModule::setGuildNote(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, note);
	auto character = client->getCharacter();
	auto characterName = character->getName();
	auto guild = character->getGuild();
	guild->getMember(characterName)->setNote(note);
	guild->sendSetNotePacket(characterName, note);
}

void GuildModule::promoteGuildMember(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto characterWhoPromote = client->getCharacter();
	auto guild = characterWhoPromote->getGuild();
	auto memberWhoPromote = guild->getMember(characterWhoPromote->getName());
	if (!memberWhoPromote->isOwner() && !memberWhoPromote->isManager()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, memberNameToPromote);
	auto memberToPromote = guild->getMember(memberNameToPromote);
	if (memberWhoPromote->getRank() <= memberToPromote->getRank()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	if (static_cast<u8>(memberToPromote->getRank()) >=
		static_cast<u8>(Shared::GuildRanks::OWNER)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	memberToPromote->promote();
	if (memberToPromote->isOwner()) {
		memberWhoPromote->setRank(Shared::GuildRanks::MANAGER);
		guild->sendSetMemberRankPacket(memberWhoPromote);
	}

	guild->sendSetMemberRankPacket(memberToPromote);
}

void GuildModule::degradeGuildMember(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto characterWhoDegrade = client->getCharacter();
	auto guild = characterWhoDegrade->getGuild();
	auto memberWhoDegrade = guild->getMember(characterWhoDegrade->getName());
	if (!memberWhoDegrade->isOwner() && !memberWhoDegrade->isManager()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, memberNameToDegrade);
	auto memberToDegrade = guild->getMember(memberNameToDegrade);
	if (memberWhoDegrade->getRank() <= memberToDegrade->getRank()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	if (static_cast<u8>(memberToDegrade->getRank()) < 1u) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	memberToDegrade->degrade();
	guild->sendSetMemberRankPacket(memberToDegrade);
}

void GuildModule::kickGuildMember(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	auto character = client->getCharacter();
	auto guild = character->getGuild();
	auto member = guild->getMember(character->getName());
	if (!member->isOwner() && !member->isManager()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, memberName);
	auto memberToKick = guild->getMember(memberName);
	if (memberToKick->getRank() >= member->getRank()) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	guild->sendLeavePacket(memberName.c_str(), false);
	deleteGuildMember(memberToKick);
	guild->removeMember(memberName);
}

void GuildModule::leaveGuild(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlayingAndInGuild(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, memberName);
	auto guild = client->getCharacter()->getGuild();
	auto member = guild->getMember(memberName);

	if (member->isOwner() || guild->getMembersCount() == 1) {
		guild->sendLeavePacket(memberName.c_str(), true);
		deleteAllGuildMembers(guild);
		deleteGuild(guild);
		server->removeGuild(guild->getID());
	} else {
		guild->sendLeavePacket(memberName.c_str(), false);
		deleteGuildMember(member);
		guild->removeMember(memberName);
	}
}

void GuildModule::deleteGuild(Guild* guild)
{
	char rest[128];
	sprintf(rest, "g.id=%u", guild->getID());
	MySQL::delete_("guilds g", rest);
}

void GuildModule::insertGuildMember(Guild* guild, GuildMember* guildMember)
{
	char values[256];
	sprintf(values, "%u,%u,%u", guildMember->getOnlineCharacter()->getID(), guild->getID(),
			static_cast<u8>(guildMember->getRank()));
	MySQL::insert("guildmembers(char_id,guild_id,rank)", values);
}

void GuildModule::setGuildDescription(Guild* guild)
{
	char set[512];
	sprintf(set, "g.description='%s'", guild->getDescription());

	char rest[64];
	sprintf(rest, "g.id=%u", guild->getID());

	MySQL::update("guilds g", set, rest);
}

void GuildModule::setGuildRanksNames(Guild* guild)
{
	char set[512];
	sprintf(set, "g.rank0='%s',g.rank1='%s',g.rank2='%s',g.rank3='%s'", guild->getRankName(0), 
			guild->getRankName(1), guild->getRankName(2), guild->getRankName(3));

	char rest[64];
	sprintf(rest, "g.id=%u", guild->getID());

	MySQL::update("guilds g", set, rest);
}

void GuildModule::setGuildMemberNote(GuildMember* member)
{
	char set[256];
	sprintf(set, "m.note='%s'", member->getNote());

	char rest[64];
	sprintf(rest, "m.char_id=%u", member->getOnlineCharacter()->getID());

	MySQL::update("guildmembers m", set, rest);
}

void GuildModule::setGuildMemberRank(GuildMember* member)
{
	char set[64];
	sprintf(set, "m.rank=%u", static_cast<u8>(member->getRank()));

	char rest[64];
	sprintf(rest, "c.name='%s'", member->getName());

	MySQL::update("guildmembers m INNER JOIN characters c ON c.id=m.char_id", set, rest);
}

void GuildModule::deleteGuildMember(GuildMember* guildMember)
{
	char query[256];
	sprintf(query, "DELETE m FROM guildmembers m INNER JOIN characters c ON c.id=m.char_id "
			"WHERE c.name='%s'", guildMember->getName());
	MySQL::execute(query);
}

void GuildModule::deleteAllGuildMembers(Guild* guild)
{
	char rest[256];
	sprintf(rest, "m.guild_id=%u", guild->getID());
	MySQL::delete_("guildmembers m", rest);
}

bool GuildModule::isGuildNameExists(const String& guildName)
{
	char rest[128];
	sprintf(rest, "g.name='%s'", guildName.c_str());
	return MySQL::isExists("guilds g", rest);
}

bool GuildModule::isGuildTagExists(const String& guildTag)
{
	char rest[128];
	sprintf(rest, "g.tag='%s'", guildTag.c_str());
	return MySQL::isExists("guilds g", rest);
}


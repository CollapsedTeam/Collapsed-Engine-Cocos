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
#include "Server.h"
#include "ConnectionModule.h"
#include "AccountModule.h"
#include "CharacterModule.h"
#include "ChatModule.h"
#include "GuildModule.h"
#include "DialogueModule.h"

#define PACKET_RECEIVED(id) Console::writeLine(Debug("Packet " id " has been received."))

void Server::handlePacket(Packet* packet, BitStream* bitStream)
{
	using namespace Shared;

	switch (static_cast<PacketID>(packet->data[0])) {
		case PacketID::NEW_INCOMING_CONNECTION: {
			PACKET_RECEIVED("NEW_INCOMING_CONNECTION");
			ConnectionModule::newIncomingConnection(packet);
		}
		break;

		case PacketID::DISCONNECTION: {
			PACKET_RECEIVED("DISCONNECTION");
			ConnectionModule::disconnectionNotification(packet);
		}
		break;

		case PacketID::CONNECTION_LOST: {
			PACKET_RECEIVED("CONNECTION_LOST");
			ConnectionModule::connectionLost(packet);
		}
		break;

		case PacketID::ACCOUNT_CREATE: {
			PACKET_RECEIVED("ACCOUNT_CREATE");
			AccountModule::createAccount(packet, bitStream);
		}
		break;

		case PacketID::ACCOUNT_GENERATE: {
			PACKET_RECEIVED("ACCOUNT_GENERATE");
			AccountModule::generateAccount(packet);
		}
		break;

		case PacketID::ACCOUNT_CHANGE_DATA: {
			PACKET_RECEIVED("ACCOUNT_CHANGE_DATA");
			AccountModule::changeAccountData(packet, bitStream);
		}
		break;

		case PacketID::ACCOUNT_DELETE: {
			PACKET_RECEIVED("ACCOUNT_DELETE");
			AccountModule::removeAccount(packet);
		}
		break;

		case PacketID::CHARACTER_CREATE: {
			PACKET_RECEIVED("CHARACTER_CREATE");
			CharacterModule::createCharacter(packet, bitStream);
		}
		break;

		case PacketID::LOGIN: {
			PACKET_RECEIVED("LOGIN");
			AccountModule::login(packet, bitStream);
		}
		break;

		case PacketID::CHAT: {
			PACKET_RECEIVED("CHAT");
			ChatModule::chat(packet, bitStream);
		}
		break;

		case PacketID::GUILD_CREATE: {
			PACKET_RECEIVED("GUILD_CREATE");
			GuildModule::createGuild(packet, bitStream);
		}
		break;

		case PacketID::GUILD_SET_DESCRIPTION: {
			PACKET_RECEIVED("GUILD_SET_DESCRIPTION");
			GuildModule::setGuildDescription(packet, bitStream);
		}
		break;

		case PacketID::GUILD_SET_RANK_NAME: {
			PACKET_RECEIVED("GUILD_SET_RANK_NAME");
			GuildModule::setGuildRankName(packet, bitStream);
		}
		break;

		case PacketID::GUILD_INVITE_CHARACTER: {
			PACKET_RECEIVED("GUILD_INVITE_CHARACTER");
			GuildModule::inviteCharacterToGuild(packet, bitStream);
		}
		break;

		case PacketID::GUILD_INVITATION_ANSWER: {
			PACKET_RECEIVED("GUILD_INVITATION_ANSWER");
			GuildModule::guildInvitationAnswer(packet, bitStream);
		}
		break;

		case PacketID::GUILD_SET_MEMBER_NOTE: {
			PACKET_RECEIVED("GUILD_SET_MEMBER_NOTE");
			GuildModule::setGuildNote(packet, bitStream);
		}
		break;

		case PacketID::GUILD_PROMOTE_MEMBER: {
			PACKET_RECEIVED("GUILD_PROMOTE_MEMBER");
			GuildModule::promoteGuildMember(packet, bitStream);
		}
		break;

		case PacketID::GUILD_DEGRADE_MEMBER: {
			PACKET_RECEIVED("GUILD_DEGRADE_MEMBER");
			GuildModule::degradeGuildMember(packet, bitStream);
		}
		break;

		case PacketID::GUILD_KICK_MEMBER: {
			PACKET_RECEIVED("GUILD_KICK_MEMBER");
			GuildModule::kickGuildMember(packet, bitStream);
		}
		break;

		case PacketID::GUILD_LEAVE: {
			PACKET_RECEIVED("GUILD_LEAVE");
			GuildModule::leaveGuild(packet, bitStream);
		}
		break;

		case PacketID::DIALOGUE_INIT: {
			PACKET_RECEIVED("DIALOGUE_INIT");
			DialogueModule::dialogueInit(packet, bitStream);
		}
		break;

		case PacketID::DIALOGUE_UPDATE: {
			PACKET_RECEIVED("DIALOGUE_UPDATE");
			DialogueModule::dialogueUpdate(packet, bitStream);
		}
		break;

		default: {
			PACKET_RECEIVED("UNKNOWN");
			Module::unknown(packet);
		}
		break;
	}
}
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
#include "Packets.h"

void Client::handlePacket(Packet* packet, BitStream* bitStream)
{
	using namespace Shared;

	switch (static_cast<PacketID>(packet->data[0])) {
		case PacketID::CONNECTION_REQUEST_ACCEPTED:
			Packets::connectionRequestAccepted();
			break;

		case PacketID::CONNECTION_ATTEMPT_FAILED:
			Packets::connectionAttemptFailed();
			break;
		
		case PacketID::ALREADY_CONNECTED:
			Packets::alreadyConnected();
			break;

		case PacketID::DISCONNECTION:
			Packets::disconnectionNotification();
			break;

		case PacketID::CONNECTION_LOST:
			Packets::connectionLost();
			break;

		case PacketID::IP_BANNED:
			Packets::ipBanned();
			break;

		case PacketID::MESSAGE:
			Packets::message(bitStream);
			break;

		case PacketID::ACCOUNT_CREATE:
			Packets::createAccount(bitStream);
			break;

		case PacketID::ACCOUNT_GENERATE:
			Packets::generateAccount(bitStream);
			break;

		case PacketID::ACCOUNT_CHANGE_DATA:
			Packets::changeAccountData(bitStream);
			break;

		case PacketID::ACCOUNT_DELETE:
			Packets::deleteAccount(bitStream);
			break;

		case PacketID::CHARACTER_CREATE:
			Packets::createCharacter(bitStream);
			break;

		case PacketID::CHARACTER_LEVEL_UP:
			Packets::characterLevelUp(bitStream);
			break;

		case PacketID::LOGIN:
			Packets::login(bitStream);
			break;

		case PacketID::CHAT:
			Packets::chat(bitStream);
			break;

		case PacketID::GUILD_CREATE:
			Packets::createGuild(bitStream);
			break;

		case PacketID::GUILD_SET_DESCRIPTION:
			Packets::setGuildDescription(bitStream);
			break; 
		
		case PacketID::GUILD_SET_RANK_NAME:
			Packets::setGuildRankName(bitStream);
			break;

		case PacketID::GUILD_INVITE_CHARACTER:
			Packets::inviteCharacterToGuild(bitStream);
			break;

		case PacketID::GUILD_INVITATION_ANSWER:
			Packets::guildInvitationAnswer(bitStream);
			break;

		case PacketID::GUILD_JOIN:
			Packets::joinGuild(bitStream);
			break;

		case PacketID::GUILD_MEMBER_BECOME_ONLINE:
			Packets::guildMemberBecomeOnline(bitStream);
			break;

		case PacketID::GUILD_MEMBER_BECOME_OFFLINE:
			Packets::guildMemberBecomeOffline(bitStream);
			break;

		case PacketID::GUILD_ADD_MEMBER:
			Packets::addGuildMember(bitStream);
			break;

		case PacketID::GUILD_SET_MEMBER_NOTE:
			Packets::setGuildNote(bitStream);
			break;

		case PacketID::GUILD_PROMOTE_MEMBER:
		case PacketID::GUILD_DEGRADE_MEMBER:
			Packets::setGuildMemberRank(bitStream);
			break;

		case PacketID::GUILD_LEAVE:
			Packets::leaveGuild(bitStream);
			break;

		case PacketID::DIALOGUE_INIT:
			Packets::dialogueInit(bitStream);
			break;

		case PacketID::DIALOGUE_START:
			Packets::dialogueStart(bitStream);
			break;

		case PacketID::DIALOGUE_UPDATE:
			Packets::dialogueUpdate(bitStream);
			break;

		default: {
			CCLOG("Unknown packet: %u", packet->data[0]);
			Packets::unknown();
		}
		break;
	}
}
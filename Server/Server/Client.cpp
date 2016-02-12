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
#include "Client.h"
#include "Server.h"

Client::Client(const NetGUID& guid, const String& email)
	: Account(email), _guid(guid)
{ }

void Client::sendCreateCharacterPacket()
{
	auto classes = server->getClasses();

	BITSTREAM(bitStream, Shared::PacketID::CHARACTER_CREATE);
	bitStream.write(server->getClassesCount());
	for (auto it = classes->begin(); it != classes->end(); ++it) {
		auto class_ = it->second;
		bitStream.write(class_->getID());
		bitStream.write(class_->getName());
		bitStream.write(class_->getHealth());
		bitStream.write(class_->getPower());
		bitStream.write(class_->getPhysique());
		bitStream.write(class_->getDexterity());
		bitStream.write(class_->getIntelligence());
		bitStream.write(class_->getHealthGain());
		bitStream.write(class_->getPowerGain());
		bitStream.write(class_->getPhysiqueGain());
		bitStream.write(class_->getDexterityGain());
		bitStream.write(class_->getIntelligenceGain());
	}
	server->send(&bitStream, Packet::MEDIUM_PRIORITY, _guid);
}

void Client::sendLoginPacket()
{
	auto character = getCharacter();

	BITSTREAM(bitStream, Shared::PacketID::LOGIN);
	bitStream.write(character->getID());
	bitStream.write(character->getName());
	bitStream.write(character->getLevel());
	bitStream.write(character->isMale());
	bitStream.write(character->getCash());
	bitStream.write(character->getHealth());
	bitStream.write(character->getPower());
	bitStream.write(character->getPhysique());
	bitStream.write(character->getDexterity());
	bitStream.write(character->getIntelligence());
	server->send(&bitStream, Packet::MEDIUM_PRIORITY, _guid);

	if (character->isBelongsToGuild())
		character->getGuild()->sendJoinPacket(_guid);
}

void Client::sendChangeAccountDataPacket(const String& email, const String& password)
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_CHANGE_DATA);
	bitStream.write(email);
	bitStream.write(password);
	server->send(&bitStream, Packet::LOW_PRIORITY, _guid);
}

void Client::sendDeleteAccountPacket()
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_DELETE);
	server->send(&bitStream, Packet::LOW_PRIORITY, _guid);
}

void Client::sendCreateGuildPacket(Guild* guild)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_CREATE);
	bitStream.write(guild->getName());
	bitStream.write(guild->getTag());
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i)
		bitStream.write(guild->getRankName(i));
	server->send(&bitStream, Packet::MEDIUM_PRIORITY, _guid);
}

void Client::sendInfoAboutRejectedGuildInvitationPacket(cchar* characterName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_INVITATION_ANSWER);
	bitStream.write(characterName);
	server->send(&bitStream, Packet::LOW_PRIORITY, _guid);
}

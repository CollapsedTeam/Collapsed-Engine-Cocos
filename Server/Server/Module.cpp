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
#include "Client.h"

void Module::unknown(Packet* packet)
{
	char buffer[128];
	sprintf(buffer, "Unknown packet from: %s", packet->systemAddress.toString(true).c_str());
	Console::writeLine(Warn(buffer));
}

void Module::sendMessagePacket(const NetGUID& guid, Shared::Message msg)
{
	BITSTREAM(bitStream, Shared::PacketID::MESSAGE);
	bitStream.write(static_cast<s32>(msg));
	server->send(&bitStream, Packet::LOW_PRIORITY, guid);
}

bool Module::isNotNullAndIsPlaying(Client* client) {
	return (client != nullptr && client->isPlaying());
}

bool Module::isNotNullAndIsPlayingAndInGuild(Client* client) {
	return (isNotNullAndIsPlaying(client) && client->getCharacter()->isBelongsToGuild());
}

bool Module::isNotNullAndIsPlayingAndNotInGuild(Client* client) {
	return (isNotNullAndIsPlaying(client) && !client->getCharacter()->isBelongsToGuild());
}
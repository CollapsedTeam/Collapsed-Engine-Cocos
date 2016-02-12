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
#include "DialogueModule.h"
#include "Server.h"
#include "MySQL.h"

void DialogueModule::dialogueInit(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlaying(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, u32, dialogueID);
	auto dialogue = server->getDialogue(dialogueID);
	auto character = client->getCharacter();
	if (!dialogue->init(character))
		return;
	dialogue->start(character);
}

void DialogueModule::dialogueUpdate(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	if (!isNotNullAndIsPlaying(client)) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	READ(bitStream, String, id);
	auto character = client->getCharacter();
	character->getDialogue()->update(character, id);
}

void DialogueModule::insertDialogueFlag(Character* character, Dialogue* dialogue, u8 value)
{
	char values[128];
	sprintf(values, "%u,%u,%u", dialogue->getID(), character->getID(), value);
	MySQL::insert("dialogues(dial_id,char_id,value)", values);
}
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
#include "Dialogue.h"
#include "Character.h"
#include "Server.h"

OOLUA_EXPORT_FUNCTIONS_CONST(Dialogue, getID);
OOLUA_EXPORT_FUNCTIONS(Dialogue);

Dialogue::Dialogue(u32 id)
	: _id(id)
{ 
	char buffer[64];
	sprintf(buffer, "Data/Dialogues/%u.lua", id);
	load(buffer);
}

bool Dialogue::init(Character* character)
{
	_lua.call("init", this, character);
	
	bool canStartDialogue = false;
	_lua.pull(canStartDialogue);
	if (canStartDialogue) {
		character->setDialogue(this);
	} else {
		sendDialogueInitFailedPacket(character);
	}
	return canStartDialogue;
}

void Dialogue::start(Character* character)
{
	_lua.call("start", this, character);
	
	String initialTextID;
	std::vector<String> optionsIDs;
	pullTextAndOptions(initialTextID, optionsIDs);
	sendDialogueStartPacket(character, initialTextID, optionsIDs);
}

void Dialogue::update(Character* character, const String& id)
{
	_lua.call("update", this, character, id);

	String textID;
	std::vector<String> optionsIDs;
	pullTextAndOptions(textID, optionsIDs);
	if (textID == "[END]")
		character->clearFlags();

	sendDialogueUpdatePacket(character, textID, optionsIDs);
}

void Dialogue::registerGlobals()
{
	Script::registerGlobals();
	
	const u8 CONST_TRUE = 1;
	OOLUA::set_global(_lua, "TRUE", CONST_TRUE);

	const u8 CONST_FALSE = 0;
	OOLUA::set_global(_lua, "FALSE", CONST_FALSE);
}

void Dialogue::registerClasses()
{
	Script::registerClasses();

	_lua.register_class<SharedCharacter>();
	_lua.register_class<Character>();
	_lua.register_class<SharedClass>();
	_lua.register_class<Class>();
	_lua.register_class<Dialogue>();
}

void Dialogue::pullTextAndOptions(String& textID, std::vector<String>& optionsIDs)
{
	if (_lua.stack_count() > 1) {
		optionsIDs.resize(_lua.stack_count() - 1);
		for (s32 i = _lua.stack_count() - 2; i > -1; --i)
			_lua.pull(optionsIDs[i]);
	}
	_lua.pull(textID);
}

void Dialogue::sendDialogueInitFailedPacket(Character* character)
{
	BITSTREAM(bitStream, Shared::PacketID::DIALOGUE_INIT);
	server->send(&bitStream, Packet::LOW_PRIORITY, character->getClient()->getGUID());
}

void Dialogue::sendDialogueStartPacket(Character* character, const String& initialTextID,
									   const std::vector<String>& optionsIDs)
{
	BITSTREAM(bitStream, Shared::PacketID::DIALOGUE_START);
	bitStream.write(_id);
	bitStream.write(initialTextID);
	bitStream.write(static_cast<u32>(optionsIDs.size()));
	for (const auto& optionID : optionsIDs)
		bitStream.write(optionID);
	server->send(&bitStream, Packet::LOW_PRIORITY, character->getClient()->getGUID());
}

void Dialogue::sendDialogueUpdatePacket(Character* character, const String& textID, 
										const std::vector<String>& optionsIDs)
{
	BITSTREAM(bitStream, Shared::PacketID::DIALOGUE_UPDATE);
	bitStream.write(textID);
	bitStream.write(static_cast<u32>(optionsIDs.size()));
	for (const auto& optionID : optionsIDs)
		bitStream.write(optionID);
	server->send(&bitStream, Packet::LOW_PRIORITY, character->getClient()->getGUID());
}

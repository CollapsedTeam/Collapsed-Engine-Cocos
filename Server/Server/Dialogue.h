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

#include "Script.h"

class Character;

class Dialogue : public Script
{
public:
	Dialogue(u32 id);

	bool init(Character* character);

	void start(Character* character);

	void update(Character* character, const String& id);

	inline u32 getID() const {
		return _id;
	}

protected:
	void registerGlobals();

	void registerClasses();

private:
	void pullTextAndOptions(String& textID, std::vector<String>& optionsIDs);
	
	void sendDialogueInitFailedPacket(Character* character);

	void sendDialogueStartPacket(Character* character, const String& initialTextID, 
								 const std::vector<String>& optionsIDs);
	
	void sendDialogueUpdatePacket(Character* character, const String& textID, 
								  const std::vector<String>& optionsIDs);

protected:
	u32 _id;
};

OOLUA_PROXY(Dialogue)
OOLUA_TAGS(OOLUA::No_default_constructor)
OOLUA_MFUNC_CONST(getID)
OOLUA_PROXY_END
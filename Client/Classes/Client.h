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

#include "Character.h"
#include "Options.h"
#include "App.h"

class Client : public App, public Jinra::NetClient
{
public:
	Client();

	void showMsgBoxWithValidationMsg(Shared::Message msg);

	void saveLoginFile(cchar* email, cchar* password);

	void loadLoginFile(SString& email, SString& password);

	void deleteLoginFile();
	
	void reactToSocketStartupResult(Jinra::NetPeer::StartupResult startupResult);

	bool isLoginFileExist() {
		return FileUtils::getInstance()->isFileExist(getFullPathOfLoginFile());
	}

	Character* getCharacter() {
		return _character;
	}

	Options* getOptionsWindow() {
		return &_options;
	}

	void setCharacter(Character* character) {
		_character = character;
	}

protected:
	void handlePacket(Packet* packet, BitStream* bitStream) override;

private:
	SString getFullPathOfLoginFile();

private:
	RefPtr<Character> _character;
	Options _options;
};
extern Client* game;

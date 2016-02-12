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
#include "MsgBox.h"

Client* game = nullptr;

Vec2 centerPoint = Vec2(DESIGN_RESOLUTION_WIDTH / 2.0f, DESIGN_RESOLUTION_HEIGHT / 2.0f);

Client::Client() { 
	game = this;
}

void Client::showMsgBoxWithValidationMsg(Shared::Message msg)
{
	using namespace Shared;

	auto set = game->getLang()->getSetElement("ValidationMsg");
	auto ok = LANG_TEXT(set, "MB_OK");

	auto msgBox = MsgBox::create();
	switch (msg) {
		case Message::EMAIL_TOO_SHORT: {
			msgBox->init(LANG_TEXT(set, "MB_EMAIL_TOO_SHORT"), 
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;
		
		case Message::EMAIL_TOO_LONG: {
			msgBox->init(LANG_TEXT(set, "MB_EMAIL_TOO_LONG"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::EMAIL_INVALID_STRING: {
			msgBox->init(LANG_TEXT(set, "MB_EMAIL_INVALID_STRING"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::EMAIL_EXISTS: {
			msgBox->init(LANG_TEXT(set, "MB_EMAIL_EXISTS"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::PASSWORD_TOO_SHORT: {
			msgBox->init(LANG_TEXT(set, "MB_PASSWORD_TOO_SHORT"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::PASSWORD_TOO_LONG: {
			msgBox->init(LANG_TEXT(set, "MB_PASSWORD_TOO_LONG"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::PASSWORD_INVALID_STRING: {
			msgBox->init(LANG_TEXT(set, "MB_PASSWORD_INVALID_STRING"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::EMAIL_OR_PASSWORD_IS_INCORRECT: {
			msgBox->init(LANG_TEXT(set, "MB_EMAIL_OR_PASSWORD_IS_INCORRECT"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::NAME_TOO_SHORT: {
			msgBox->init(LANG_TEXT(set, "MB_NAME_TOO_SHORT"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::NAME_TOO_LONG: {
			msgBox->init(LANG_TEXT(set, "MB_NAME_TOO_LONG"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::NAME_INVALID_STRING: {
			msgBox->init(LANG_TEXT(set, "MB_NAME_INVALID_STRING"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::NAME_EXISTS: {
			msgBox->init(LANG_TEXT(set, "MB_NAME_EXISTS"),
						 "", nullptr, 
						 ok, [=](Ref*) { msgBox->hide(); }, 
						 "", nullptr);
		}
		break;

		case Message::ACCOUNT_IN_USE: {
			msgBox->init(LANG_TEXT(set, "MB_ACCOUNT_IN_USE"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::ACCOUNT_BANNED: {
			msgBox->init(LANG_TEXT(set, "MB_ACCOUNT_BANNED"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::GUILD_NAME_TOO_SHORT: {
			msgBox->init(LANG_TEXT(set, "MB_GUILD_NAME_TOO_SHORT"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::GUILD_NAME_TOO_LONG: {
			msgBox->init(LANG_TEXT(set, "MB_GUILD_NAME_TOO_LONG"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::GUILD_NAME_EXISTS: {
			msgBox->init(LANG_TEXT(set, "MB_GUILD_NAME_EXISTS"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::GUILD_TAG_TOO_SHORT: {
			msgBox->init(LANG_TEXT(set, "MB_GUILD_TAG_TOO_SHORT"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::GUILD_TAG_TOO_LONG: {
			msgBox->init(LANG_TEXT(set, "MB_GUILD_TAG_TOO_LONG"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::GUILD_TAG_EXISTS: {
			msgBox->init(LANG_TEXT(set, "MB_GUILD_TAG_EXISTS"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::CHARACTER_DIDNT_EXIST: {
			msgBox->init(LANG_TEXT(set, "MB_CHARACTER_DIDNT_EXIST"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::CHARACTER_OFFLINE: {
			msgBox->init(LANG_TEXT(set, "MB_CHARACTER_OFFLINE"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::CHARACTER_ALREADY_IN_GUILD: {
			msgBox->init(LANG_TEXT(set, "MB_CHARACTER_ALREADY_IN_GUILD"),
						 "", nullptr,
						 ok, [=](Ref*) { msgBox->hide(); },
						 "", nullptr);
		}
		break;

		case Message::HACK: {
			msgBox->init(LANG_TEXT(set, "MB_HACK"),
						 "", nullptr, 
						 ok, [](Ref*) { game->exit(); },
						 "", nullptr);
		}
		break;
	}
	msgBox->show();
}

void Client::saveLoginFile(cchar* email, cchar* password)
{
	// Creates login file.
	auto file = fopen(getFullPathOfLoginFile().c_str(), "wb");
	if (file == nullptr) {
		game->exit();
		return;
	}

	// Writes e-mail length.
	s32 emailLen = strlen(email);
	fwrite(&emailLen, sizeof(emailLen), 1, file);

	// Writes e-mail
	fwrite(email, sizeof(char), emailLen, file);

	// Writes password length.
	s32 passwordLen = strlen(password);
	fwrite(&passwordLen, sizeof(passwordLen), 1, file);
	
	// Writes password
	fwrite(password, sizeof(char), passwordLen, file);
	
	// Saves changes and closes file.
	fclose(file);

	// Set email in options window.
	game->getOptionsWindow()->setEmail(email);
}

void Client::loadLoginFile(SString& email, SString& password)
{
	// Opens login file.
	auto file = fopen(getFullPathOfLoginFile().c_str(), "rb");
	if (file == nullptr) {
		game->exit();
		return;
	}

	// Reads e-mail length.
	s32 emailLen = 0;
	fread(&emailLen, sizeof(emailLen), 1, file);

	// Reads e-mail
	email.resize(emailLen);
	fread(&email[0], sizeof(char), emailLen, file);

	// Reads password length.
	s32 passwordLen = 0;
	fread(&passwordLen, sizeof(passwordLen), 1, file);

	// Reads password
	password.resize(passwordLen);
	fread(&password[0], sizeof(char), passwordLen, file);
	
	// Closes file.
	fclose(file);

	// Set email in options window.
	game->getOptionsWindow()->setEmail(email);
}

void Client::deleteLoginFile() {
	FileUtils::getInstance()->removeFile(getFullPathOfLoginFile());
}

using StartupResult = Jinra::NetPeer::StartupResult;
void Client::reactToSocketStartupResult(StartupResult startupResult)
{
	// TODO: Handle it.
	switch (startupResult) {
		case StartupResult::ALREADY_STARTED:
			CCLOG("ALREADY_STARTED");
			break;

		case StartupResult::INVALID_SOCKET_DESCRIPTORS:
			CCLOG("INVALID_SOCKET_DESCRIPTORS");
			break;

		case StartupResult::INVALID_MAX_CONNECTIONS:
			CCLOG("INVALID_MAX_CONNECTIONS");
			break;

		case StartupResult::SOCKET_FAMILY_NOT_SUPPORTED:
			CCLOG("SOCKET_FAMILY_NOT_SUPPORTED");
			break;

		case StartupResult::SOCKET_PORT_ALREADY_IN_USE:
			CCLOG("SOCKET_PORT_ALREADY_IN_USE");
			break;

		case StartupResult::SOCKET_FAILED_TO_BIND:
			CCLOG("SOCKET_FAILED_TO_BIND");
			break;

		case StartupResult::SOCKET_FAILED_TEST_SEND:
			CCLOG("SOCKET_FAILED_TEST_SEND");
			break;

		case StartupResult::PORT_CANNOT_BE_ZERO:
			CCLOG("PORT_CANNOT_BE_ZERO");
			break;

		case StartupResult::FAILED_TO_CREATE_NETWORK_THREAD:
			CCLOG("FAILED_TO_CREATE_NETWORK_THREAD");
			break;

		case StartupResult::COULD_NOT_GENERATE_GUID:
			CCLOG("COULD_NOT_GENERATE_GUID");
			break;

		case StartupResult::OTHER_FAILURE:
			CCLOG("STARTUP_OTHER_FAILURE");
			break;
	}
}

SString Client::getFullPathOfLoginFile()
{
	auto filePath = FileUtils::getInstance()->getWritablePath(); 
	filePath += "login.dat";
	return filePath;
}
/*
 * Copyright 2015 Collapsed Team
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

namespace Shared 
{

#define MIN_LEN_EMAIL 6
#define MAX_LEN_EMAIL 64
#define MIN_LEN_PASSWORD 6
#define MAX_LEN_PASSWORD 16
#define MIN_LEN_NAME 4
#define MAX_LEN_NAME 16
#define MIN_LEN_GUILD_NAME 3
#define MAX_LEN_GUILD_NAME 24
#define MIN_LEN_GUILD_TAG 3
#define MAX_LEN_GUILD_TAG 8
#define GUEST_PREFIX "guest"

enum class PacketID : u8
{
	CONNECTION_REQUEST_ACCEPTED = ID_CONNECTION_REQUEST_ACCEPTED,
	CONNECTION_ATTEMPT_FAILED = ID_CONNECTION_ATTEMPT_FAILED,
	ALREADY_CONNECTED = ID_ALREADY_CONNECTED,
	NEW_INCOMING_CONNECTION = ID_NEW_INCOMING_CONNECTION,
	DISCONNECTION = ID_DISCONNECTION_NOTIFICATION,
	CONNECTION_LOST = ID_CONNECTION_LOST,
	IP_BANNED = ID_CONNECTION_BANNED,
	MESSAGE = ID_USER_PACKET_ENUM,

	ACCOUNT_CREATE,
	ACCOUNT_GENERATE,
	ACCOUNT_CHANGE_DATA,
	ACCOUNT_DELETE,

	CHARACTER_CREATE,
	CHARACTER_LEVEL_UP,

	LOGIN,

	CHAT,

	GUILD_CREATE,
	GUILD_SET_DESCRIPTION,
	GUILD_SET_RANK_NAME,
	GUILD_INVITE_CHARACTER,
	GUILD_INVITATION_ANSWER,
	GUILD_JOIN,
	GUILD_MEMBER_BECOME_ONLINE,
	GUILD_MEMBER_BECOME_OFFLINE,
	GUILD_ADD_MEMBER,
	GUILD_SET_MEMBER_NOTE,
	GUILD_PROMOTE_MEMBER,
	GUILD_DEGRADE_MEMBER,
	GUILD_KICK_MEMBER,
	GUILD_LEAVE,

	DIALOGUE_INIT,
	DIALOGUE_START,
	DIALOGUE_UPDATE,

	COUNT
};

enum class Message : u8
{
	SUCCESS,

	EMAIL_TOO_SHORT,
	EMAIL_TOO_LONG,
	EMAIL_INVALID_STRING,
	EMAIL_EXISTS,

	PASSWORD_TOO_SHORT,
	PASSWORD_TOO_LONG,
	PASSWORD_INVALID_STRING,

	EMAIL_OR_PASSWORD_IS_INCORRECT,

	NAME_TOO_SHORT,
	NAME_TOO_LONG,
	NAME_INVALID_STRING,
	NAME_EXISTS,

	ACCOUNT_IN_USE,
	ACCOUNT_BANNED,

	GUILD_NAME_TOO_SHORT,
	GUILD_NAME_TOO_LONG,
	GUILD_NAME_INVALID_STRING,
	GUILD_NAME_EXISTS,

	GUILD_TAG_TOO_SHORT,
	GUILD_TAG_TOO_LONG,
	GUILD_TAG_EXISTS,

	CHARACTER_DIDNT_EXIST,
	CHARACTER_OFFLINE,
	CHARACTER_ALREADY_IN_GUILD,

	HACK
};

enum class ChatChannelID : u8
{
	LOCAL,
	GLOBAL,
	PARTY,
	GUILD,
	SYSTEM
};

class Global
{
public:
	static bool hasUpperCase(const SString& str) {
		return std::any_of(str.begin(), str.end(), ::isupper);
	}

	static bool hasNumber(const SString& str) {
		return std::any_of(str.begin(), str.end(), Global::isNumber);
	}

	static bool isCharacter(const char character) {
		return ((character >= 'a' && character <= 'z') ||
				(character >= 'A' && character <= 'Z'));
	}

	static bool isNumber(const char character) {
		return (character >= '0' && character <= '9');
	}

	static bool isValidString(const SString& str)
	{
		const char allowedCharacters[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"._-@";

		return (str.find_first_not_of(allowedCharacters) == SString::npos);
	}

	static Message isValidEmail(const SString& email, bool checkPrefix = true)
	{
		if (email.length() < MIN_LEN_EMAIL)		
			return Message::EMAIL_TOO_SHORT;
		
		if (email.length() > MAX_LEN_EMAIL) 
			return Message::EMAIL_TOO_LONG;

		if (!isCharacter(email[0]))	
			return Message::EMAIL_INVALID_STRING;

		if (!isValidString(email))
			return Message::EMAIL_INVALID_STRING;

		if (checkPrefix) {
			if (email.compare(0, strlen(GUEST_PREFIX), GUEST_PREFIX) == 0)
				return Message::EMAIL_INVALID_STRING;

			auto atIt = email.find_last_of("@");
			auto dotIt = email.find_last_of(".");
			if (atIt == SString::npos || dotIt == SString::npos || atIt > dotIt ||
				dotIt >= email.length() - 1)
				return Message::EMAIL_INVALID_STRING;
		}

		return Message::SUCCESS;
	}

	static Message isValidPassword(const SString& password) {
		if (password.length() < MIN_LEN_PASSWORD)	   
			return Message::PASSWORD_TOO_SHORT;

		if (password.length() > MAX_LEN_PASSWORD) 
			return Message::PASSWORD_TOO_LONG;

		if (!isValidString(password))
			return Message::PASSWORD_INVALID_STRING;

		if (!hasUpperCase(password))
			return Message::PASSWORD_INVALID_STRING;

		if (!hasNumber(password))
			return Message::PASSWORD_INVALID_STRING;

		return Message::SUCCESS;
	}

	static Message isValidName(const SString& name) {
		if (name.length() < MIN_LEN_NAME)	
			return Message::NAME_TOO_SHORT;

		if (name.length() > MAX_LEN_NAME)	
			return Message::NAME_TOO_LONG;

		if (!isValidString(name))
			return Message::NAME_INVALID_STRING;

		return Message::SUCCESS;
	}

	static Message isValidGuildName(const SString& guildName) {
		if (guildName.length() < MIN_LEN_GUILD_NAME) 
			return Message::GUILD_NAME_TOO_SHORT;
		
		if (guildName.length() > MAX_LEN_GUILD_NAME)
			return Message::GUILD_NAME_TOO_LONG;
		
		if (!isValidString(guildName))
			return Message::GUILD_NAME_INVALID_STRING;
		
		return Message::SUCCESS;
	}

	static Message isValidGuildTag(const SString& guildTag) {
		if (guildTag.length() < MIN_LEN_GUILD_TAG)
			return Message::GUILD_TAG_TOO_SHORT;

		if (guildTag.length() > MAX_LEN_GUILD_TAG)
			return Message::GUILD_TAG_TOO_LONG;

		return Message::SUCCESS;
	}
};

}

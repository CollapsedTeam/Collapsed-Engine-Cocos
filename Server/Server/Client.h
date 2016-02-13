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

#include "Account.h"

class Client : public Account
{
public:
	/**
	 * 
	 * @param guid
	 * @param email 
	 */
	Client(const NetGUID& guid, const String& email);

	/**
	 * 
	 * @return 
	 */
	const NetGUID& getGUID() const {
		return _guid;
	}

public:
	/**
	 *
	 */
	void sendCreateCharacterPacket();

	/**
	 * 
	 * @param client 
	 */
	void sendLoginPacket();

	/**
	 *
	 * @param email
	 * @param password 
	 */
	void sendChangeAccountDataPacket(const String& email, const String& password);
	
	/**
	 * 
	 */
	void sendDeleteAccountPacket();

	/**
	 * @param guild
	 */
	void sendCreateGuildPacket(Guild* guild);

	/**
	 *
	 * @param characterName 
	 */
	void sendInfoAboutRejectedGuildInvitationPacket(cchar* characterName);

protected:
	NetGUID _guid;
};

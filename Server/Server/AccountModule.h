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

class Account;

class AccountModule : public Module
{
public: /* Network Handle */
	/**
	 * 
	 * @param packet
	 * @param bitStream
	 */
	static void createAccount(Packet* packet, BitStream* bitStream);

	/**
	 * 
	 * @param packet
	 * @param bitStream 
	 */
	static void generateAccount(Packet* packet);

	/**
	 * 
	 * @param packet
	 * @param bitStream 
	 */
	static void login(Packet* packet, BitStream* bitStream);
	
	/**
	 * 
	 * @param packet
	 * @param bitStream 
	 */
	static void changeAccountData(Packet* packet, BitStream* bitStream);
	
	/**
	 * 
	 * @param packet 
	 * @param bitStream
	 */
	static void removeAccount(Packet* packet);

public: /* Network Send */
	/**
	 *
	 * @param guid 
	 */
	static void sendCreateAccountPacket(const NetGUID& guid);

	/**
	 *
	 * @param guid 
	 * @param email
	 * @param password 
	 */
	static void sendGenerateAccountPacket(const NetGUID& guid, const String& email,
										  const String& password);

public: /* Database */
	static void insertAccount(const String& email, const String& password);

	static void updateEmail(Account* account, const String& newEmail);

	static void updatePassword(Account* account, const String& newPassword);

	static void deleteAccount(Account* account);

	static bool isAccountExists(const String& email);

	static bool isAccountExists(const String& email, const String& password);

private: /* Other */
	static String encryptPassword(const String& password);
};

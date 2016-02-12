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
#include "AccountModule.h"
#include "MySQL.h"
#include "Server.h"
#include "sha1.h"
#include "CharacterModule.h"
#include "GuildModule.h"

void AccountModule::createAccount(Packet* packet, BitStream* bitStream)
{
	READ(bitStream, String, email);
	auto msg = Shared::Global::isValidEmail(email);
	if (msg == Shared::Message::SUCCESS && AccountModule::isAccountExists(email))
		msg = Shared::Message::EMAIL_EXISTS;

	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	}

	READ(bitStream, String, password);
	msg = Shared::Global::isValidPassword(password);
	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	}

	insertAccount(email, password);
	sendCreateAccountPacket(packet->guid);
}

void AccountModule::generateAccount(Packet* packet)
{
	String email = "guest";
	email += Utils::uintToStr(MySQL::getLastGuestID() + 1);

	const char alphaNum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	String password = "A0"; // This guarantee at least one upper-case letter and one digit.
	s32 max = sizeof(alphaNum) / sizeof(char) - 2;
	for (u32 i = 2u; i < MAX_LEN_PASSWORD; ++i)
		password += alphaNum[Jinra::Random::generateInt(0, max)];

	insertAccount(email, password);
	sendGenerateAccountPacket(packet->guid, email, password);
}

void AccountModule::login(Packet* packet, BitStream* bitStream)
{
	READ(bitStream, String, email);
	auto msg = Shared::Global::isValidEmail(email, false);
	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	}

	READ(bitStream, String, password);
	msg = Shared::Global::isValidPassword(password);
	if (msg != Shared::Message::SUCCESS) {
		sendMessagePacket(packet->guid, msg);
		return;
	}

	if (!isAccountExists(email, password)) {
		sendMessagePacket(packet->guid, Shared::Message::EMAIL_OR_PASSWORD_IS_INCORRECT);
		return;
	}

	auto client = server->getClient(email);
	if (client != nullptr) {
		sendMessagePacket(packet->guid, Shared::Message::ACCOUNT_IN_USE);
		return;
	}

	client = new Client(packet->guid, email);
	MySQL::loadAccountData(client);
	if (client->getType() == Account::Type::BANNED) {
		sendMessagePacket(packet->guid, Shared::Message::ACCOUNT_BANNED);
		delete client;
		return;
	}

	MySQL::loadAccountCharacter(client);
	server->addClient(client);

	if (client->getCharacter() != nullptr) {
		client->sendLoginPacket();
	} else {
		client->sendCreateCharacterPacket();
	}
}

void AccountModule::changeAccountData(Packet* packet, BitStream* bitStream)
{
	auto client = server->getClient(packet->guid);
	READ(bitStream, String, email);
	if (!email.empty() && email != client->getEmail()) {
		auto msg = Shared::Global::isValidEmail(email);
		if (msg == Shared::Message::SUCCESS && AccountModule::isAccountExists(email))
			msg = Shared::Message::EMAIL_EXISTS;

		if (msg != Shared::Message::SUCCESS) {
			sendMessagePacket(packet->guid, msg);
			return;
		}

		updateEmail(client, email);
		client->setEmail(email);
	}

	READ(bitStream, String, password);
	if (!password.empty()) {
		auto msg = Shared::Global::isValidPassword(password);
		if (msg != Shared::Message::SUCCESS) {
			sendMessagePacket(packet->guid, msg);
			return;
		}

		updatePassword(client, password);
	}

	client->sendChangeAccountDataPacket(email, password);
}

void AccountModule::removeAccount(Packet* packet)
{
	auto client = server->getClient(packet->guid);
	if (client == nullptr) {
		sendMessagePacket(packet->guid, Shared::Message::HACK);
		return;
	}

	if (client->isPlaying()) {
		auto character = client->getCharacter();

		if (character->isBelongsToGuild()) {
			// leaveGuild() is doing pretty much the same thing as we want, so we can 
			// simulate receiving packet by creating temporary bit stream.
			BitStream tempBitStream;
			tempBitStream.write(character->getName());
			GuildModule::leaveGuild(packet, &tempBitStream);
		}

		CharacterModule::deleteCharacter(character);
	}

	deleteAccount(client);

	client->sendDeleteAccountPacket();
	server->removeClient(packet->guid);
}

void AccountModule::sendCreateAccountPacket(const NetGUID& guid)
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_CREATE);
	server->send(&bitStream, Packet::MEDIUM_PRIORITY, guid);
}

void AccountModule::sendGenerateAccountPacket(const NetGUID& guid, const String& email,
											  const String& password)
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_GENERATE);
	bitStream.write(email);
	bitStream.write(password);
	server->send(&bitStream, Packet::MEDIUM_PRIORITY, guid);
}

void AccountModule::insertAccount(const String& email, const String& password)
{
	char values[256];
	sprintf(values, "'%s','%s'", email.c_str(), encryptPassword(password).c_str());
	MySQL::insert("accounts(email,password)", values);
}

void AccountModule::updateEmail(Account* account, const String& newEmail)
{
	char set[128];
	sprintf(set, "a.email='%s'", newEmail.c_str());

	char rest[128];
	sprintf(rest, "a.email='%s'", account->getEmail());

	MySQL::update("accounts a", set, rest);
}

void AccountModule::updatePassword(Account* account, const String& newPassword)
{
	char set[256];
	sprintf(set, "a.password='%s'", encryptPassword(newPassword).c_str());

	char rest[256];
	sprintf(rest, "a.email='%s'", account->getEmail());

	MySQL::update("accounts a", set, rest);
}

void AccountModule::deleteAccount(Account* account)
{
	char rest[128];
	sprintf(rest, "a.email='%s'", account->getEmail());
	MySQL::delete_("accounts a", rest);
}

bool AccountModule::isAccountExists(const String& email)
{
	char rest[128];
	sprintf(rest, "a.email='%s'", email.c_str());
	return MySQL::isExists("accounts a", rest);
}

bool AccountModule::isAccountExists(const String& email, const String& password)
{
	char rest[256];
	sprintf(rest, "a.email='%s' AND a.password='%s'", email.c_str(),
			encryptPassword(password).c_str());
	return MySQL::isExists("accounts a", rest);
}

String AccountModule::encryptPassword(const String& password)
{
	// Simple salt
	auto encryptedPassword = password;
	encryptedPassword += password[password.length() - 1];
	encryptedPassword += password[password.length() - 2];
	encryptedPassword += password[1];
	encryptedPassword += password[0];

	// SHA1 encryption
	u8 hash[20];
	char hexString[41];
	sha1::calc(encryptedPassword.c_str(), encryptedPassword.length(), hash);
	sha1::toHexString(hash, hexString);
	encryptedPassword = hexString;

	return encryptedPassword;
}
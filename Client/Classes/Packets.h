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

class Packets
{
public:
	static void connectionRequestAccepted();
	
	static void connectionAttemptFailed();

	static void alreadyConnected();

	static void disconnectionNotification();

	static void connectionLost();
	
	static void ipBanned();

	static void message(BitStream* bitStream);

	static void createAccount(BitStream* bitStream);

	static void generateAccount(BitStream* bitStream);

	static void changeAccountData(BitStream* bitStream);

	static void deleteAccount(BitStream* bitStream);

	static void createCharacter(BitStream* bitStream);

	static void characterLevelUp(BitStream* bitStream);

	static void login(BitStream* bitStream);

	static void chat(BitStream* bitStream);

	static void createGuild(BitStream* bitStream);

	static void setGuildDescription(BitStream* bitStream);

	static void setGuildRankName(BitStream* bitStream);

	static void inviteCharacterToGuild(BitStream* bitStream);
	
	static void guildInvitationAnswer(BitStream* bitStream);

	static void joinGuild(BitStream* bitStream);

	static void guildMemberBecomeOnline(BitStream* bitStream);

	static void guildMemberBecomeOffline(BitStream* bitStream);

	static void addGuildMember(BitStream* bitStream);

	static void setGuildNote(BitStream* bitStream);

	static void setGuildMemberRank(BitStream* bitStream);

	static void leaveGuild(BitStream* bitStream);

	static void dialogueInit(BitStream* bitStream);

	static void dialogueStart(BitStream* bitStream);

	static void dialogueUpdate(BitStream* bitStream);

	static void unknown();

public: // Global packets to send.
	static void sendLogin(const SString& login, const SString& password);

private:
	static void sendAcceptGuildInvitation(const SString& guildName, u32 inviteID);

	static void sendDeclineGuildInvitation(const SString& guildName, u32 inviteID);
};


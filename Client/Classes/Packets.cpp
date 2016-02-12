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
#include "Packets.h"
#include "MsgBox.h"
#include "Dialogue.h"
#include "LoadingScreen.h"
#include "AccountScreen.h"
#include "CharacterCreationScreen.h"
#include "PlayScreen.h"

void Packets::connectionRequestAccepted() {
	game->getCurrentScreen<LoadingScreen>()->loadAfterConnection();
}

void Packets::connectionAttemptFailed()
{
	auto loadingScreen = game->getCurrentScreen<LoadingScreen>();
	auto set = game->getLang()->getSetElement("LoadingScreen");

	loadingScreen->setInfo(LANG_TEXT(set, "CANNOT_CONNECT"));

	auto msgBox = MsgBox::create();
	auto tryAgainFunction = [=](Ref*) {
		msgBox->hide();
		loadingScreen->setInfo(LANG_TEXT(set, "RECONNECTING"));
		game->reconnect("static");
	};
	msgBox->init(LANG_TEXT(set, "MB_CANNOT_CONNECT"), 
				 LANG_TEXT(set, "MB_TRY_AGAIN"), tryAgainFunction,
				 "", nullptr, 
				 LANG_TEXT(set, "MB_EXIT"), [](Ref*) { game->exit(); });
	msgBox->show();
}

void Packets::alreadyConnected()
{
	auto set = game->getLang()->getSetElement("LoadingScreen");
	auto msgBox = MsgBox::create(LANG_TEXT(set, "MB_ALREADY_CONNECTED"),
								 "", nullptr,
								 LANG_TEXT(set, "MB_OK"), [](Ref*) { game->exit(); },
								 "", nullptr);
	msgBox->show();
}

void Packets::disconnectionNotification()
{
	auto set = game->getLang()->getSetElement("Global");
	auto msgBox = MsgBox::create(LANG_TEXT(set, "MB_DISCONNECTION"),
								 "", nullptr,
								 LANG_TEXT(set, "MB_OK"), [](Ref*) { game->exit(); },
								 "", nullptr);
	msgBox->show();
}

void Packets::connectionLost()
{
	auto set = game->getLang()->getSetElement("Global");
	auto msgBox = MsgBox::create(LANG_TEXT(set, "MB_LOST_CONNECTION"),
								 "", nullptr,
								 LANG_TEXT(set, "MB_OK"), [](Ref*) { game->exit(); },
								 "", nullptr);
	msgBox->show();
}

void Packets::ipBanned()
{
	auto set = game->getLang()->getSetElement("Global");
	auto msgBox = MsgBox::create(LANG_TEXT(set, "MB_IP_BANNED"),
								 "", nullptr,
								 LANG_TEXT(set, "MB_OK"), [](Ref*) { game->exit(); },
								 "", nullptr);
	msgBox->show();
}

void Packets::message(BitStream* bitStream)
{
	READ(bitStream, s32, msg);
	game->showMsgBoxWithValidationMsg(static_cast<Shared::Message>(msg));
}

void Packets::createAccount(BitStream* bitStream)
{
	auto panel = game->getCurrentScreen<AccountScreen>()->getChildByName(
		"Scene")->getChildByName("account_view")->getChildByName("register");

	auto email = GET_CHILD(panel, "txtEmail", TextField)->getString();
	auto password = GET_CHILD(panel, "txtPassword", TextField)->getString();

	game->saveLoginFile(email.c_str(), password.c_str());
	sendLogin(email, password);
}

void Packets::generateAccount(BitStream* bitStream)
{
	READ(bitStream, SString, email);
	READ(bitStream, SString, password);

	game->saveLoginFile(email.c_str(), password.c_str());
	sendLogin(email, password);
}

void Packets::changeAccountData(BitStream* bitStream)
{
	READ(bitStream, SString, email);
	READ(bitStream, SString, password);

	if (!email.empty())
		game->getOptionsWindow()->setEmail(email);
	
	SString oldEmail;
	SString oldPassword;
	game->loadLoginFile(oldEmail, oldPassword);
	game->saveLoginFile((email.empty()) ? oldEmail.c_str() : email.c_str(),
						(password.empty()) ? oldPassword.c_str() : password.c_str());
}

void Packets::deleteAccount(BitStream* bitStream)
{
	game->deleteLoginFile();

	auto set = game->getLang()->getSetElement("Global");
	auto msgBox = MsgBox::create();
	auto okFunction = [=](Ref*) {
		msgBox->hide();
		Director::getInstance()->replaceScene(AccountScreen::createScene());
	};
	msgBox->init(LANG_TEXT(set, "MB_ACCOUNT_DELETED"), 
				 "", nullptr,
				 LANG_TEXT(set, "MB_OK"), okFunction,
				 "", nullptr);
	msgBox->show();
}

void Packets::createCharacter(BitStream* bitStream)
{
	std::vector<Shared::Class> classes;
	READ(bitStream, u8, classesCount);
	for (u8 i = 0u; i < classesCount; ++i) {
		READ(bitStream, u32, id);
		READ(bitStream, SString, name);
		READ(bitStream, u8, health);
		READ(bitStream, u8, power);
		READ(bitStream, u8, physique);
		READ(bitStream, u8, dexterity);
		READ(bitStream, u8, intelligence);
		READ(bitStream, u8, healthGain);
		READ(bitStream, u8, powerGain);
		READ(bitStream, u8, physiqueGain);
		READ(bitStream, u8, dexterityGain);
		READ(bitStream, u8, intelligenceGain);

		classes.push_back(Shared::Class(id, name, health, power, physique, 
			dexterity, intelligence, healthGain, powerGain, physiqueGain, dexterityGain,
			intelligenceGain));
	}
	Director::getInstance()->replaceScene(CharacterCreationScreen::createScene(&classes));
}

void Packets::characterLevelUp(BitStream* bitStream)
{
	READ(bitStream, u16, level);
	READ(bitStream, u16, health);
	READ(bitStream, u16, power);
	READ(bitStream, u16, physique);
	READ(bitStream, u16, dexterity);
	READ(bitStream, u16, intelligence);

	auto character = game->getCharacter();
	character->setLevel(level);
	character->setHealth(health);
	character->setPower(power);
	character->setPhysique(physique);
	character->setDexterity(dexterity);
	character->setIntelligence(intelligence);

	PlayScreen::getStatisticsWindow()->update();
}

void Packets::login(BitStream* bitStream)
{
	READ(bitStream, u32, id);
	READ(bitStream, SString, name);
	READ(bitStream, u16, level);
	READ(bitStream, bool, male);
	READ(bitStream, u32, cash);
	READ(bitStream, u16, health);
	READ(bitStream, u16, power);
	READ(bitStream, u16, physique);
	READ(bitStream, u16, dexterity);
	READ(bitStream, u16, intelligence);

	Director::getInstance()->replaceScene(PlayScreen::createScene());
	game->setCharacter(new Character(id, name, level, male, cash, health, power, physique, 
		dexterity, intelligence));
}

void Packets::chat(BitStream* bitStream)
{
	//READ(bitStream, u8, channelID);
	READ(bitStream, SString, text);

	game->getCurrentScreen<PlayScreen>()->getChat()->addText(text.c_str());
}

void Packets::createGuild(BitStream* bitStream)
{
	READ(bitStream, SString, name);
	READ(bitStream, SString, tag);

	auto character = game->getCharacter();
	character->setGuild(new Shared::Guild(tag));

	// Updates guild tag for character name.
	PlayScreen::getStatisticsWindow()->update();

	PlayScreen::getGuildCreationWindow()->hide();
	PlayScreen::getGuildWindow()->setName(name);
	PlayScreen::getGuildWindow()->setDescription("");

	auto guild = game->getCharacter()->getGuild();
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i) {
		READ(bitStream, SString, name);
		PlayScreen::getGuildWindow()->setRankName(i, name);
		PlayScreen::getGuildSettingsWindow()->setRankName(i, name);
		guild->setRankName(i, name);
	}

	PlayScreen::getGuildWindow()->addMember(character->getName(), true, character->getLevel(), 
											Shared::GuildRanks::OWNER, "");
	PlayScreen::getGuildWindow()->show();
}

void Packets::setGuildDescription(BitStream* bitStream)
{
	READ(bitStream, SString, description);
	game->getCharacter()->getGuild()->setDescription(description);
	PlayScreen::getGuildWindow()->setDescription(description);
	PlayScreen::getGuildSettingsWindow()->setDescription(description);
}

void Packets::setGuildRankName(BitStream* bitStream)
{
	READ(bitStream, u8, rankIndex);
	READ(bitStream, SString, rankName);
	PlayScreen::getGuildWindow()->setRankName(rankIndex, rankName);
	PlayScreen::getGuildSettingsWindow()->setRankName(rankIndex, rankName);
	game->getCharacter()->getGuild()->setRankName(rankIndex, rankName);
}

void Packets::inviteCharacterToGuild(BitStream* bitStream)
{
	READ(bitStream, u32, inviteID);
	READ(bitStream, SString, guildName);

	auto set = game->getLang()->getSetElement("PlayScreen");
	auto msgBox = MsgBox::create();

	auto acceptFunc = [=](Ref*) { 
		sendAcceptGuildInvitation(guildName, inviteID);
		msgBox->hide(); 
	};

	auto declineFunc = [=](Ref*) { 
		sendDeclineGuildInvitation(guildName, inviteID);
		msgBox->hide(); 
	};

	msgBox->init(StringUtils::format(
		LANG_TEXT(set, "MB_GUILD_INVITE").c_str(), guildName.c_str()),
		LANG_TEXT(set, "MB_ACCEPT"), acceptFunc,
		"", nullptr,
		LANG_TEXT(set, "MB_DECLINE"), declineFunc);
	msgBox->show();
}

void Packets::guildInvitationAnswer(BitStream* bitStream)
{
	READ(bitStream, SString, characterName);
	auto set = game->getLang()->getSetElement("PlayScreen");
	auto msgBox = MsgBox::create();
	msgBox->init(StringUtils::format(
		LANG_TEXT(set, "MB_GUILD_INVITE_REFUSED").c_str(), characterName.c_str()),
				 "", nullptr,
				 LANG_TEXT(set, "MB_OK"), [=](Ref*) { msgBox->hide(); },
				 "", nullptr);
	msgBox->show();
}

void Packets::joinGuild(BitStream* bitStream)
{
	READ(bitStream, SString, guildName);
	READ(bitStream, SString, guildTag);
	READ(bitStream, SString, guildDescription);

	// Updates basic guild info.
	PlayScreen::getGuildWindow()->setName(guildName);
	PlayScreen::getGuildWindow()->setDescription(guildDescription);
	PlayScreen::getGuildSettingsWindow()->setDescription(guildDescription);

	// Updates ranks names in guild settings.
	auto guild = new Shared::Guild(guildTag, guildDescription);
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i) {
		READ(bitStream, SString, guildRank);
		PlayScreen::getGuildSettingsWindow()->setRankName(i, guildRank);
		guild->setRankName(i, guildRank);
	}

	// Updates members list.
	READ(bitStream, u16, membersCount);
	for (u32 i = 0u; i < membersCount; ++i) {
		READ(bitStream, SString, memberName);
		READ(bitStream, bool, memberOnline); 
		READ(bitStream, u16, memberLevel);
		READ(bitStream, u8, memberRank);
		READ(bitStream, SString, memberNote);
		PlayScreen::getGuildWindow()->addMember(memberName, memberOnline, memberLevel, 
												static_cast<Shared::GuildRanks>(memberRank), 
												memberNote);
	}

	game->getCharacter()->setGuild(guild);
}

void Packets::guildMemberBecomeOnline(BitStream* bitStream)
{
	READ(bitStream, SString, memberName);
	PlayScreen::getGuildWindow()->setOnlineOfflineMember(memberName, true);
}

void Packets::guildMemberBecomeOffline(BitStream* bitStream)
{
	READ(bitStream, SString, memberName);
	PlayScreen::getGuildWindow()->setOnlineOfflineMember(memberName, false);
}

void Packets::addGuildMember(BitStream* bitStream)
{
	READ(bitStream, SString, memberName);
	READ(bitStream, u16, memberLevel);
	PlayScreen::getGuildWindow()->addMember(memberName, true, memberLevel, 
											Shared::GuildRanks::RECRUIT, "");
}

void Packets::setGuildNote(BitStream* bitStream)
{
	READ(bitStream, SString, memberName);
	READ(bitStream, SString, memberNote);
	PlayScreen::getGuildWindow()->setMemberNote(memberName, memberNote);
}

void Packets::setGuildMemberRank(BitStream* bitStream)
{
	READ(bitStream, SString, memberName);
	READ(bitStream, u8, memberRank);
	PlayScreen::getGuildWindow()->setMemberRank(memberName, memberRank);
	PlayScreen::getGuildWindow()->update();
}

void Packets::leaveGuild(BitStream* bitStream)
{
	READ(bitStream, bool, disbandGuild);
	READ(bitStream, SString, memberName);
	
	auto character = game->getCharacter();
	if (disbandGuild || memberName == character->getName()) {
		character->setGuild(nullptr);
		PlayScreen::getStatisticsWindow()->update();
		PlayScreen::getGuildWindow()->clear();
		PlayScreen::getGuildWindow()->hide();
		return;
	}
	PlayScreen::getGuildWindow()->removeMember(memberName);
}

void Packets::dialogueInit(BitStream* bitStream)
{
	auto set = game->getLang()->getSetElement("PlayScreen");
	auto msgBox = MsgBox::create();
	msgBox->init(LANG_TEXT(set, "MB_DIALOGUE_INIT_FAILED"), 
				 "", nullptr,
				 LANG_TEXT(set, "MB_OK"), [=](Ref*) { msgBox->hide(); },
				 "", nullptr);
	msgBox->show();
}

void Packets::dialogueStart(BitStream* bitStream)
{
	READ(bitStream, u32, dialogueID);
	READ(bitStream, SString, initialTextID);
	READ(bitStream, u32, optionsCount);

	std::vector<SString> optionsIDs;
	if (optionsCount > 0) {
		optionsIDs.resize(optionsCount);
		for (auto& option : optionsIDs)
			bitStream->read(option);
	}

	Dialogue::show(dialogueID);
	Dialogue::update(initialTextID, optionsIDs);
}

void Packets::dialogueUpdate(BitStream* bitStream)
{
	READ(bitStream, SString, textID);
	READ(bitStream, u32, optionsCount);

	std::vector<SString> optionsIDs;
	if (optionsCount > 0) {
		optionsIDs.resize(optionsCount);
		for (auto& option : optionsIDs)
			bitStream->read(option);
	}

	(textID == "[END]") ? Dialogue::hide() : Dialogue::update(textID, optionsIDs);
}

void Packets::unknown()
{
	auto set = game->getLang()->getSetElement("Global");
	auto msgBox = MsgBox::create(LANG_TEXT(set, "MB_UNKNOWN_PACKET"),
								 "", nullptr,
								 LANG_TEXT(set, "MB_OK"), [](Ref*) { game->exit(); },
								 "", nullptr);
	msgBox->show();
}

void Packets::sendLogin(const SString& email, const SString& password)
{
	BITSTREAM(bitStream, Shared::PacketID::LOGIN);
	bitStream.write(email);
	bitStream.write(password);
	game->send(&bitStream);
}

void Packets::sendAcceptGuildInvitation(const SString& guildName, u32 inviteID)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_INVITATION_ANSWER);
	bitStream.write(guildName);
	bitStream.write(inviteID);
	bitStream.write(true);
	game->send(&bitStream);
}

void Packets::sendDeclineGuildInvitation(const SString& guildName, u32 inviteID)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_INVITATION_ANSWER);
	bitStream.write(guildName);
	bitStream.write(inviteID);
	bitStream.write(false);
	game->send(&bitStream);
}


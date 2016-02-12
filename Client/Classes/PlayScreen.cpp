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
#include "PlayScreen.h"

Statistics PlayScreen::_statistics;
Guild PlayScreen::_guild;
GuildCreation PlayScreen::_guildCreation;
GuildSettings PlayScreen::_guildSettings;
GuildAddMember PlayScreen::_guildAddMember;

Scene* PlayScreen::createScene()
{
	auto scene = Scene::create();
	auto layer = PlayScreen::create();
	scene->addChild(layer);
	return scene;
}

void PlayScreen::setLanguage() 
{ 
	_statistics.setLanguage();
	_guild.setLanguage();
	_guildCreation.setLanguage();
	_guildSettings.setLanguage();
	_guildAddMember.setLanguage();
}

bool PlayScreen::init()
{
	if (!Layer::init()) {
		game->exit();
		return false;
	}

	auto scene = Loader::loadScene("Screens/Play/scene.csb");
	addChild(scene);
	setScene(scene);

	setLanguage();

	initChat();

	addClickEventToOptionsButton();
	addClickEventToStatisticsButton();
	addClickEventToGuildButton();
	addClickEventToDialogueButton();

	scheduleUpdate();
	return true;
}

void PlayScreen::initChat()
{
	auto set = game->getLang()->getSetElement("Chat");
	std::vector<Chat::Channel> channels{
		Chat::Channel(Shared::ChatChannelID::GLOBAL, 
					  LANG_TEXT(set, "CHANNEL_GLOBAL"), nullptr),
		Chat::Channel(Shared::ChatChannelID::GUILD,
					  LANG_TEXT(set, "CHANNEL_GUILD"), nullptr),
		Chat::Channel(Shared::ChatChannelID::SYSTEM,
					  LANG_TEXT(set, "CHANNEL_SYSTEM"), nullptr)
	};
	_chat.create(channels, getScene());
}

void PlayScreen::addClickEventToOptionsButton()
{
	auto scene = getScene();
	GET_CHILD(scene, "btnOptions", Button)->addClickEventListener([=](Ref* sender) {
		game->getOptionsWindow()->show(scene);
	});
}

void PlayScreen::addClickEventToStatisticsButton()
{
	auto scene = getScene();
	GET_CHILD(scene, "btnStatistics", Button)->addClickEventListener([=](Ref* sender) {
		PlayScreen::getStatisticsWindow()->show(scene);
	});
}

void PlayScreen::addClickEventToGuildButton()
{
	auto scene = getScene();
	GET_CHILD(scene, "btnGuild", Button)->addClickEventListener([=](Ref* sender) {
		if (game->getCharacter()->getGuild() != nullptr) {
			PlayScreen::getGuildWindow()->show(scene);
		} else {
			PlayScreen::getGuildCreationWindow()->show(scene);
		}
	});
}

void PlayScreen::addClickEventToDialogueButton()
{
	GET_CHILD(getScene(), "btnDialogue", Button)->addClickEventListener([=](Ref* sender) {
		initDialogue();
	});
}

void PlayScreen::onEnter()
{
	Layer::onEnter();
	PlayScreen::getStatisticsWindow()->update();
}

void PlayScreen::update(float delta)
{
	Node::update(delta);
	game->receive();
}

void PlayScreen::initDialogue()
{
	BITSTREAM(bitStream, Shared::PacketID::DIALOGUE_INIT);
	bitStream.write(0); // ID of dialogue to init and possibly start.
	game->send(&bitStream);
}


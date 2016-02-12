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
#include "LoadingScreen.h"
#include "AccountScreen.h"
#include "MsgBox.h"
#include "Chat.h"
#include "PlayScreen.h"
#include "Guild.h"
#include "Dialogue.h"
#include "Packets.h"

Scene* LoadingScreen::createScene()
{
    auto scene = Scene::create();
	auto layer = LoadingScreen::create();
	scene->addChild(layer);
    return scene;
}

void LoadingScreen::setLanguage() { }

void LoadingScreen::loadAfterConnection() {
	loadOptions();
}

bool LoadingScreen::init()
{
	if (!Layer::init()) {
		game->exit();
		return false;
	}

	auto scene = Loader::loadScene("Screens/Loading/scene.csb");
	addChild(scene);
	setScene(scene);

	// Initialize peer. 
	auto startupResult = game->initialize();
	if (startupResult != Jinra::NetPeer::StartupResult::STARTED) {
		game->reactToSocketStartupResult(startupResult);
		return false;
	}

	scheduleUpdate();
	return true;
}

void LoadingScreen::onEnter()
{
	Node::onEnter();
	startLoading();
}

void LoadingScreen::update(float delta)
{
	Node::update(delta);
	game->receive();
}

void LoadingScreen::startLoading()
{
	// Message box is first due to possible communications about errors.
	loadMessageBox();
}

void LoadingScreen::loadMessageBox()
{
	setInfo(LANG_TEXT("LoadingScreen", "LOAD_MSG_BOX"));
	MsgBox::load(Loader::loadScene("Windows/MessageBox/scene.csb"));

	// Msg box has been loaded and now we can connect to server. 
	// There's no reason to load other stuff before connection, due to possible fail.
	connectToServer();
}

void LoadingScreen::connectToServer()
{
	setInfo(LANG_TEXT("LoadingScreen", "CONNECTING"));
	game->connect(CONNECTION_IP, CONNECTION_PORT, "static");

	// After successful connection it goes to loadAfterConnection()
}

void LoadingScreen::loadOptions()
{
	setInfo(LANG_TEXT("LoadingScreen", "LOAD_OPTIONS"));
	game->getOptionsWindow()->load(Loader::loadScene("Windows/Options/scene.csb"), "Options");

	loadChat();
}

void LoadingScreen::loadChat()
{
	setInfo(LANG_TEXT("LoadingScreen", "LOAD_CHAT"));
	Chat::load(Loader::loadScene("Windows/Chat/scene.csb"));

	loadStatistics();
}

void LoadingScreen::loadStatistics()
{
	setInfo(LANG_TEXT("LoadingScreen", "LOAD_STATISTICS"));
	PlayScreen::getStatisticsWindow()->load(Loader::loadScene("Windows/Stats/scene.csb"),
											"Stats");

	loadGuild();
}

void LoadingScreen::loadGuild()
{
	setInfo(LANG_TEXT("LoadingScreen", "LOAD_GUILD"));
	auto scene = Loader::loadScene("Windows/Guild/scene.csb");
	PlayScreen::getGuildWindow()->load(scene, "Guild");
	PlayScreen::getGuildSettingsWindow()->load(scene, "GuildSettings");
	PlayScreen::getGuildCreationWindow()->load(scene, "GuildCreation");
	PlayScreen::getGuildAddMemberWindow()->load(scene, "GuildAddMember");

	loadDialogue();
}

void LoadingScreen::loadDialogue()
{
	setInfo(LANG_TEXT("LoadingScreen", "LOAD_DIALOGUE"));
	auto scene = Loader::loadScene("Windows/Dialogue/scene.csb");
	Dialogue::load(scene);

	finishLoading();
}

void LoadingScreen::finishLoading()
{
	setInfo(LANG_TEXT("LoadingScreen", "FINISH"));
	if (game->isLoginFileExist()) {
		SString email;
		SString password;
		game->loadLoginFile(email, password);
		Packets::sendLogin(email, password);
	} else {
		Director::getInstance()->replaceScene(AccountScreen::createScene());
	}
}

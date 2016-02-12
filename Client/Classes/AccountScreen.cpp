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
#include "AccountScreen.h"
#include "Packets.h"
#include "MsgBox.h"

Scene* AccountScreen::createScene()
{
	auto scene = Scene::create();
	auto layer = AccountScreen::create();
	scene->addChild(layer);
	return scene;
}

void AccountScreen::setLanguage()
{
	auto set = game->getLang()->getSetElement("AccountScreen");
	auto view = getScene()->getChildByName("account_view");

	// === LOGIN PANEL ===
	auto panel = view->getChildByName("login");
	GET_CHILD(panel, "lblEmail", Text)->setString(LANG_TEXT(set, "EMAIL"));
	GET_CHILD(panel, "txtEmail", TextField)->setPlaceHolder(
		LANG_TEXT(set, "EMAIL_PLACEHOLDER"));
	GET_CHILD(panel, "lblPassword", Text)->setString(LANG_TEXT(set, "PASSWORD"));
	GET_CHILD(panel, "txtPassword", TextField)->setPlaceHolder(
		LANG_TEXT(set, "PASSWORD_PLACEHOLDER"));
	GET_CHILD(panel, "btnLogin", Button)->setTitleText(LANG_TEXT(set, "LOGIN"));
	GET_CHILD(panel, "lblSlideRegister", Text)->setString(LANG_TEXT(set, "SLIDE_REGISTER"));

	// === REGISTER PANEL ===
	panel = view->getChildByName("register");
	GET_CHILD(panel, "lblEmail", Text)->setString(LANG_TEXT(set, "EMAIL"));
	GET_CHILD(panel, "txtEmail", TextField)->setPlaceHolder(
		LANG_TEXT(set, "EMAIL_PLACEHOLDER"));
	GET_CHILD(panel, "lblReEmail", Text)->setString(LANG_TEXT(set, "REEMAIL"));
	GET_CHILD(panel, "txtReEmail", TextField)->setPlaceHolder(
		LANG_TEXT(set, "REEMAIL_PLACEHOLDER"));
	GET_CHILD(panel, "lblPassword", Text)->setString(LANG_TEXT(set, "PASSWORD"));
	GET_CHILD(panel, "txtPassword", TextField)->setPlaceHolder(
		LANG_TEXT(set, "PASSWORD_PLACEHOLDER"));
	GET_CHILD(panel, "lblRePassword", Text)->setString(LANG_TEXT(set, "REPASSWORD"));
	GET_CHILD(panel, "txtRePassword", TextField)->setPlaceHolder(
		LANG_TEXT(set, "REPASSWORD_PLACEHOLDER"));
	GET_CHILD(panel, "btnCreate", Button)->setTitleText(LANG_TEXT(set, "CREATE"));
	GET_CHILD(panel, "btnClear", Button)->setTitleText(LANG_TEXT(set, "CLEAR"));
	GET_CHILD(panel, "lblSlideLogin", Text)->setString(LANG_TEXT(set, "SLIDE_LOGIN"));
	GET_CHILD(panel, "lblSlideGenerate", Text)->setString(LANG_TEXT(set, "SLIDE_GENERATE"));

	// === GENERATE PANEL ===
	panel = view->getChildByName("generate");
	GET_CHILD(panel, "lblGenerate", Text)->setString(LANG_TEXT(set, "GENERATE"));
	GET_CHILD(panel, "lblSlideRegister", Text)->setString(LANG_TEXT(set, "SLIDE_REGISTER"));
}


bool AccountScreen::init()
{
	if (!Layer::init()) {
		game->exit();
		return false;
	}

	auto scene = Loader::loadScene("Screens/Account/scene.csb");
	addChild(scene);
	setScene(scene);

	setLanguage();

	addClickEventToOptionsButton();
	addClickEventToLoginButton();
	addClickEventToCreateAccountButton();
	addClickEventToClearFieldsButton();
	addClickEventToGenerateButton();

	// Play background music.
	static_cast<ComAudio*>(scene->getChildByName("audio")->getComponent(
		"audio"))->playBackgroundMusic();

	// Account screen is only initialized when login file doesn't exist. 
	// So player is not logged in and we need to hide account options.
	game->getOptionsWindow()->setEmail("");

	scheduleUpdate(); 
	return true;
}

void AccountScreen::addClickEventToOptionsButton()
{
	auto scene = getScene();
	GET_CHILD(scene, "btnOptions", Button)->addClickEventListener([=](Ref* sender) {
		game->getOptionsWindow()->show(scene);
	});
}

void AccountScreen::addClickEventToLoginButton()
{
	auto panel = getScene()->getChildByName("account_view")->getChildByName("login");
	GET_CHILD(panel, "btnLogin", Button)->addClickEventListener([=](Ref* sender) {
		login(GET_CHILD(panel, "txtEmail", TextField)->getString(),
			  GET_CHILD(panel, "txtPassword", TextField)->getString());
	});
}

void AccountScreen::addClickEventToCreateAccountButton()
{
	auto panel = getScene()->getChildByName("account_view")->getChildByName("register");
	GET_CHILD(panel, "btnCreate", Button)->addClickEventListener([=](Ref* sender) {
		createAccount(GET_CHILD(panel, "txtEmail", TextField)->getString(), 
					  GET_CHILD(panel, "txtReEmail", TextField)->getString(),
					  GET_CHILD(panel, "txtPassword", TextField)->getString(),
					  GET_CHILD(panel, "txtRePassword", TextField)->getString());
	});
}

void AccountScreen::addClickEventToClearFieldsButton()
{
	auto panel = getScene()->getChildByName("account_view")->getChildByName("register");
	GET_CHILD(panel, "btnClear", Button)->addClickEventListener([=](Ref* sender) {
		GET_CHILD(panel, "txtEmail", TextField)->setString("");
		GET_CHILD(panel, "txtReEmail", TextField)->setString("");
		GET_CHILD(panel, "txtPassword", TextField)->setString("");
		GET_CHILD(panel, "txtRePassword", TextField)->setString("");
	});
}

void AccountScreen::addClickEventToGenerateButton()
{
	GET_CHILD(getScene()->getChildByName("account_view")->getChildByName("generate"),
			  "btnGenerate", Button)->addClickEventListener([](Ref* sender) {
		BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_GENERATE);
		game->send(&bitStream);
	});
}

void AccountScreen::update(float delta)
{
	Node::update(delta);
	game->receive();
}

void AccountScreen::login(const SString& email, const SString& password)
{
	if (!validateEmailAndPassword(email, password))
		return;

	game->saveLoginFile(email.c_str(), password.c_str());
	Packets::sendLogin(email, password);
}

void AccountScreen::createAccount(const SString& email, const SString& reemail, 
								  const SString& password, const SString& repassword)
{
	if (!validateEmailAndPassword(email, password))				return;
	if (!validateEmailWithReEmail(email, reemail))				return;
	if (!validatePasswordWithRePassword(password, repassword))	return;
	sendCreateAccount(email, password);
}

bool AccountScreen::validateEmailAndPassword(const SString& email, const SString& password)
{
	auto msg = Shared::Global::isValidEmail(email, false);
	if (msg != Shared::Message::SUCCESS) {
		game->showMsgBoxWithValidationMsg(msg);
		return false;
	}

	msg = Shared::Global::isValidPassword(password);
	if (msg != Shared::Message::SUCCESS) {
		game->showMsgBoxWithValidationMsg(msg);
		return false;
	}

	return true;
}

bool AccountScreen::validateEmailWithReEmail(const SString& email, const SString& reemail)
{
	if (email == reemail)
		return true;

	auto set = game->getLang()->getSetElement("ValidationMsg");
	auto msgBox = MsgBox::create();
	msgBox->init(LANG_TEXT(set, "MB_REEMAILL_DIDNT_MATCH"),
				 "", nullptr,
				 LANG_TEXT(set, "MB_OK"), [msgBox](Ref*) { msgBox->hide(); },
				 "", nullptr);
	msgBox->show();
	return false;
}

bool AccountScreen::validatePasswordWithRePassword(const SString& password, 
												   const SString& repassword)
{
	if (password == repassword)
		return true;

	auto set = game->getLang()->getSetElement("ValidationMsg");
	auto msgBox = MsgBox::create();
	msgBox->init(LANG_TEXT(set, "MB_REPASSWORD_DIDNT_MATCH"),
				 "", nullptr,
				 LANG_TEXT(set, "MB_OK"), [msgBox](Ref*) { msgBox->hide(); },
				 "", nullptr);
	msgBox->show();
	return false;
}

void AccountScreen::sendCreateAccount(const SString& email, const SString& password)
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_CREATE);
	bitStream.write(email);
	bitStream.write(password);
	game->send(&bitStream);
}
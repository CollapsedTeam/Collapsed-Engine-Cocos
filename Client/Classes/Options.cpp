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

void Options::setLanguage()
{
	auto set = game->getLang()->getSetElement("Options");

	// === ACCOUNT ===
	GET_CHILD(_main, "lblAccount", Text)->setString(LANG_TEXT(set, "ACCOUNT"));
	GET_CHILD(_main, "lblEmail", Text)->setString(LANG_TEXT(set, "EMAIL"));
	GET_CHILD(_main, "txtEmail", TextField)->setPlaceHolder(LANG_TEXT(
		set, "EMAIL_PLACEHOLDER"));
	GET_CHILD(_main, "txtReEmail", TextField)->setPlaceHolder(LANG_TEXT(
		set, "REEMAIL_PLACEHOLDER"));
	GET_CHILD(_main, "lblPassword", Text)->setString(LANG_TEXT(set, "PASSWORD"));
	GET_CHILD(_main, "txtPassword", TextField)->setPlaceHolder(LANG_TEXT(
		set, "PASSWORD_PLACEHOLDER"));
	GET_CHILD(_main, "txtRePassword", TextField)->setPlaceHolder(LANG_TEXT(
		set, "REPASSWORD_PLACEHOLDER"));
	GET_CHILD(_main, "btnDelete", Button)->setTitleText(LANG_TEXT(set, "DELETE"));

	// === DISPLAY ===
	GET_CHILD(_main, "lblDisplay", Text)->setString(LANG_TEXT(set, "DISPLAY"));
	GET_CHILD(_main, "lblResolution", Text)->setString(LANG_TEXT(set, "RESOLUTION"));
	GET_CHILD(_main, "lblLanguage", Text)->setString(LANG_TEXT(set, "LANGUAGE"));

	// === SOUND ===
	GET_CHILD(_main, "lblSound", Text)->setString(LANG_TEXT(set, "SOUND"));
	GET_CHILD(_main, "lblMusic", Text)->setString(LANG_TEXT(set, "MUSIC"));
	GET_CHILD(_main, "lblSFX", Text)->setString(LANG_TEXT(set, "SFX"));

	GET_CHILD(_main, "btnApply", Button)->setTitleText(LANG_TEXT(set, "APPLY"));
	GET_CHILD(_main, "btnCancel", Button)->setTitleText(LANG_TEXT(set, "CANCEL"));
}

void Options::setEmail(const SString& email)
{
	// Player need to be logged to change account data. This should be pretty obvious.
	bool playerIsLogged = !(email.empty());

	auto tbEmail = GET_CHILD(_main, "txtEmail", TextField);
	tbEmail->setVisible(playerIsLogged);
	tbEmail->setString(email);

	_main->getChildByName("lblEmail")->setVisible(playerIsLogged);
	_main->getChildByName("txtReEmail")->setVisible(playerIsLogged);
	_main->getChildByName("txtEmailBg")->setVisible(playerIsLogged);
	_main->getChildByName("txtReEmailBg")->setVisible(playerIsLogged);
	_main->getChildByName("lblPassword")->setVisible(playerIsLogged);
	_main->getChildByName("txtPassword")->setVisible(playerIsLogged);
	_main->getChildByName("txtRePassword")->setVisible(playerIsLogged);
	_main->getChildByName("txtPasswordBg")->setVisible(playerIsLogged);
	_main->getChildByName("txtRePasswordBg")->setVisible(playerIsLogged);
	_main->getChildByName("btnDelete")->setVisible(playerIsLogged);
}

void Options::onLoad(Node* scene)
{
	DragWindow::onLoad(scene);

	reset();

	addClickEventToPreviousResolutionButton();
	addClickEventToNextResolutionButton();
	addClickEventToPreviousLanguageButton();
	addClickEventToNextLanguageButton();
	addEventToMusicSlider();
	addEventToSFXSlider();
	addClickEventToDeleteAccountButton();
	addClickEventToApplyChangesButton();
	addClickEventToCancelChangesButton();
}

void Options::reset()
{
	// Only desktop platforms can change resolution.
#if DESKTOP_PLATFORM
	loadResolutions();
#else // DESKTOP_PLATFORM
	_main->getChildByName("btnPrevRes")->setVisible(false);
	_main->getChildByName("btnNextRes")->setVisible(false);
#endif // DESKTOP_PLATFORM

	loadLanguages();

	GET_CHILD(_main, "sldMusic", Slider)->setPercent(game->getMusicVolume());
	GET_CHILD(_main, "sldSFX", Slider)->setPercent(game->getSFXVolume());

	updateMusic();
	updateSFX();
}

void Options::loadResolutions()
{
	// No specified resolutions on the list?
	if (_resolutions.empty()) {
		// Adds default one.
		_resolutions.push_back(Size(1280.0f, 720.0f));
	} else {
		// Search for resolution on the list to set iterator on it.
		for (_resolutionIt = _resolutions.begin(); _resolutionIt != _resolutions.end(); 
			 ++_resolutionIt) {
			if (_resolutionIt->equals(game->getResolution()))
				break;
		}

		// Resolution not found on the list?
		if (_resolutionIt == _resolutions.end()) {
			game->exit();
			return;
		}
	}
	updateResolution();
}

void Options::updateResolution()
{
	auto btnPrevRes = GET_CHILD(_main, "btnPrevRes", Button);
	auto btnNextRes = GET_CHILD(_main, "btnNextRes", Button);

	// Only desktop platforms can change resolution.
#if DESKTOP_PLATFORM
	btnPrevRes->setVisible(!(_resolutionIt == _resolutions.begin()));
	btnNextRes->setVisible(!(_resolutionIt + 1 == _resolutions.end()));
#endif // DESKTOP_PLATFORM

	GET_CHILD(_main, "lblCurResolution", Text)->setString(
		StringUtils::format("%.0fx%.0f", _resolutionIt->width, _resolutionIt->height));
}

void Options::loadLanguages()
{
	// No specified languages on the list?
	if (_languages.empty()) {
		// Adds default one.
		_languages.push_back(LangNameAndLocale("English", "en_EN"));
	} else {
		auto currentLocale = game->getLang()->getLocale();

		// Search for language on the list to set iterator on it.
		for (_languageIt = _languages.begin(); _languageIt != _languages.end(); 
			 ++_languageIt) {
			if (_languageIt->second == currentLocale)
				break;
		}

		// Language not found on the list?
		if (_languageIt == _languages.end()) {
			game->exit();
			return;
		}
	}
	updateLanguage();
}

void Options::updateLanguage()
{
	GET_CHILD(_main, "btnPrevLang", Button)->setVisible(!(_languageIt == _languages.begin()));
	GET_CHILD(_main, "btnNextLang", Button)->setVisible(
		!(_languageIt + 1 == _languages.end()));
	GET_CHILD(_main, "lblCurLanguage", Text)->setString(_languageIt->first);
}

void Options::updateMusic()
{
	GET_CHILD(_main, "lblMusicVolume", Text)->setString(
		StringUtils::format("%d%%", GET_CHILD(_main, "sldMusic", Slider)->getPercent()));
}

void Options::updateSFX()
{
	GET_CHILD(_main, "lblSFXVolume", Text)->setString(
		StringUtils::format("%d%%", GET_CHILD(_main, "sldSFX", Slider)->getPercent()));
}

void Options::addClickEventToPreviousResolutionButton()
{
	GET_CHILD(_main, "btnPrevRes", Button)->addClickEventListener([=](Ref* sender) {
		--_resolutionIt;
		updateResolution();
	});
}

void Options::addClickEventToNextResolutionButton()
{
	GET_CHILD(_main, "btnNextRes", Button)->addClickEventListener([=](Ref* sender) {
		++_resolutionIt;
		updateResolution();
	});
}

void Options::addClickEventToPreviousLanguageButton()
{
	GET_CHILD(_main, "btnPrevLang", Button)->addClickEventListener([=](Ref* sender) {
		--_languageIt;
		updateLanguage();
	});
}

void Options::addClickEventToNextLanguageButton()
{
	GET_CHILD(_main, "btnNextLang", Button)->addClickEventListener([=](Ref* sender) {
		++_languageIt;
		updateLanguage();
	});
}

void Options::addEventToMusicSlider()
{
	GET_CHILD(_main, "sldMusic", Slider)->addEventListener([=](Ref*, Slider::EventType) {
		updateMusic();
	});
}

void Options::addEventToSFXSlider()
{
	GET_CHILD(_main, "sldSFX", Slider)->addEventListener([=](Ref*, Slider::EventType) {
		updateSFX();
	});
}

void Options::addClickEventToDeleteAccountButton()
{
	GET_CHILD(_main, "btnDelete", Button)->addClickEventListener([=](Ref* sender) {
		auto set = game->getLang()->getSetElement("Options");

		auto msgBox = MsgBox::create();
		auto deleteFunction = [=](Ref*) {
			sendDeleteAccountPacket();
			msgBox->hide();
		};
		msgBox->init(LANG_TEXT(set, "MB_DELETE_ACCOUNT"),
					 LANG_TEXT(set, "MB_YES"), deleteFunction,
					 "", nullptr,
					 LANG_TEXT(set, "MB_NO"), [msgBox](Ref*) { msgBox->hide(); });
		msgBox->show();
	});
}

void Options::addClickEventToApplyChangesButton()
{
	GET_CHILD(_main, "btnApply", Button)->addClickEventListener([=](Ref* sender) {
		game->setResolution(*_resolutionIt);
		game->setLanguage(_languageIt->second);
		game->setMusicVolume(GET_CHILD(_main, "sldMusic", Slider)->getPercent());
		game->setSFXVolume(GET_CHILD(_main, "sldSFX", Slider)->getPercent());
		game->saveConfigFile();
		confirmAccountData();
		hide();
	});
}

void Options::addClickEventToCancelChangesButton()
{
	GET_CHILD(_main, "btnCancel", Button)->addClickEventListener([=](Ref* sender) {
		reset();
		hide();
	});
}

void Options::confirmAccountData()
{
	auto email = GET_CHILD(_main, "txtEmail", TextField)->getString();
	auto reemail = GET_CHILD(_main, "txtReEmail", TextField)->getString();
	auto password = GET_CHILD(_main, "txtPassword", TextField)->getString();
	auto repassword = GET_CHILD(_main, "txtRePassword", TextField)->getString();

	// If email is empty then player is not logged.
	if (email.empty())
		return;

	if (!reemail.empty()) {
		auto msg = Shared::Global::isValidEmail(email);
		if (msg != Shared::Message::SUCCESS) {
			game->showMsgBoxWithValidationMsg(msg);
			return;
		}

		if (email != reemail) {
			auto set = game->getLang()->getSetElement("ValidationMsg");
			auto msgBox = MsgBox::create();
			msgBox->init(LANG_TEXT(set, "MB_REEMAILL_DIDNT_MATCH"),
						 "", nullptr,
						 LANG_TEXT(set, "MB_OK"), [msgBox](Ref*) { msgBox->hide(); },
						 "", nullptr);
			msgBox->show();
			return;
		}
	}

	if (!password.empty() && !repassword.empty()) {
		auto msg = Shared::Global::isValidPassword(password);
		if (msg != Shared::Message::SUCCESS) {
			game->showMsgBoxWithValidationMsg(msg);
			return;
		}

		if (password != repassword) {
			auto set = game->getLang()->getSetElement("ValidationMsg");
			auto msgBox = MsgBox::create();
			msgBox->init(LANG_TEXT(set, "MB_REPASSWORD_DIDNT_MATCH"),
						 "", nullptr,
						 LANG_TEXT(set, "MB_OK"), [msgBox](Ref*) { msgBox->hide(); },
						 "", nullptr);
			msgBox->show();
			return;
		}
	}

	sendChangeAccountDataPacket(email, password);
}

void Options::sendChangeAccountDataPacket(const SString& email, const SString& password)
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_CHANGE_DATA);
	bitStream.write(email);
	bitStream.write(password);
	game->send(&bitStream);
}

void Options::sendDeleteAccountPacket()
{
	BITSTREAM(bitStream, Shared::PacketID::ACCOUNT_DELETE);
	game->send(&bitStream);
}

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
#include "CharacterCreationScreen.h"
#include "MsgBox.h"

Scene* CharacterCreationScreen::createScene(std::vector<Shared::Class>* classes)
{
	auto scene = Scene::create();
	auto layer = CharacterCreationScreen::create();
	std::copy(classes->begin(), classes->end(), std::back_inserter(layer->_classes));
	scene->addChild(layer);
	return scene;
}

void CharacterCreationScreen::setLanguage()
{
	auto scene = getScene();
	auto set = game->getLang()->getSetElement("CharacterCreationScreen");
	GET_CHILD(scene, "lblText", Text)->setString(LANG_TEXT(set, "TEXT"));
	GET_CHILD(scene, "btnCreate", Button)->setTitleText(LANG_TEXT(set, "CREATE"));

	updateClassInfo();
}

bool CharacterCreationScreen::init()
{
	if (!Layer::init()) {
		game->exit();
		return false;
	}

	auto scene = Loader::loadScene("Screens/CharacterCreation/scene.csb");
	addChild(scene);
	setScene(scene);

	setLanguage();

	addClickEventToOptionsButton();
	addClickEventToMaleCheckBox();
	addClickEventToFemaleCheckBox();
	addClickEventToPreviousClassButton();
	addClickEventToNextClassButton();
	addClickEventToCreateButton();

	scheduleUpdate();
	return true;
}

void CharacterCreationScreen::onEnter()
{
	Node::onEnter();

	// Set current class iterator to the first one on the list.
	_currentClass = _classes.begin();

	updateClassInfo();
}

void CharacterCreationScreen::addClickEventToOptionsButton()
{
	auto scene = getScene();
	GET_CHILD(scene, "btnOptions", Button)->addClickEventListener([=](Ref* sender) {
		game->getOptionsWindow()->show(scene);
	});
}

void CharacterCreationScreen::addClickEventToMaleCheckBox()
{
	auto scene = getScene();
	auto checkBox = GET_CHILD(scene, "chkMale", CheckBox);
	checkBox->addEventListener([=](Ref* sender, CheckBox::EventType eventType) {
		if (eventType == CheckBox::EventType::UNSELECTED)
			checkBox->setSelected(true);

		GET_CHILD(scene, "chkFemale", CheckBox)->setSelected(false);
	});
}

void CharacterCreationScreen::addClickEventToFemaleCheckBox()
{
	auto scene = getScene();
	auto checkBox = GET_CHILD(scene, "chkFemale", CheckBox);
	checkBox->addEventListener([=](Ref* sender, CheckBox::EventType eventType) {
		if (eventType == CheckBox::EventType::UNSELECTED)
			checkBox->setSelected(true);

		GET_CHILD(scene, "chkMale", CheckBox)->setSelected(false);
	});
}

void CharacterCreationScreen::addClickEventToCreateButton()
{
	auto scene = getScene();
	GET_CHILD(scene, "btnCreate", Button)->addClickEventListener([=](Ref* sender) {
		createCharacter(GET_CHILD(scene, "txtName", TextField)->getString(),
						GET_CHILD(scene, "chkMale", CheckBox)->isSelected());
	});
}

void CharacterCreationScreen::addClickEventToPreviousClassButton()
{
	GET_CHILD(getScene(), "btnPreviousClass", Button)->addClickEventListener([=](Ref* sender) {
		--_currentClass;
		updateClassInfo();
	});
}

void CharacterCreationScreen::addClickEventToNextClassButton()
{
	GET_CHILD(getScene(), "btnNextClass", Button)->addClickEventListener([=](Ref* sender) {
		++_currentClass;
		updateClassInfo();
	});
}

void CharacterCreationScreen::update(float delta)
{
	Node::update(delta);
	game->receive();
}

void CharacterCreationScreen::updateClassInfo()
{
	if (_classes.size() < 1)
		return;

	auto scene = getScene();
	auto previousClass = GET_CHILD(scene, "btnPreviousClass", Button);
	auto nextClass = GET_CHILD(scene, "btnNextClass", Button);

	bool noPreviousClass = (_currentClass == _classes.begin());
	previousClass->setEnabled(!noPreviousClass);
	previousClass->setBright(!noPreviousClass);

	bool noNextClass = (_currentClass == _classes.end() - 1);
	nextClass->setEnabled(!noNextClass);
	nextClass->setBright(!noNextClass);

	GET_CHILD(scene, "lblClass", Text)->setString(_currentClass->getName());

	auto set = game->getLang()->getSetElement("CharacterCreationScreen");
	auto formatedString = StringUtils::format(LANG_TEXT(set, "STATS_TEXT").c_str(),
											  _currentClass->getHealth(),
											  _currentClass->getPower(),
											  _currentClass->getPhysique(),
											  _currentClass->getDexterity(),
											  _currentClass->getIntelligence(),
											  _currentClass->getHealthGain(),
											  _currentClass->getPowerGain(),
											  _currentClass->getPhysiqueGain(),
											  _currentClass->getDexterityGain(),
											  _currentClass->getIntelligenceGain());
	GET_CHILD(scene, "lblStatsText", Text)->setString(formatedString);
}

void CharacterCreationScreen::createCharacter(const SString& name, bool male)
{
	auto msg = Shared::Global::isValidName(name);
	if (msg != Shared::Message::SUCCESS) {
		game->showMsgBoxWithValidationMsg(msg);
		return;
	}

	sendCharacterCreatePacket(name, male);
}

void CharacterCreationScreen::sendCharacterCreatePacket(const SString& name, bool male)
{
	BITSTREAM(bitStream, Shared::PacketID::CHARACTER_CREATE);
	bitStream.write(name);
	bitStream.write(_currentClass->getID());
	bitStream.write(male);
	game->send(&bitStream);
}

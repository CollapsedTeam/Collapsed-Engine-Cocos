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
#include "Statistics.h"

void Statistics::update()
{
	auto character = game->getCharacter();
	SString characterName;

	// If character has a guild then adds a guild tag at the beginning of character name.
	auto guild = character->getGuild();
	if (guild != nullptr)
		characterName = StringUtils::format("[%s]", guild->getTag());

	// Temp function for easy and readable update of label.
	auto setValueToText = [=](cchar* widgetName, u32 value) {
		GET_CHILD(_main, widgetName, Text)->setString(StringUtils::toString(value));
	};

	// Adds real character name
	characterName += character->getName();

	// Updates labels
	GET_CHILD(_main, "lblName", Text)->setString(characterName);
	setValueToText("lblLevel", character->getLevel());
	setValueToText("lblCsh", character->getCash());
	setValueToText("lblHp", character->getHealth());
	setValueToText("lblPw", character->getPower());
	setValueToText("lblPh", character->getPhysique());
	setValueToText("lblDx", character->getDexterity());
	setValueToText("lblInt", character->getIntelligence());
}

void Statistics::setLanguage()
{
	auto set = game->getLang()->getSetElement("Statistics");
	GET_CHILD(_main, "lblLevelCaption", Text)->setString(LANG_TEXT(set, "LEVEL"));
	GET_CHILD(_main, "lblCashCaption", Text)->setString(LANG_TEXT(set, "CASH"));
	GET_CHILD(_main, "lblHpCaption", Text)->setString(LANG_TEXT(set, "HEALTH"));
	GET_CHILD(_main, "lblPwCaption", Text)->setString(LANG_TEXT(set, "POWER"));
	GET_CHILD(_main, "lblPhCaption", Text)->setString(LANG_TEXT(set, "PHYSIQUE"));
	GET_CHILD(_main, "lblDxCaption", Text)->setString(LANG_TEXT(set, "DEXTERITY"));
	GET_CHILD(_main, "lblIntCaption", Text)->setString(LANG_TEXT(set, "INTELLIGENCE"));
	GET_CHILD(_main, "btnClose", Button)->setTitleText(LANG_TEXT(set, "CLOSE"));
}

void Statistics::onLoad(Node* scene) 
{
	DragWindow::onLoad(scene);

	addClickEventToCloseButton();
}

void Statistics::addClickEventToCloseButton()
{
	GET_CHILD(_main, "btnClose", Button)->addClickEventListener([=](Ref* sender) {
		hide();
	});
}

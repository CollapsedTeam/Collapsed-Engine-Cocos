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
#include "Dialogue.h"

ImageView* Dialogue::_main = nullptr;
ImageView* Dialogue::_options = nullptr;
tinyxml2::XMLDocument Dialogue::_dialogueXML;
EventListenerTouchOneByOne* Dialogue::_eventListener = nullptr;

void Dialogue::show(u32 dialogueID, Node* scene, Vec2 position)
{
	hide();
	_main->setPosition(position);
	_options->setPosition(Vec2(_main->getContentSize().width + 10.0f, 0.0f) + position);

	if (scene == nullptr)
		scene = game->getSceneFromRunningScreen();

	// Loads dialogue file from specified locale folder.
	if (!Loader::loadXMLFile(&_dialogueXML, StringUtils::format(
						"Dialogues/%s/%u.xml", game->getLang()->getLocale(), dialogueID))) {
		game->exit();
		return;
	}

	// Event listener for global click/touch.
	_eventListener = EventListenerTouchOneByOne::create();
	_eventListener->onTouchBegan = [](Touch* touch, Event* event) { return false; };
	_eventListener->onTouchMoved = [](Touch* touch, Event* event) { };
	_eventListener->onTouchEnded = [](Touch* touch, Event* event) { };
	scene->getEventDispatcher()->addEventListenerWithFixedPriority(_eventListener, -1);

	scene->addChild(_main);
	scene->addChild(_options);
}

void Dialogue::hide()
{
	// Event listener is only useful when dialogue window is visible.
	if (_eventListener != nullptr) {
		_main->getParent()->getEventDispatcher()->removeEventListener(_eventListener);
		_eventListener = nullptr;
	}

	_main->removeFromParent();
	_options->removeFromParent();
}

void Dialogue::update(const SString& textID, const std::vector<SString>& optionsIDs)
{
	auto rootElement = _dialogueXML.RootElement();
	auto element = rootElement->FirstChildElement(textID.c_str());
	GET_CHILD(_main, "lblText", Text)->setString(element->GetText());

	// Now we can specify global touch/click event because we know id of text.
	_eventListener->onTouchBegan = [=](Touch* touch, Event* event) {
		return textClicked(textID);
	};

	// Parse [P] as character name of player. Otherwise sets what is specified.
	auto who = element->Attribute("who");
	if (strcmp(who, "[P]") == 0) {
		GET_CHILD(_main, "lblWho", Text)->setString(game->getCharacter()->getName());
	} else {
		GET_CHILD(_main, "lblWho", Text)->setString(who);
	}
	
	auto lstOptions = GET_CHILD(_options, "lstOptions", ListView);
	lstOptions->removeAllItems();
	for (u32 i = 0u; i < optionsIDs.size(); ++i) {
		// Adds option model to the list.
		lstOptions->pushBackDefaultItem();

		const auto& optionID = optionsIDs[i];
		auto option = static_cast<Text*>(lstOptions->getItem(i));
		option->setString(StringUtils::format("%u) %s", i + 1u,
			rootElement->FirstChildElement(optionID.c_str())->GetText()));
		option->addClickEventListener([=](Ref* sender) {
			sendDialogueUpdatePacket(optionID);
		});
	}
}

void Dialogue::load(Node* scene)
{
	// Main window of dialogue where text is displayed.
	_main = GET_CHILD(scene, "Dialogue", ImageView);
	_main->retain();
	_main->removeFromParent();

	// Window with options to choose.
	_options = GET_CHILD(scene, "Options", ImageView);
	_options->retain();
	_options->removeFromParent();

	auto lstOptions = GET_CHILD(_options, "lstOptions", ListView);
	auto option = GET_CHILD(lstOptions, "lblOption", Text);
	lstOptions->setItemModel(option);
	option->removeFromParent();
	lstOptions->setItemsMargin(1.0f);
}

bool Dialogue::textClicked(const SString& textID)
{
	// If we have options to choose then we need to ignore this event.
	if (!GET_CHILD(_options, "lstOptions", ListView)->getItems().empty())
		return false;

	sendDialogueUpdatePacket(textID);
	return true;
}

void Dialogue::sendDialogueUpdatePacket(const SString& textOrOptionID)
{
	BITSTREAM(bitStream, Shared::PacketID::DIALOGUE_UPDATE);
	bitStream.write(textOrOptionID);
	game->send(&bitStream);
}

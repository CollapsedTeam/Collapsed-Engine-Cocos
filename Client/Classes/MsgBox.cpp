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

ImageView* MsgBox::_template = nullptr;

RefPtr<MsgBox> MsgBox::create() {
	return new MsgBox();
}

RefPtr<MsgBox> MsgBox::create(const SString& text, const SString& textForLeftButton, 
							  const std::function<void(Ref*)>& funForLeftButton,
							  const SString& textForMiddleButton,
							  const std::function<void(Ref*)>& funForMiddleButton,
							  const SString& textForRightButton,
							  const std::function<void(Ref*)>& funForRightButton)
{
	auto instance = new MsgBox();
	instance->init(text, textForLeftButton, funForLeftButton, textForMiddleButton, 
				   funForMiddleButton, textForRightButton, funForRightButton);
	return instance;
}

void MsgBox::init(const SString& text, const SString& textForLeftButton,
				  const std::function<void(Ref*)>& funForLeftButton,
				  const SString& textForMiddleButton,
				  const std::function<void(Ref*)>& funForMiddleButton,
				  const SString& textForRightButton,
				  const std::function<void(Ref*)>& funForRightButton)
{
	if (_main != nullptr)
		return;

	_main = static_cast<ImageView*>(_template->clone());

	setText(text);
	setTextOfLeftButton(textForLeftButton);
	setTextOfMiddleButton(textForMiddleButton);
	setTextOfRightButton(textForRightButton);

	setClickEventOfLeftButton(funForLeftButton);
	setClickEventOfMiddleButton(funForMiddleButton);
	setClickEventOfRightButton(funForRightButton);
}

void MsgBox::show(Node* scene, Vec2 position)
{
	_main->setPosition(position);

	if (scene == nullptr)
		scene = game->getSceneFromRunningScreen();

	scene->addChild(_main);
}

void MsgBox::hide() {
	_main->removeFromParent();
}

void MsgBox::setText(const SString& text)
{
	auto lblMessage = GET_CHILD(_main, "lblMsg", Text);
	lblMessage->setString(text);
	lblMessage->setVisible(!text.empty());
}

void MsgBox::setTextOfLeftButton(const SString& text)
{
	auto btnLeft = GET_CHILD(_main, "btnL", Button);
	btnLeft->setTitleText(text);
	btnLeft->setVisible(!text.empty());
}

void MsgBox::setTextOfMiddleButton(const SString& text)
{
	auto btnMiddle = GET_CHILD(_main, "btnM", Button);
	btnMiddle->setTitleText(text);
	btnMiddle->setVisible(!text.empty());
}

void MsgBox::setTextOfRightButton(const SString& text)
{
	auto btnRight = GET_CHILD(_main, "btnR", Button);
	btnRight->setTitleText(text);
	btnRight->setVisible(!text.empty());
}

MsgBox::MsgBox()
	: _main(nullptr)
{
	autorelease();
}

void MsgBox::load(Node* scene)
{
	_template = GET_CHILD(scene, "MsgBox", ImageView);
	_template->retain();
	_template->removeFromParent();

	_template->getChildByName("lblMsg")->setVisible(false);
	_template->getChildByName("btnL")->setVisible(false);
	_template->getChildByName("btnM")->setVisible(false);
	_template->getChildByName("btnR")->setVisible(false);
}
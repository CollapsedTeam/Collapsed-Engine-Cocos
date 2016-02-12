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

void DragWindow::onLoad(Node* scene) { 
	if (_main == nullptr)
		return;

	_dragRect.origin = Vec2::ZERO;
	_dragRect.size = _main->getContentSize();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto anchorPoint = _main->getAnchorPoint();
		Rect rect(_main->getPosition(), _main->getContentSize());
		rect.origin -= Vec2(anchorPoint.x * _main->getContentSize().width, 
							anchorPoint.y * _main->getContentSize().height);
		
		auto touchLocation = touch->getLocation();

		// Is window touched?
		if (!rect.containsPoint(touchLocation))
			return false;

		// Sets window to top layer(last rendered) of the screen.
		auto parent = _main->getParent();
		_main->removeFromParent();
		parent->addChild(_main);

		auto transformedDragRect = _dragRect;
		transformedDragRect.origin += rect.origin;
		
		// Is window touched in drag area?
		return transformedDragRect.containsPoint(touchLocation);
	};

	listener->onTouchMoved = [=](Touch* touch, Event* event) {
		auto newPos = _main->getPosition();
		newPos += touch->getLocation() - touch->getPreviousLocation();
		_main->setPosition(newPos);
	};

	listener->onTouchEnded = [](Touch* touch, Event* event) { };

	_main->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _main);
}
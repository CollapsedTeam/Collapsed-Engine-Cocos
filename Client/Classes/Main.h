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

#pragma once

#include <Jinra/Jinra.h>
#include <Jinra/BitStream.h>
#include <Jinra/NetClient.h>
#include <Jinra/MessageIdentifiers.h>

#include <cocos2d.h>
using namespace cocos2d;

#include <cocostudio/CocoStudio.h>
using namespace cocostudio;

#include <ui/CocosGUI.h>
using namespace ui;

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <tinyxml2/tinyxml2.h>
#include <json/rapidjson.h>

#include <Typedefs.h>

#include <SharedGlobal.h>
#include <SharedClass.h>
#include <SharedCharacter.h>
#include <SharedGuild.h>

#include "Config.h"

#define DESKTOP_PLATFORM (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32\
 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC\
 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#define LANG_TEXT game->getLang()->getText 

#define GET_CHILD(parent, childName, type) \
	static_cast<type*>(parent->getChildByName(childName))

extern Vec2 centerPoint;

#include "Lang.h"
#include "Screen.h"
#include "DragWindow.h"
#include "Loader.h"
#include "Client.h"
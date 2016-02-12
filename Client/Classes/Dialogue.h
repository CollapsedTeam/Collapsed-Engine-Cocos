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

class Dialogue
{
	friend class LoadingScreen;

public:
	static void show(u32 dialogueID, Node* scene = nullptr, Vec2 position = Vec2::ZERO);

	static void hide();

	static void update(const SString& textID, const std::vector<SString>& optionsIDs);

private:
	static void load(Node* scene);

	static bool textClicked(const SString& textID);

	static void sendDialogueUpdatePacket(const SString& textOrOptionID);

private:
	static ImageView* _main;
	static ImageView* _options;
	static tinyxml2::XMLDocument _dialogueXML;
	static EventListenerTouchOneByOne* _eventListener;
};

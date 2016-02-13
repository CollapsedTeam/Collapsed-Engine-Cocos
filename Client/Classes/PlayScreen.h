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

#include "Chat.h"
#include "Statistics.h"
#include "Guild.h"

class PlayScreen : public Screen
{
public:
	CREATE_FUNC(PlayScreen);

	static Scene* createScene();

	Chat* getChat() {
		return &_chat;
	}

	static Statistics* getStatisticsWindow() {
		return &_statistics;
	}

	static Guild* getGuildWindow() {
		return &_guild;
	}

	static GuildCreation* getGuildCreationWindow() {
		return &_guildCreation;
	}

	static GuildSettings* getGuildSettingsWindow() {
		return &_guildSettings;
	}

	static GuildAddMember* getGuildAddMemberWindow() {
		return &_guildAddMember;
	}

	void setLanguage() override;

private:
	bool init() override;

	void initChat();

	void addClickEventToOptionsButton();

	void addClickEventToStatisticsButton();

	void addClickEventToGuildButton();
	
	void addClickEventToDialogueButton();

	void onEnter() override;

	void update(float delta) override;

	void initDialogue();

private:
	Chat _chat;
	static Statistics _statistics;
	static Guild _guild;
	static GuildCreation _guildCreation;
	static GuildSettings _guildSettings;
	static GuildAddMember _guildAddMember;
};

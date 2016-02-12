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
#include "GuildSettings.h"

void GuildSettings::setLanguage()
{
	auto set = game->getLang()->getSetElement("GuildSettings");
	GET_CHILD(_main, "lblTitle", Text)->setString(LANG_TEXT(set, "TITLE"));
	GET_CHILD(_main, "txtDesc", TextField)->setString(LANG_TEXT(set, "DESC_PLACEHOLDER"));
	GET_CHILD(_main, "btnApply", Button)->setTitleText(LANG_TEXT(set, "APPLY"));
	GET_CHILD(_main, "btnCancel", Button)->setTitleText(LANG_TEXT(set, "CANCEL"));
}

void GuildSettings::onLoad(Node* scene)
{
	DragWindow::onLoad(scene);

	addClickEventToApplyButton();
	addClickEventToCancelButton();
}

void GuildSettings::addClickEventToApplyButton()
{
	GET_CHILD(_main, "btnApply", Button)->addClickEventListener([=](Ref* sender) {
		applySettings();
	});
}

void GuildSettings::addClickEventToCancelButton()
{
	GET_CHILD(_main, "btnCancel", Button)->addClickEventListener([=](Ref* sender) {
		revertSettings();
		hide();
	});
}

void GuildSettings::applySettings()
{
	auto guild = game->getCharacter()->getGuild();

	// Sends packet with new guild description if was changed.
	auto description = GET_CHILD(_main, "txtDesc", TextField)->getString();
	if (description != guild->getDescription())
		sendSetGuildDescriptionPacket(description);

	// Sends packet with new name of rank if was changed.
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i) {
		auto rankName = getRankName(i);
		if (strcmp(rankName, guild->getRankName(i)) != 0)
			sendSetGuildRankNamePacket(i, rankName);
	}

	hide();
}

void GuildSettings::revertSettings()
{
	auto guild = game->getCharacter()->getGuild();
	setDescription(guild->getDescription());
	for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i)
		GuildSettings::setRankName(i, guild->getRankName(i));
}

void GuildSettings::sendSetGuildDescriptionPacket(const SString& description)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_SET_DESCRIPTION);
	bitStream.write(description);
	game->send(&bitStream);
}

void GuildSettings::sendSetGuildRankNamePacket(u8 rankIndex, cchar* rankName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_SET_RANK_NAME);
	bitStream.write(rankIndex);
	bitStream.write(rankName);
	game->send(&bitStream);
}

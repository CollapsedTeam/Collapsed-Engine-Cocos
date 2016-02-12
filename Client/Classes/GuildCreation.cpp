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
#include "GuildCreation.h"

void GuildCreation::setLanguage()
{
	auto set = game->getLang()->getSetElement("GuildCreation");
	GET_CHILD(_main, "lblTitle", Text)->setString(LANG_TEXT(set, "TITLE"));
	GET_CHILD(_main, "txtName", TextField)->setPlaceHolder(LANG_TEXT(set, "NAME_PLACEHOLDER"));
	GET_CHILD(_main, "txtTag", TextField)->setPlaceHolder(LANG_TEXT(set, "TAG_PLACEHOLDER"));
	GET_CHILD(_main, "btnCreate", Button)->setTitleText(LANG_TEXT(set, "CREATE"));
	GET_CHILD(_main, "btnCancel", Button)->setTitleText(LANG_TEXT(set, "CANCEL"));
}

void GuildCreation::onLoad(Node* scene)
{
	DragWindow::onLoad(scene);

	addClickEventToCreateButton();
	addClickEventToCancelButton();
}

void GuildCreation::addClickEventToCreateButton()
{
	GET_CHILD(_main, "btnCreate", Button)->addClickEventListener([=](Ref* sender) {
		create(GET_CHILD(_main, "txtName", TextField)->getString(),
			   GET_CHILD(_main, "txtTag", TextField)->getString());
	});
}

void GuildCreation::addClickEventToCancelButton()
{
	GET_CHILD(_main, "btnCancel", Button)->addClickEventListener([=](Ref* sender) {
		hide();
	});
}

void GuildCreation::create(const SString& guildName, const SString& guildTag)
{
	auto msg = Shared::Global::isValidGuildName(guildName);
	if (msg != Shared::Message::SUCCESS) {
		game->showMsgBoxWithValidationMsg(msg);
		return;
	}

	msg = Shared::Global::isValidGuildTag(guildTag);
	if (msg != Shared::Message::SUCCESS) {
		game->showMsgBoxWithValidationMsg(msg);
		return;
	}

	sendCreateGuildPacket(guildName, guildTag);
}

void GuildCreation::sendCreateGuildPacket(const SString& guildName, const SString& guildTag)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_CREATE);
	bitStream.write(guildName);
	bitStream.write(guildTag);
	game->send(&bitStream);
}

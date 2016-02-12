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
#include "PlayScreen.h"

void GuildAddMember::setLanguage()
{
	auto set = game->getLang()->getSetElement("GuildAddMember");
	GET_CHILD(_main, "lblTitle", Text)->setString(LANG_TEXT(set, "TITLE"));
	GET_CHILD(_main, "txtName", TextField)->setPlaceHolder(LANG_TEXT(set, "NAME_PLACEHOLDER"));
	GET_CHILD(_main, "btnInvite", Button)->setTitleText(LANG_TEXT(set, "INVITE"));
	GET_CHILD(_main, "btnCancel", Button)->setTitleText(LANG_TEXT(set, "CANCEL"));
}

void GuildAddMember::onLoad(Node* scene)
{
	DragWindow::onLoad(scene);

	addClickEventToInviteButton();
	addClickEventToCancelButton();
}

void GuildAddMember::addClickEventToInviteButton()
{
	GET_CHILD(_main, "btnInvite", Button)->addClickEventListener([=](Ref* sender) {
		invite(GET_CHILD(_main, "txtName", TextField)->getString());
		hide();
	});
}

void GuildAddMember::addClickEventToCancelButton()
{
	GET_CHILD(_main, "btnCancel", Button)->addClickEventListener([=](Ref* sender) {
		hide();
	});
}

void GuildAddMember::invite(const SString& characterName)
{
	auto msg = Shared::Global::isValidName(characterName);
	if (msg != Shared::Message::SUCCESS) {
		game->showMsgBoxWithValidationMsg(msg);
		return;
	}

	if (PlayScreen::getGuildWindow()->isInGuild(characterName)) {
		game->showMsgBoxWithValidationMsg(Shared::Message::CHARACTER_ALREADY_IN_GUILD);
		return;
	}

	sendInviteCharacterToGuildPacket(characterName);
}

void GuildAddMember::sendInviteCharacterToGuildPacket(const SString& characterName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_INVITE_CHARACTER);
	bitStream.write(characterName);
	game->send(&bitStream);
}

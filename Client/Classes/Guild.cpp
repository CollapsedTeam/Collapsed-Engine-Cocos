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
#include "Guild.h"
#include "MsgBox.h"
#include "PlayScreen.h"

void Guild::addMember(const SString& name, bool online, u16 level, Shared::GuildRanks rank,
					  const SString& note)
{
	auto lstMembers = GET_CHILD(_main, "lstMembers", ListView);
	lstMembers->pushBackDefaultItem();
	auto member = lstMembers->getItem(lstMembers->getItems().size() - 1);
	
	auto lblName = GET_CHILD(member, "lblName", Text);
	lblName->setString(name);
	if (online)
		lblName->setTextColor(Color4B::GREEN);

	GET_CHILD(member, "lblLevel", Text)->setString(StringUtils::toString(level));
	GET_CHILD(member, "lblRank", Text)->setString(
		PlayScreen::getGuildSettingsWindow()->getRankName(static_cast<u8>(rank)));
	GET_CHILD(member, "lblNote", Text)->setString(note);

	update();
}

void Guild::removeMember(const SString& name)
{
	auto lstMembers = GET_CHILD(_main, "lstMembers", ListView);
	for (s32 i = 0; i < lstMembers->getItems().size(); ++i) {
		auto member = lstMembers->getItem(i);
		if (GET_CHILD(member, "lblName", Text)->getString() == name) {
			lstMembers->removeItem(i);
			break;
		}
	}
}

void Guild::update()
{
	auto myRank = getMemberRank(game->getCharacter()->getName());

	// Only owner and managers can invite new members and promote/degrade members.
	auto havePermissionsToAddAndEditMember = (myRank == Shared::GuildRanks::OWNER || 
											  myRank == Shared::GuildRanks::MANAGER);

	GET_CHILD(_main, "btnSettings", Button)->setVisible((myRank == Shared::GuildRanks::OWNER));
	GET_CHILD(_main, "btnAddMember", Button)->setVisible(havePermissionsToAddAndEditMember);

	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		auto memberName = GET_CHILD(member, "lblName", Text)->getString();
		auto memberRank = getMemberRank(memberName);
		auto visible = (memberName != game->getCharacter()->getName()) &&
			havePermissionsToAddAndEditMember && (memberRank < myRank);

		member->getChildByName("btnPromote")->setVisible(visible);
		auto btnDegradation = GET_CHILD(member, "btnDegradation", Button);
		btnDegradation->setVisible(visible);
		btnDegradation->setEnabled(memberRank > Shared::GuildRanks::RECRUIT);
		member->getChildByName("btnKick")->setVisible(visible);
	}
}

void Guild::clear() {
	GET_CHILD(_main, "lstMembers", ListView)->removeAllItems();
}

bool Guild::isInGuild(const SString& characterName)
{
	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		if (GET_CHILD(member, "lblName", Text)->getString() == characterName)
			return true;
	}
	return false;
}

Shared::GuildRanks Guild::getMemberRank(const SString& memberName)
{
	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		if (GET_CHILD(member, "lblName", Text)->getString() == memberName) {
			auto rankName = GET_CHILD(member, "lblRank", Text)->getString();
			for (u8 i = 0u; i < Shared::GUILD_RANKS_COUNT; ++i) {
				if (rankName == PlayScreen::getGuildSettingsWindow()->getRankName(i))
					return static_cast<Shared::GuildRanks>(i);
			}
		}
	}
	return Shared::GuildRanks::RECRUIT;
}

void Guild::setLanguage()
{
	auto set = game->getLang()->getSetElement("Guild");
	GET_CHILD(_main, "txtNote", TextField)->setPlaceHolder(LANG_TEXT(set, "NOTE_PLACEHOLDER"));
	GET_CHILD(_main, "btnSetNote", Button)->setTitleText(LANG_TEXT(set, "SET_NOTE"));
	GET_CHILD(_main, "btnLeave", Button)->setTitleText(LANG_TEXT(set, "LEAVE"));
	GET_CHILD(_main, "btnClose", Button)->setTitleText(LANG_TEXT(set, "CLOSE"));
}

void Guild::setOnlineOfflineMember(const SString& name, bool online)
{
	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		auto lblName = GET_CHILD(member, "lblName", Text);
		if (lblName->getString() == name)
			lblName->setTextColor(online ? Color4B::GREEN : Color4B::WHITE);
	}
}

void Guild::setRankName(u32 index, const SString& name)
{
	auto guild = game->getCharacter()->getGuild();
	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		auto lblRank = GET_CHILD(member, "lblRank", Text);
		if (lblRank->getString() == guild->getRankName(index))
			lblRank->setString(name);
	}
}

void Guild::setMemberRank(const SString& name, u8 rank)
{
	auto guild = game->getCharacter()->getGuild();
	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		if (GET_CHILD(member, "lblName", Text)->getString() == name)
			GET_CHILD(member, "lblRank", Text)->setString(guild->getRankName(rank));
	}
}

void Guild::setMemberNote(const SString& memberName, const SString& memberNote)
{
	auto members = GET_CHILD(_main, "lstMembers", ListView)->getItems();
	for (auto member : members) {
		if (GET_CHILD(member, "lblName", Text)->getString() == memberName) {
			GET_CHILD(member, "lblNote", Text)->setString(memberNote);
			break;
		}
	}
}

void Guild::onLoad(Node* scene)
{
	DragWindow::onLoad(scene);

	auto lstMembers = GET_CHILD(_main, "lstMembers", ListView);
	auto member = GET_CHILD(lstMembers, "Member", Layout);
	GET_CHILD(member, "btnPromote", Button)->addClickEventListener([=](Ref* sender) {
		promoteMember(GET_CHILD(static_cast<Button*>(sender)->getParent(), "lblName",
			Text)->getString());
	});
	GET_CHILD(member, "btnDegradation", Button)->addClickEventListener([=](Ref* sender) {
		sendDegradeMemberPacket(GET_CHILD(static_cast<Button*>(sender)->getParent(), 
			"lblName", Text)->getString());
	});
	GET_CHILD(member, "btnKick", Button)->addClickEventListener([=](Ref* sender) {
		kickMember(GET_CHILD(static_cast<Button*>(sender)->getParent(), "lblName", 
			Text)->getString());
	});
	lstMembers->setItemsMargin(1.0f);
	lstMembers->setItemModel(member);
	member->removeFromParent();

	addClickEventToSettingsButton();
	addClickEventToAddMemberButton();
	addClickEventToSetNoteButton();
	addClickEventToLeaveButton();
	addClickEventToCloseButton();
}

void Guild::addClickEventToSettingsButton()
{
	GET_CHILD(_main, "btnSettings", Button)->addClickEventListener([](Ref* sender) {
		PlayScreen::getGuildSettingsWindow()->show();
	});
}

void Guild::addClickEventToAddMemberButton()
{
	GET_CHILD(_main, "btnAddMember", Button)->addClickEventListener([](Ref* sender) {
		PlayScreen::getGuildAddMemberWindow()->show();
	});
}

void Guild::addClickEventToSetNoteButton()
{
	GET_CHILD(_main, "btnSetNote", Button)->addClickEventListener([=](Ref* sender) {
		sendSetMemberNotePacket(GET_CHILD(_main, "txtNote", TextField)->getString());
	});
}

void Guild::addClickEventToLeaveButton()
{
	GET_CHILD(_main, "btnLeave", Button)->addClickEventListener([=](Ref* sender) {
		auto set = game->getLang()->getSetElement("Guild");
		
		// Leave of owner means guild deletion, so we need to inform about it.
		SString leaveMsg;
		if (getMemberRank(game->getCharacter()->getName()) == Shared::GuildRanks::OWNER) {
			leaveMsg = LANG_TEXT(set, "MB_LEAVE_OWNER");
		} else {
			leaveMsg = LANG_TEXT(set, "MB_LEAVE");
		}

		auto msgBox = MsgBox::create();
		auto yesFunc = [=](Ref*) {
			sendLeaveGuildPacket();
			msgBox->hide();
		};
		msgBox->init(leaveMsg, 
					 LANG_TEXT(set, "MB_YES"), yesFunc,
					 "", nullptr,
					 LANG_TEXT(set, "MB_NO"), [=](Ref*) { msgBox->hide(); });
		msgBox->show();
	});
}

void Guild::addClickEventToCloseButton()
{
	GET_CHILD(_main, "btnClose", Button)->addClickEventListener([=](Ref* sender) {
		hide();
	});
}

void Guild::promoteMember(const SString& memberName)
{
	// Promoting manager means promote to owner, so current owner will be degraded.
	// But before that, the owner must click 'Yes' in message box.
	if (getMemberRank(memberName) == Shared::GuildRanks::MANAGER) {
		auto set = game->getLang()->getSetElement("Guild");
		auto msgBox = MsgBox::create();

		auto yesFunc = [=](Ref*) {
			sendPromoteMemberPacket(memberName);
			msgBox->hide();
		};

		msgBox->init(StringUtils::format(LANG_TEXT(set, "MB_PROMOTE_TO_OWNER").c_str(), 
			memberName.c_str()),
			LANG_TEXT(set, "MB_YES"), yesFunc,
			"", nullptr,
			LANG_TEXT(set, "MB_NO"), [=](Ref*) { msgBox->hide(); });
		msgBox->show();
	} else {
		sendPromoteMemberPacket(memberName);
	}
}

void Guild::kickMember(const SString& memberName)
{
	auto set = game->getLang()->getSetElement("Guild");
	auto msgBox = MsgBox::create();

	auto yesFunc = [=](Ref*) { 
		sendKickMemberPacket(memberName);
		msgBox->hide(); 
	};

	msgBox->init(StringUtils::format(
		LANG_TEXT(set, "MB_KICK").c_str(), memberName.c_str()),
		LANG_TEXT(set, "MB_YES"), yesFunc, 
		"", nullptr, 
		LANG_TEXT(set, "MB_NO"), [=](Ref*) { msgBox->hide(); });
	msgBox->show();
}

void Guild::sendSetMemberNotePacket(const SString& note)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_SET_MEMBER_NOTE);
	bitStream.write(note);
	game->send(&bitStream);
}

void Guild::sendPromoteMemberPacket(const SString& memberName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_PROMOTE_MEMBER);
	bitStream.write(memberName);
	game->send(&bitStream);
}

void Guild::sendDegradeMemberPacket(const SString& memberName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_DEGRADE_MEMBER);
	bitStream.write(memberName);
	game->send(&bitStream);
}

void Guild::sendKickMemberPacket(const SString& memberName)
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_KICK_MEMBER);
	bitStream.write(memberName);
	game->send(&bitStream);
}

void Guild::sendLeaveGuildPacket()
{
	BITSTREAM(bitStream, Shared::PacketID::GUILD_LEAVE);
	bitStream.write(game->getCharacter()->getName());
	game->send(&bitStream);
}
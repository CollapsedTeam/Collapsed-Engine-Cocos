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

#include "GuildCreation.h"
#include "GuildSettings.h"
#include "GuildAddMember.h"

class Guild : public DragWindow
{
public:
	void addMember(const SString& name, bool online, u16 level, Shared::GuildRanks rank, 
				   const SString& note);

	void removeMember(const SString& name);

	void update();

	void clear();

	bool isInGuild(const SString& characterName);

	Shared::GuildRanks getMemberRank(const SString& memberName);

	void setLanguage() override;

	inline void setName(const SString& name) {
		GET_CHILD(_main, "lblName", Text)->setString(name);
	}

	inline void setDescription(const SString& description) {
		GET_CHILD(_main, "lblDesc", Text)->setString(description);
	}

	void setOnlineOfflineMember(const SString& name, bool online);

	void setRankName(u32 index, const SString& name);

	void setMemberRank(const SString& name, u8 rank);

	void setMemberNote(const SString& memberName, const SString& memberNote);

private:
	void onLoad(Node* scene) override;

	void addClickEventToSettingsButton();

	void addClickEventToAddMemberButton();

	void addClickEventToSetNoteButton();

	void addClickEventToLeaveButton();

	void addClickEventToCloseButton();

	void promoteMember(const SString& memberName);

	void kickMember(const SString& memberName);

	void sendSetMemberNotePacket(const SString& note);

	void sendPromoteMemberPacket(const SString& memberName);

	void sendDegradeMemberPacket(const SString& memberName);

	void sendKickMemberPacket(const SString& memberName);

	void sendLeaveGuildPacket();
};

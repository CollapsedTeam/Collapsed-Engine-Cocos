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

class GuildSettings : public DragWindow
{
public:
	void setLanguage() override;

	inline cchar* getRankName(u8 index) {
		return GET_CHILD(_main, StringUtils::format("txtRank%u", index), 
						 TextField)->getString().c_str();
	}

	inline void setRankName(u8 index, const SString& rankName) {
		GET_CHILD(_main, StringUtils::format("txtRank%u", index), TextField)->setString(
			rankName);
	}

	inline void setDescription(const SString& description) {
		GET_CHILD(_main, "txtDesc", TextField)->setString(description);
	}

private:
	void onLoad(Node* scene) override;

	void addClickEventToApplyButton();

	void addClickEventToCancelButton();

	void applySettings();

	void revertSettings();

	void sendSetGuildDescriptionPacket(const SString& description);

	void sendSetGuildRankNamePacket(u8 rankIndex, cchar* rankName);
};

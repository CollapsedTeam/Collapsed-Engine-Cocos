/*
 * Copyright 2015 Collapsed Team
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

namespace Shared {

enum class GuildRanks : u8
{
	RECRUIT,
	MEMBER,
	MANAGER,
	OWNER,
};
const u8 GUILD_RANKS_COUNT = 4u;

class Guild
{
public:
	/**
	 * Creates new guild object.
	 */
	Guild() { }

	/**
	 * Creates new guild object.
	 * @param[in] tag Tag of guild.
	 */
	Guild(const SString& tag) 
		: _tag(tag)
	{ }

	/**
	 * Creates new guild object.
	 * @param[in] tag Tag of guild.
	 * @param[in] description Description of guild.
	 */
	Guild(const SString& tag, const SString& description)
		: _tag(tag), _description(description)
	{ }

	/**
	 * Gets tag of guild.
	 * @return Return tag of guild.
	 */
	inline cchar* getTag() const {
		return _tag.c_str();
	}

	/**
	 * Gets description of guild.
	 * @return Return description of guild.
	 */
	inline cchar* getDescription() const {
		return _description.c_str();
	}

	/**
	 * Gets rank name[index] of guild.
	 * @return Return rank name[index] of guild.
	 */
	inline cchar* getRankName(u8 index) const {
		return _ranksNames[index].c_str();
	}

	/**
	 * Sets tag of guild.
	 * @param tag Tag of guild.
	 */
	inline void setTag(const SString& tag) {
		_tag = tag;
	}

	/**
	 * Sets description of guild.
	 * @param description Description of guild.
	 */
	virtual inline void setDescription(const SString& description) {
		_description = description;
	}

	/**
	 * Sets rank name[index] of guild.
	 * @param index Index of rank name.
	 * @param rankName Rank name of guild.
	 */
	virtual inline void setRankName(u8 index, const SString& rankName) {
		_ranksNames[index] = rankName;
	}

protected:
	SString _tag;
	SString _description;
	SString _ranksNames[GUILD_RANKS_COUNT];
};

} // namespace Shared


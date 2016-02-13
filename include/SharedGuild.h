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
	 *
	 */
	Guild() { }

	/**
	 * 
	 * @param[in] tag
	 */
	Guild(const SString& tag) 
		: _tag(tag)
	{ }

	/**
	 *
	 * @param[in] tag
	 * @param[in] description
	 */
	Guild(const SString& tag, const SString& description)
		: _tag(tag), _description(description)
	{ }

	/**
	 * 
	 * @return
	 */
	cchar* getTag() const {
		return _tag.c_str();
	}

	/**
	 *
	 * @return
	 */
	cchar* getDescription() const {
		return _description.c_str();
	}

	/**
	 *
	 * @return
	 */
	cchar* getRankName(u8 index) const {
		return _ranksNames[index].c_str();
	}

	/**
	 * 
	 * @param tag
	 */
	void setTag(const SString& tag) {
		_tag = tag;
	}

	/**
	 * 
	 * @param description
	 */
	virtual void setDescription(const SString& description) {
		_description = description;
	}

	/**
	 * 
	 * @param index
	 * @param rankName
	 */
	virtual void setRankName(u8 index, const SString& rankName) {
		_ranksNames[index] = rankName;
	}

protected:
	SString _tag;
	SString _description;
	SString _ranksNames[GUILD_RANKS_COUNT];
};

} // namespace Shared


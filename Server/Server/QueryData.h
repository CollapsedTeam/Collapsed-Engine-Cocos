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

class QueryData
{
public:
	enum class ID : u8
	{
		KEEP_ALIVE,
		CHARACTER_LEVEL_UP,

		GUILD_SET_DESCRIPTION,
		GUILD_SET_RANKS_NAMES,
		GUILD_ADD_MEMBER,
		GUILD_SET_MEMBER_NOTE,
		GUILD_SET_MEMBER_RANK
	};

public:
	QueryData(ID id, void* object, void* object2 = nullptr, void* object3 = nullptr);

	ID getID() const {
		return _id;
	}

	void* getObject() const {
		return _object;
	}

	void* getObject2() const {
		return _object2;
	}

	void* getObject3() const {
		return _object2;
	}

	bool operator==(const QueryData& queryData) {
		return (_id == queryData.getID() && _object == queryData.getObject() &&
				_object2 == queryData.getObject2() && _object3 == queryData.getObject3());
	}

protected:
	ID _id;
	void* _object;
	void* _object2;
	void* _object3;
};

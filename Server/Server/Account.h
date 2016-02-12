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

#include "Character.h"

class Account
{
public:
	enum class Type : char
	{
		NORMAL,
		SUPER,
		BANNED
	};

public:
	/**
	 * 
	 * @param email
	 */
	Account(const String& email);
	
	/**
	 * 
	 */
	virtual ~Account();

	/**
	 *
	 */
	void deleteCharacter();

	/**
	 * 
	 * @return
	 */
	inline bool isPlaying() const {
		return (_character != nullptr);
	}

	/**
	 *
	 * @return 
	 */
	inline u32 getID() const {
		return _id;
	}

	/**
	 * 
	 * @return 
	 */
	inline cchar* getEmail() const {
		return _email.c_str();
	}

	/**
	 *
	 * @return 
	 */
	inline Type getType() const {
		return _type;
	}

	/**
	 *
	 * @return 
	 */
	inline Character* getCharacter() const {
		return _character;
	}

	/**
	 *
	 * @param id
	 */
	inline void setID(u32 id) {
		_id = id;
	}

	/**
	 * 
	 * @param email
	 */
	inline void setEmail(const String& email) {
		_email = email;
	}

	/**
	 *
	 * @param type
	 */
	inline void setType(Type type) {
		_type = type;
	}

	/**
	 * 
	 * @param character
	 */
	inline void setCharacter(Character* character) {
		_character = character;
	}

protected:
	u32 _id;
	String _email;
	Type _type;
	Character* _character;
};

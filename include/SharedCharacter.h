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

class Character
{
public:
	/**
	 * 
	 * @param[in] id
	 * @param[in] name 
	 * @param[in] level
	 * @param[in] male
	 * @param[in] cash
	 * @param[in] health
	 * @param[in] power
	 * @param[in] physique
	 * @param[in] dexterity
	 * @param[in] intelligence
	 */
	Character(u32 id, const SString& name, u16 level, bool male, u32 cash, u16 health, 
			  u16 power, u16 physique, u16 dexterity, u16 intelligence)
			: _id(id), _name(name), _level(level), _male(male), _cash(cash), _health(health), 
			_power(power), _physique(physique), _dexterity(dexterity), 
			_intelligence(intelligence)
	{ }

	/**
	 * 
	 * @return 
	 */
	bool isMale() const {
		return _male;
	}

	/**
	 * 
	 * @return
	 */
	bool isFemale() const {
		return !_male;
	}

	/**
	 * 
	 * @return 
	 */
	u32 getID() const {
		return _id;
	}

	/**
	 * 
	 * @return 
	 */
	cchar* getName() const {
		return _name.c_str();
	}

	/**
	 * 
	 * @return
	 */
	u16 getLevel() const {
		return _level;
	}

	/**
	 *
	 * @return
	 */
	u32 getCash() const {
		return _cash;
	}

	/**
	 *
	 * @return
	 */
	u16 getHealth() const {
		return _health;
	}

	/**
	 * 
	 * @return
	 */
	u16 getPower() const {
		return _power;
	}

	/**
	 *
	 * @return 
	 */
	u16 getPhysique() const {
		return _physique;
	}

	/**
	 * 
	 * @return
	 */
	u16 getDexterity() const {
		return _dexterity;
	}

	/**
	 * 
	 * @return 
	*/
	u16 getIntelligence() const {
		return _intelligence;
	}
	
	/**
	 * 
	 * @param id
	 */
	void setID(u32 id) {
		_id = id;
	}

	/**
	 *
	 * @param name
	 */
	void setName(const SString& name) {
		_name = name;
	}

	/**
	 * 
	 * @param
	 */
	void setLevel(u16 level) {
		_level = level;
	}

	/**
	 * 
	 * @param cash
	 */
	void setCash(u32 cash) {
		_cash = cash;
	}

	/**
	 * 
	 * @param health 
	 */
	void setHealth(u16 health) {
		_health = health;
	}

	/**
	 * 
	 * @param power
	 */
	void setPower(u16 power) {
		_power = power;
	}

	/**
	 * 
	 * @param physique
	 */
	void setPhysique(u16 physique) {
		_physique = physique;
	}

	/**
	 * 
	 * @param dexterity
	 */
	void setDexterity(u16 dexterity) {
		_dexterity = dexterity;
	}

	/**
	 * 
	 * @param intelligence
	 */
	void setIntelligence(u16 intelligence) {
		_intelligence = intelligence;
	}

protected:
	u32 _id;
	SString _name;
	u16 _level;
	bool _male;
	u32 _cash;
	u16 _health;
	u16 _power;
	u16 _physique;
	u16 _dexterity;
	u16 _intelligence;
};

} // namespace Shared

typedef Shared::Character SharedCharacter;


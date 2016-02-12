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
	 * Creates new character object.
	 * @param[in] id ID of character.
	 * @param[in] name Name of character.
	 * @param[in] level Level of character.
	 * @param[in] male Sex of character.
	 * @param[in] cash Cash of character.
	 * @param[in] health Health of character.
	 * @param[in] power Power of character.
	 * @param[in] physique Physique of character.
	 * @param[in] dexterity Dexterity of character.
	 * @param[in] intelligence Intelligence of character.
	 */
	Character(u32 id, const SString& name, u16 level, bool male, u32 cash, u16 health, 
			  u16 power, u16 physique, u16 dexterity, u16 intelligence)
			: _id(id), _name(name), _level(level), _male(male), _cash(cash), _health(health), 
			_power(power), _physique(physique), _dexterity(dexterity), 
			_intelligence(intelligence)
	{ }

	/**
	 * Checks if a character's sex is male.
	 * @return Return true if character's sex is male. Otherwise, it returns false.
	 */
	inline bool isMale() const {
		return _male;
	}

	/**
	 * Checks if a character's sex is female.
	 * @return Return true if character's sex is female. Otherwise, it returns false.
	 */
	inline bool isFemale() const {
		return !_male;
	}

	/**
	 * Gets ID of character.
	 * @return Return ID of character.
	 */
	inline u32 getID() const {
		return _id;
	}

	/**
	 * Gets name of character.
	 * @return Return name of character.
	 */
	inline cchar* getName() const {
		return _name.c_str();
	}

	/**
	 * Gets level of character.
	 * @return Return level of character.
	 */
	inline u16 getLevel() const {
		return _level;
	}

	/**
	 * Gets cash of character.
	 * @return Return cash of character.
	 */
	inline u32 getCash() const {
		return _cash;
	}

	/**
	 * Gets health of character.
	 * @return Return health of character.
	 */
	inline u16 getHealth() const {
		return _health;
	}

	/**
	 * Gets power of character.
	 * @return Return power of character.
	 */
	inline u16 getPower() const {
		return _power;
	}

	/**
	 * Gets physique of character.
	 * @return Return physique of character.
	 */
	inline u16 getPhysique() const {
		return _physique;
	}

	/**
	 * Gets dexterity of character.
	 * @return Return dexterity of character.
	 */
	inline u16 getDexterity() const {
		return _dexterity;
	}

	/**
	 * Gets intelligence of character.
	 * @return Return intelligence of character.
	*/
	inline u16 getIntelligence() const {
		return _intelligence;
	}
	
	/**
	 * Sets ID of character.
	 * @param id ID of character.
	 */
	inline void setID(u32 id) {
		_id = id;
	}

	/**
	 * Sets name of character.
	 * @param name Name of character.
	 */
	inline void setName(const SString& name) {
		_name = name;
	}

	/**
	 * Sets level of character.
	 * @param level Level of character.
	 */
	inline void setLevel(u16 level) {
		_level = level;
	}

	/**
	 * Sets cash of character.
	 * @param cash Cash of character.
	 */
	inline void setCash(u32 cash) {
		_cash = cash;
	}

	/**
	 * Sets health of character.
	 * @param health Health of character.
	 */
	inline void setHealth(u16 health) {
		_health = health;
	}

	/**
	 * Sets power of character.
	 * @param power Power of character.
	 */
	inline void setPower(u16 power) {
		_power = power;
	}

	/**
	 * Sets physique of character.
	 * @param physique Physique of character.
	 */
	inline void setPhysique(u16 physique) {
		_physique = physique;
	}

	/**
	 * Sets dexterity of character.
	 * @param dexterity Dexterity of character.
	 */
	inline void setDexterity(u16 dexterity) {
		_dexterity = dexterity;
	}

	/**
	 * Sets intelligence of character.
	 * @param intelligence Intelligence of character.
	 */
	inline void setIntelligence(u16 intelligence) {
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


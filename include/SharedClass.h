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

class Class
{
public:
	Class(u32 id, const SString& name, u8 health, u8 power, u8 physique, u8 dexterity, 
		  u8 intelligence, u8 healthGain, u8 powerGain, u8 physiqueGain, u8 dexterityGain,
		  u8 intelligenceGain)
		: _id(id), _name(name), _health(health), _power(power), _physique(physique), 
		_dexterity(dexterity), _intelligence(intelligence), _healthGain(healthGain),
		_powerGain(powerGain), _physiqueGain(physiqueGain), _dexterityGain(dexterityGain),
		_intelligenceGain(intelligenceGain)
	{ }

	u32 getID() const {
		return _id;
	}

	cchar* getName() const {
		return _name.c_str();
	}

	u8 getHealth() const {
		return _health;
	}

	u8 getPower() const {
		return _power;
	}

	u8 getPhysique() const {
		return _physique;
	}

	u8 getDexterity() const {
		return _dexterity;
	}

	u8 getIntelligence() const {
		return _intelligence;
	}

	u8 getHealthGain() const {
		return _healthGain;
	}

	u8 getPowerGain() const {
		return _powerGain;
	}

	u8 getPhysiqueGain() const {
		return _physiqueGain;
	}

	u8 getDexterityGain() const {
		return _dexterityGain;
	}

	u8 getIntelligenceGain() const {
		return _intelligenceGain;
	}

private:
	u32 _id;
	SString _name;
	u8 _health;
	u8 _power;
	u8 _physique;
	u8 _dexterity;
	u8 _intelligence;
	u8 _healthGain;
	u8 _powerGain;
	u8 _physiqueGain;
	u8 _dexterityGain;
	u8 _intelligenceGain;
};

} // namespace Shared

typedef Shared::Class SharedClass;
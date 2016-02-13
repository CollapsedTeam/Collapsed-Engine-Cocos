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

class Class : public SharedClass
{
public:
	/**
	 * 
	 * @param id 
	 * @param name 
	 * @param startCash 
	 * @param health 
	 * @param power 
	 * @param physique 
	 * @param dexterity 
	 * @param intelligence 
	 * @param healthGain
	 * @param powerGain 
	 * @param physiqueGain 
	 * @param dexterityGain 
	 * @param intelligenceGain 
	 */
	Class(u32 id, const String& name, u16 startCash, u8 health, u8 power, u8 physique, 
		  u8 dexterity, u8 intelligence, u8 healthGain, u8 powerGain, u8 physiqueGain, 
		  u8 dexterityGain, u8 intelligenceGain); 
	
	/**
	 * 
	 * @return 
	 */
	u16 getStartCash() const {
		return _startCash;
	}

protected:
	u16 _startCash;
};

OOLUA_PROXY(SharedClass)
OOLUA_TAGS(OOLUA::No_default_constructor)
OOLUA_MFUNC_CONST(getID)
OOLUA_MFUNC_CONST(getName)
OOLUA_MFUNC_CONST(getHealth)
OOLUA_MFUNC_CONST(getPower)
OOLUA_MFUNC_CONST(getPhysique)
OOLUA_MFUNC_CONST(getDexterity)
OOLUA_MFUNC_CONST(getIntelligence)
OOLUA_MFUNC_CONST(getHealthGain)
OOLUA_MFUNC_CONST(getPowerGain)
OOLUA_MFUNC_CONST(getPhysiqueGain)
OOLUA_MFUNC_CONST(getDexterityGain)
OOLUA_MFUNC_CONST(getIntelligenceGain)
OOLUA_PROXY_END

OOLUA_PROXY(Class, SharedClass)
OOLUA_TAGS(OOLUA::No_default_constructor)
OOLUA_MFUNC_CONST(getStartCash)
OOLUA_PROXY_END
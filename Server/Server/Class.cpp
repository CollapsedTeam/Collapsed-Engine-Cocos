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

#include "Main.h"
#include "Class.h"

OOLUA_EXPORT_FUNCTIONS_CONST(SharedClass, getID, getName, getHealth, getPower, getPhysique, 
							 getDexterity, getIntelligence, getHealthGain, getPowerGain, 
							 getPhysiqueGain, getDexterityGain, getIntelligenceGain);
OOLUA_EXPORT_FUNCTIONS(SharedClass);

OOLUA_EXPORT_FUNCTIONS_CONST(Class, getStartCash);
OOLUA_EXPORT_FUNCTIONS(Class);

Class::Class(u32 id, const String& name, u16 startCash, u8 health, u8 power, 
			 u8 physique, u8 dexterity, u8 intelligence, u8 healthGain, u8 powerGain, 
			 u8 physiqueGain, u8 dexterityGain, u8 intelligenceGain)
	: SharedClass(id, name, health, power, physique, dexterity, intelligence, healthGain, 
	powerGain, physiqueGain, dexterityGain, intelligenceGain), _startCash(startCash)
{ }
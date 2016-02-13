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

class Character : public Shared::Character, public Ref
{ 
public:
	Character(u32 id, const SString& name, u16 level, bool male, u32 cash, u16 health, 
			  u16 power, u16 physique, u16 dexterity, u16 intelligence);
	
	virtual ~Character();

	Shared::Guild* getGuild() const {
		return _guild;
	}

	void setGuild(Shared::Guild* guild);

protected:
	Shared::Guild* _guild;
};
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
#include "Account.h"
#include "Server.h"

Account::Account(const String& email)
	: _id(0), _email(email), _type(Type::NORMAL), _character(nullptr)
{ }

Account::~Account() {
	deleteCharacter();
}

void Account::deleteCharacter()
{
	// Do nothing when player didn't have character (exit while is in character creation screen).
	if (_character == nullptr)
		return;

	server->removeCharacter(_character->getID());
	_character = nullptr;
}
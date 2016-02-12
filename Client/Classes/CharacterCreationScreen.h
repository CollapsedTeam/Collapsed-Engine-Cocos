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

class CharacterCreationScreen : public Screen
{
public:
	CREATE_FUNC(CharacterCreationScreen);

	static Scene* createScene(std::vector<Shared::Class>* classes);

	void setLanguage() override;

private:
	bool init() override;

	void onEnter() override;
	
	void addClickEventToOptionsButton();

	void addClickEventToMaleCheckBox();

	void addClickEventToFemaleCheckBox();

	void addClickEventToPreviousClassButton();

	void addClickEventToNextClassButton();

	void addClickEventToCreateButton();

	void update(float delta) override;

	void updateClassInfo();

	void createCharacter(const SString& name, bool male);

	void sendCharacterCreatePacket(const SString& name, bool male);

private:
	std::vector<Shared::Class> _classes;
	std::vector<Shared::Class>::iterator _currentClass;
};

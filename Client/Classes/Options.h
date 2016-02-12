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

#include "DragWindow.h"

class Options : public DragWindow
{
public:
	void setLanguage() override;

	void setEmail(const SString& email);

private:
	void onLoad(Node* scene) override;

	void reset();

	void loadResolutions();

	void updateResolution();

	void loadLanguages();

	void updateLanguage();

	void updateMusic();

	void updateSFX();

	void addClickEventToPreviousResolutionButton();

	void addClickEventToNextResolutionButton();

	void addClickEventToPreviousLanguageButton();

	void addClickEventToNextLanguageButton();

	void addEventToMusicSlider();

	void addEventToSFXSlider();

	void addClickEventToDeleteAccountButton();

	void addClickEventToApplyChangesButton();

	void addClickEventToCancelChangesButton();

	void confirmAccountData();

	void sendChangeAccountDataPacket(const SString& email, const SString& password);
	
	void sendDeleteAccountPacket();

public:
	typedef std::pair<SString, SString> LangNameAndLocale;

private:
	std::vector<Size>::iterator _resolutionIt;
	static std::vector<Size> _resolutions;

	std::vector<LangNameAndLocale>::iterator _languageIt;
	static std::vector<LangNameAndLocale> _languages;
};

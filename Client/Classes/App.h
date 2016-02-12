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

class App : private Application
{
public:
    void initGLContextAttrs() override;

	bool applicationDidFinishLaunching() override;

	void applicationDidEnterBackground() override;

	void applicationWillEnterForeground() override;

	void saveConfigFile();

	void exit();

	inline const Size& getResolution() {
		return _resolution;
	}

	inline bool getDisplayStats() const {
		return _displayStats;
	}

	inline Lang* getLang() {
		return &_lang;
	}

	inline u8 getMusicVolume() const {
		return _musicVolume;
	}

	inline u8 getSFXVolume() const {
		return _sfxVolume;
	}

	inline Node* getSceneFromRunningScreen() {
		return Director::getInstance()->getRunningScene()->getChildren().at(
			1)->getChildByName("Scene");
	}

	template<typename ScreenType>
	inline ScreenType* getCurrentScreen() {
		return static_cast<ScreenType*>(
			Director::getInstance()->getRunningScene()->getChildren().at(1));
	}

	void setResolution(const Size& resolution);

	void setLanguage(const SString& locale);

	void setDisplayStats(bool displayStats);

	void setMusicVolume(u8 musicVolume);

	void setSFXVolume(u8 sfxVolume);

private:
	void createConfigFile();

	void loadConfigFile();

	void setupView();

	void runFirstScene();

private:
	Size _resolution;
	bool _displayStats;
	Lang _lang;
	u8 _musicVolume;
	u8 _sfxVolume;
};




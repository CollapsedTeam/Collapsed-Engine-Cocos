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

class LoadingScreen : public Screen
{
public:
	CREATE_FUNC(LoadingScreen);

    static Scene* createScene();

	void setLanguage() override;

	void loadAfterConnection();

	void setInfo(const SString& text) {
		GET_CHILD(getChildByName("Scene"), "lblLoading", Text)->setString(text);
	}

private:
	bool init() override;

	void onEnter() override;

	void update(float delta) override;

	void startLoading();

	void loadMessageBox();

	void connectToServer();

	void loadOptions();

	void loadChat();

	void loadStatistics();

	void loadGuild();

	void loadDialogue();

	void finishLoading();
};

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

class AccountScreen : public Screen
{
public:
	CREATE_FUNC(AccountScreen);

	static Scene* createScene();

	void setLanguage() override;

private:
	bool init() override;

	void addClickEventToOptionsButton();

	void addClickEventToLoginButton();

	void addClickEventToCreateAccountButton();
	
	void addClickEventToClearFieldsButton();

	void addClickEventToGenerateButton();

	void update(float delta) override;

	void login(const SString& email, const SString& password);

	void createAccount(const SString& email, const SString& reemail, 
					   const SString& password, const SString& repassword);

	bool validateEmailAndPassword(const SString& email, const SString& password);
	
	bool validateEmailWithReEmail(const SString& email, const SString& reemail);

	bool validatePasswordWithRePassword(const SString& password, const SString& repassword);

	void sendCreateAccount(const SString& email, const SString& password);

};


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

class MsgBox : public Ref
{
	friend class LoadingScreen;

public:
	static RefPtr<MsgBox> create();

	static RefPtr<MsgBox> create(const SString& text, const SString& textForLeftButton,
								 const std::function<void(Ref*)>& funForLeftButton,
								 const SString& textForMiddleButton,
								 const std::function<void(Ref*)>& funForMiddleButton,
								 const SString& textForRightButton,
								 const std::function<void(Ref*)>& funForRightButton);

	void init(const SString& text, const SString& textForLeftButton, 
			  const std::function<void(Ref*)>& funForLeftButton,
			  const SString& textForMiddleButton,
			  const std::function<void(Ref*)>& funForMiddleButton,
			  const SString& textForRightButton,
			  const std::function<void(Ref*)>& funForRightButton);

	void show(Node* scene = nullptr, Vec2 position = centerPoint);

	void hide();

	void setText(const SString& text);

	void setTextOfLeftButton(const SString& text);

	void setTextOfMiddleButton(const SString& text);

	void setTextOfRightButton(const SString& text);

	void setClickEventOfLeftButton(const std::function<void(Ref*)>& function) {
		GET_CHILD(_main, "btnL", Button)->addClickEventListener(function);
	}

	void setClickEventOfMiddleButton(const std::function<void(Ref*)>& function) {
		GET_CHILD(_main, "btnM", Button)->addClickEventListener(function);
	}

	void setClickEventOfRightButton(const std::function<void(Ref*)>& function) {
		GET_CHILD(_main, "btnR", Button)->addClickEventListener(function);
	}

private:
	MsgBox();

	static void load(Node* scene);

private:
	ImageView* _main;

	static ImageView* _template;
};

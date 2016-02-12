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

class Screen : public Layer
{
public:
	Screen();

	inline Scene* getScene() const override {
		return _scene;
	}

	virtual void setLanguage() = 0;

protected:
	inline void setScene(Node* scene){
		_scene = static_cast<Scene*>(scene);
	}

protected:
	Scene* _scene;
};


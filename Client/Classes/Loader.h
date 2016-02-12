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

class Loader
{
public:
	static Node* loadScene(const SString& fileName);

	static bool loadXMLFile(tinyxml2::XMLDocument* document, const SString& fileName);

	static bool loadJSONFile(rapidjson::Document* document, const SString& fileName);

private:
	static void handleXMLError(tinyxml2::XMLError xmlError);

	static void handleJSONError(rapidjson::ParseErrorCode jsonError);
};


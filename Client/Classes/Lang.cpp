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

bool Lang::load(cchar* locale)
{
	// We are not interested in nullptr pointer.
	if (locale == nullptr)
		return false;

	_locale = locale;
	if (!Loader::loadXMLFile(&_file, StringUtils::format("Languages/%s.xml", locale))) {
		// Let's try again, but with English setup.
		_locale = "en_EN";
		if (!Loader::loadXMLFile(&_file, "Languages/en_EN.xml")) {
			// Something goes totally wrong.
			_locale = "ERROR";
			return false;
		}
	} 
	return true;
}

tinyxml2::XMLElement* Lang::getSetElement(cchar* setName)
{
	// For every element ...
	for (auto element = _file.RootElement()->FirstChildElement("set"); 
		 element != nullptr; element = element->NextSiblingElement()) {
		// ... check if its name is equal to name of set.
		if (element->Attribute("id", setName) != nullptr)
			return element;
	}

	// Not found.
	return nullptr;
}

SString Lang::getText(tinyxml2::XMLElement* setElement, cchar* id)
{
	// We are not interested in nullptr pointer.
	if (setElement == nullptr)
		return "[SET_ELEMENT_IS_NULL]";

	// For every element in set ...
	for (auto element = setElement->FirstChildElement(); element != nullptr; 
		 element = element->NextSiblingElement()) {
		// ... check if its id attribute is equal to searched id.
		if (element->Attribute("id", id) != nullptr) {
			SString msg = element->Attribute("msg");
			parseNewLine(msg);
			return msg.c_str();
		}
	}

	// Not found.
	return "[TEXT_ID_NOT_FOUND]";
}

void Lang::parseNewLine(SString& string)
{
	SString replace; replace += '\n';
	u32 pos = 0u;
	
	// Every "\n" ... 
	for (; (pos = string.find("\\n", pos)) != SString::npos; ++pos) {
		// ... replace with '\n'.
		string.replace(pos, 2, replace);
	}
}

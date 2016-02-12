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

class Error : public Warn 
{
public:
	/**
	 * 
	 */
	Error() = default;

	/**
	 *
	 * @param what
	 */
	Error(const String& what) 
		: Warn(what.c_str())
	{ }

	/**
	 * 
	 * @param fileName
	 * @param function
	 * @param line 
	 * @param what
	 */
	Error(const String& fileName, const String& function, s32 line, cchar* what, ...)
		: Warn(fileName, function, line) 
	{ 
		va_list list;
		va_start(list, what);
		setWhat(what, list);
		va_end(list);
	}
};

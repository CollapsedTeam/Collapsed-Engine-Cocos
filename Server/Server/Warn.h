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

class Warn : public Info
{
public:
	/**
	 * 
	 */
	Warn() = default;

	/**
	 * 
	 * @param what
	 */
	Warn(const String& what)
		: Info(what.c_str()), _line(-1)
	{ }

	/**
	 * 
	 * @param fileName
	 * @param function
	 * @param line
	 */
	Warn(const String& fileName, const String& function, s32 line)
		: _line(line), _fileName(fileName), _function(function)
	{ }

	/**
	 * 
	 * @param fileName
	 * @param function
	 * @param line
	 * @param what
	 */
	Warn(const String& fileName, const String& function, s32 line, cchar* what, ...)
		: _line(line), _fileName(fileName), _function(function)
	{ 
		va_list list;
		va_start(list, what);
		setWhat(what, list);
		va_end(list);
	}

	/**
	 *
	 * @return
	 */
	inline s32 getLine() const throw() { 
		return _line; 
	}

	/**
	 *
	 * @return
	 */
	inline cchar* getFileName() const throw() {
		return _fileName.c_str();
	}

	/**
	 * 
	 * @return
	 */
	inline cchar* getFunction() const throw() {
		return _function.c_str();
	}

protected:
	String _fileName;
	String _function;
	s32 _line;
};

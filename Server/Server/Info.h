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

class Info
{
public:
	/**
	 * 
	 */
	Info() = default;

	/**
	 * 
	 * @param what 
	 */
	Info(cchar* what, ...) {
		va_list list;
		va_start(list, what);
		setWhat(what, list);
		va_end(list);
	}

	/**
	 * 
	 */
	virtual ~Info() throw() { };

	/**
	 *
	 * @param what
	 */
	void setWhat(cchar* what, ...) throw() {
		char buffer[512];
		va_list list;
		va_start(list, what);
		vsprintf(buffer, what, list);
		va_end(list);
		_what = buffer;
	}

	/**
	 * 
	 * @return 
	 */
	cchar* getWhat() const throw() {
		return _what.c_str(); 
	}

protected:
	/**
	 * 
	 * @param what
	 */
	void setWhat(cchar* what, va_list list) throw() {
		char buffer[512];
		vsprintf(buffer, what, list);
		_what = buffer;
	}

protected:
	String _what;
};

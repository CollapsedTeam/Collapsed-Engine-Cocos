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

#include <fstream>

class FileLogger 
{
public:
	FileLogger();

	void open(cchar* fileName);

	void writeDebugLine(cchar* text);

	void newLine();

	void close();

	bool isOpen() const { 
		return _file.is_open(); 
	}

public:
	const FileLogger* operator<<(cchar* text);

	const FileLogger* operator<<(const Info& info);

	const FileLogger* operator<<(const Debug& debug);

	const FileLogger* operator<<(const Warn& warn);

	const FileLogger* operator<<(const Error& error);

public:
	FileLogger(const FileLogger&) = delete;

	FileLogger& operator=(const FileLogger&) = delete;

public:
	static FileLogger instance;

private:
	std::ofstream _file;
	u32 _numWarnings;
};

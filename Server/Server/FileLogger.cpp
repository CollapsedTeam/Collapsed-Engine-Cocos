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

FileLogger FileLogger::instance;

FileLogger::FileLogger()
	: _numWarnings(0u)
{ }

void FileLogger::open(cchar* fileName) 
{
	_file.open(fileName);
	if (!isOpen()) 
		throw "Failed to create log file!";
}

void FileLogger::writeDebugLine(cchar* text) 
{
#if defined(_DEBUG)
	_file << "[DEBUG] " << text << std::endl;
#endif // defined(_DEBUG)
}

void FileLogger::newLine() { 
	_file << std::endl; 
}

void FileLogger::close() 
{
#if defined(_DEBUG)
	if (!isOpen()) {
		Console::writeLine(Warn("FileLogger", "close()", __LINE__, 
			"Cannot close file that is not open."));
		return;
	}
#endif // defined(_DEBUG)

	_file << "\n\n" << _numWarnings << " warnings" << std::endl;
	_file.close(); 
}

const FileLogger* FileLogger::operator<<(cchar *text) 
{
#if defined(_DEBUG)
	if (!isOpen()) 
		throw Error("FileLogger", "<< text", __LINE__, "File is not open!");
#endif // defined(DEBUG) || defined(_DEBUG)

	_file << text;
	return this;
}

const FileLogger* FileLogger::operator<<(const Info& info)
{
#if defined(_DEBUG)
	if (!isOpen()) 
		throw Error("FileLogger", "<< info", __LINE__, "File is not open!");
#endif // defined(DEBUG) || defined(_DEBUG)

	_file << "[INFO] " << info.getWhat() << std::endl;
	return this;
}

const FileLogger* FileLogger::operator<<(const Debug& debug)
{
#if defined(_DEBUG)
	if (!isOpen()) 
		throw Error("FileLogger", "<< debug", __LINE__, "File is not open!");

	_file << "[DEBUG] " << debug.getWhat() << std::endl;
#endif // defined(DEBUG) || defined(_DEBUG)

	return this;
}

const FileLogger* FileLogger::operator<<(const Warn& warn) 
{
#if defined(_DEBUG)
	if (!isOpen()) 
		throw Error("FileLogger", "<< warn", __LINE__, "File is not open!");
#endif // defined(_DEBUG)

	if (warn.getLine() > -1) {
		_file << "[WARN] " << warn.getFileName() << "::" << warn.getFunction() << "::" <<
			warn.getLine() << " " << warn.getWhat() << std::endl;
	} else {
		_file << "[WARN] " << warn.getWhat() << std::endl;
	}

	++_numWarnings;
	return this;
}

const FileLogger* FileLogger::operator<<(const Error& error) 
{
#if defined(_DEBUG)
	if (!isOpen()) 
		throw Error("FileLogger", "<< error", __LINE__, "File is not open!");
#endif // defined(_DEBUG)

	if (error.getLine() > -1) {
		_file << "[ERROR] " << error.getFileName() << "::" << error.getFunction() << "::" << 
			error.getLine() << " " << error.getWhat() << std::endl;
	} else {
		_file << "[ERROR] " << error.getWhat() << std::endl;
	}

	return this;
}

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
#include "Script.h"

void writeInfoLine(cchar* text) { Console::writeLine(Info(text)); }
OOLUA_CFUNC(writeInfoLine, iWriteInfoLine)

void writeDebugLine(cchar* text) { Console::writeLine(Debug(text)); }
OOLUA_CFUNC(writeDebugLine, iWriteDebugLine)

void writeWarnLine(cchar* text) { Console::writeLine(Warn(text)); }
OOLUA_CFUNC(writeWarnLine, iWriteWarnLine)

void writeErrorLine(cchar* text) { Console::writeLine(Error(text)); }
OOLUA_CFUNC(writeErrorLine, iWriteErrorLine)

float randomFloat(float min, float max) { return Jinra::Random::generateFloat(min, max); }
OOLUA_CFUNC(randomFloat, iRandomFloat)

void Script::load(const SString &fileName)
{
	registerGlobals();
	registerClasses();

	// Opens script file.
	auto file = fopen(fileName.c_str(), "r");
	if (file == nullptr) 
		throw Error("Script", "load()", __LINE__, "File %s not found.", fileName.c_str());
	_fileName = fileName;

	// Gets file size by going to end of file then back to start.
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Reads file content to buffer.
	auto buffer = new char[fileSize + 1];
	u32 readSize = fread(buffer, sizeof(buffer[0]), fileSize, file);
	buffer[fileSize] = '\0';
	if (readSize < static_cast<u32>(fileSize))
		buffer[readSize] = '\0';

	// Closes file.
	fclose(file);
	
	// run_chunk() from OOLUA can only pass std::string, so it's better to create it now 
	// and be sure that buffer is deleted from memory.
	String chunk = buffer;
	delete[] buffer;

	runChunk(chunk);
}

void Script::collectGarbage()
{
	if (!_lua.run_chunk("collectgarbage()")) {
		throw Error("Script", "collectGarbage()", __LINE__,
					"Error while calling collectGarbage() for file %s.", _fileName.c_str());
	}
}

void Script::registerGlobals()
{
	OOLUA::set_global(_lua, "writeInfoLine", iWriteInfoLine);
	OOLUA::set_global(_lua, "writeDebugLine", iWriteDebugLine);
	OOLUA::set_global(_lua, "writeWarnLine", iWriteWarnLine);
	OOLUA::set_global(_lua, "writeErrorLine", iWriteErrorLine);
	OOLUA::set_global(_lua, "randomf", iRandomFloat);
}

void Script::registerClasses() { }

void Script::runChunk(const String& chunk)
{
	if (!_lua.run_chunk(chunk)) {
		throw Error("Script", "runChunk()", __LINE__,
					"Error while parsing file %s. Be sure that lua syntax is correct",
					_fileName.c_str());
	}
}

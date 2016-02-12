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
#include "Server.h"

bool Console::_quitRequest = false;
String Console::_command;

#if !defined(_WIN32)
#include <termios.h>

struct termios initialSettings;
s32 peekCharacter;
#endif // !defined(_WIN32)

void Console::init() 
{
	// Some basic console settings for linux platform.
#if !defined(_WIN32)
	struct termios newSettings;
	tcgetattr(0, &initialSettings);
	newSettings = initialSettings;
//	newSettings.c_iflag &= ~(ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
	newSettings.c_lflag &= ~(ISIG);
	newSettings.c_cc[VMIN] = 1;
	newSettings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &newSettings);
	peekCharacter = -1;
#endif // !defined(_WIN32)

	disableCloseButton();
}

void Console::writeLine(const Info& info)
{
	writeLine("[INFO] %s", info.getWhat()); 

	if (FileLogger::instance.isOpen())
		FileLogger::instance << info;
}

void Console::writeLine(const Debug& debug)
{
#if defined(_DEBUG)
	writeLine("[DEBUG] %s", debug.getWhat());

	if (FileLogger::instance.isOpen())
		FileLogger::instance << debug;
#endif // defined(_DEBUG)
}

void Console::writeLine(const Warn& warn) 
{
	if (warn.getLine() > -1) {
		writeLine("[WARN] %s::%s::%d %s", warn.getFileName(), warn.getFunction(), 
				  warn.getLine(), warn.getWhat());
	} else {
		writeLine("[WARN] %s", warn.getWhat());
	}
	
	if (FileLogger::instance.isOpen())
		FileLogger::instance << warn;
}

void Console::writeLine(const Error& error) 
{
	if (error.getLine() > -1) {
		writeLine("[ERROR] %s::%s::%d %s", error.getFileName(), error.getFunction(), 
				  error.getLine(), error.getWhat());
	} else {
		writeLine("[ERROR] %s", error.getWhat());
	}

	if (FileLogger::instance.isOpen())
		FileLogger::instance << error;
}

void Console::newLine() { 
	printf("\n"); 
}

void Console::inputUpdate()
{
	if (kbhit()) {
		s32 pressedKey = getch();
		switch (pressedKey) {
			// ENTER
			case 10:
			case '\r':
				processEnter();
				break;

			// BACKSPACE 
			case 8: 
				processBackspace();
				break;

			// ESC
			case 27:
				requestQuit();
				break;

			default:
				processKey(pressedKey);
				break;
		}
	}
}

void Console::requestQuit() 
{ 
	if (_quitRequest) {
		writeLine(Warn("Console", "requestQuit()", __LINE__, "Quit already requested."));
		return;
	}
	_quitRequest = true;
}

void Console::release() 
{
	// Release basic console settings for linux platform.
#if !defined(_WIN32)
	tcsetattr(0, TCSANOW, &initialSettings);
#endif // !defined(_WIN32)

	pause();
}

void Console::disableCloseButton() 
{
	// It would be nice to block button on linux platforms that have UI
#if defined(_WIN32)
	auto handle = GetForegroundWindow();
	if (handle != 0) {
		auto menuHandle = GetSystemMenu(handle, 0);
		if (menuHandle != nullptr) {
			DeleteMenu(menuHandle, SC_CLOSE, MF_BYCOMMAND);
			DrawMenuBar(handle);
		}
	}
#endif
}

bool Console::kbhit() 
{
#if defined(_WIN32)
	return (_kbhit() != 0);
#else // defined(_WIN32)
	s32 i;
	ioctl(0, FIONREAD, &i);
	return (i != 0);
#endif // defined(_WIN32)
}

void Console::processEnter()
{
	if (_command.empty())
		return;

	Console::newLine();

	// 
	if (FileLogger::instance.isOpen())
		FileLogger::instance << ("[ROOT] " + _command + "\n").c_str();

	// Parses command name.
	auto commandName = _command.substr(0, _command.find_first_of(" "));

	// Parses command arguments from command string if there's any.
	String commandArgs = "";
	if (_command != commandName) {
		commandArgs = _command.substr(commandName.length() + 1, 
									  _command.length() - commandName.length());;
	}

	std::vector<String> parsedArgs;
	parseArguments(parsedArgs, commandArgs);

	// Executes command with name and parsed arguments as list. 
	executeCommand(commandName, parsedArgs);

	// Clear command string.
	_command = "";
}

void Console::processBackspace() 
{
	// Erases last character in command and moves cursor if command isn't empty.
	if (!_command.empty()) {
		_command.erase(_command.length() - 1, 1);
		printf("\b \b");
	}
}

void Console::processKey(s32 pressedKey)
{
	// On windows, character is not printed, so we need to do it manually.
#if defined(_WIN32)
	std::wstringstream stream;
	stream << static_cast<wchar_t>(pressedKey);
	wprintf(stream.str().c_str());
#endif

	// Add pressed key to command string.
	_command += pressedKey;
}

void Console::parseArguments(std::vector<String>& dest, const String& args)
{
	if (args.empty())
		return;

	String arg;

	auto restArgs = args;
	while (true) {
		u32 endArg = 0u;

		// Is first argument starts with "?
		if (restArgs[0] == '\"') {
			endArg = restArgs.find_first_of("\"", 1);
			arg = restArgs.substr(1, endArg - 1);
		} else {
			endArg = restArgs.find_first_of(" ");
			if (endArg == String::npos) {
				endArg = restArgs.length();
				if (endArg == 0u)
					break;
			}
			arg = restArgs.substr(0, endArg);
		}

		// Checks if argument string is valid then push it to std::vector. 
		if (!arg.empty() && arg != " ")
			dest.push_back(arg);

		// No more arguments?
		if (endArg + 1 >= restArgs.length() - 1)
			break;

		restArgs = restArgs.substr(endArg + 1, restArgs.length() - 1);
	}
}

void Console::pause()
{
#if defined(_WIN32)
	system("PAUSE");
#endif
}

s32 Console::getch()
{
#if defined(_WIN32)
	return _getch();
#else // defined(_WIN32)
	char ch;
	if (peekCharacter != -1) {
		ch = peekCharacter;
		peekCharacter = -1;
	} else { 
		read(0, &ch, 1);
	}
	return ch;
#endif // defined(_WIN32)
}
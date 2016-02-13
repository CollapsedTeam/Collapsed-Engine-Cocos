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

class Console 
{
public:
	/**
	 * 
	 */
	static void init();

	/**
	 * 
	 * @param text 
	 */
	static void write(cchar* text, ...) {
		va_list list;
		va_start(list, text);
		vprintf(text, list);
		va_end(list);
	}

	/**
	 * 
	 * @param text 
	 */
	static void writeLine(cchar* text, ...) {
		va_list list;
		va_start(list, text);
		vprintf(text, list);
		newLine();
		va_end(list);
	}

	/**
	 *
	 * @param info 
	 */
	static void writeLine(const Info& info);

	/**
	 *
	 * @param debug
	 */
	static void writeLine(const Debug& debug);
	
	/**
	 * 
	 * @param warn
	 */
	static void writeLine(const Warn& warn);
	
	/**
	 * 
	 * @param error
	 */
	static void writeLine(const Error& error);

	/**
	 * 
	 */
	static void newLine();

	/**
	 * 
	 */
	static void inputUpdate();

	/**
	 * 
	 * @param command
	 * @param args
	 */
	static void executeCommand(const String& command, const std::vector<String>& args);
	
	/**
	 *
	 */
	static void requestQuit();

	/**
	 *
	 */
	static void release();

	/**
	 * 
	 * @return
	 */
	static bool isQuitRequested() {
		return _quitRequest;
	}

private:
	/**
	 * 
	 * @note Closing console via close button generates memory leaks.
	 */
	static void disableCloseButton();
	
	/**
	 * Cross-platform version of kbhit(). Gets last pressed key.
	 * @return Return true if a key has been pressed. Otherwise, it returns false.
	 */
	static bool kbhit();

	/**
	 *
	 */
	static void processEnter();

	/**
	 *
	 */
	static void processBackspace();

	/**
	 * 
	 */
	static void processKey(s32 pressedKey);

	/**
	 * 
	 * @param dest
	 * @param args
	 */
	static void parseArguments(std::vector<String>& dest, const String& args);

	/**
	 * 
	 */
	static void executeStatsCommand();

	/**
	 * 
	 */
	static void executeLPSCommand();

	/**
	 * 
	 */
	static void executeOnlineCommand();

	/**
	 * 
	 */
	static void executeWhoCommand();

	/**
	 * 
	 * @param args 
	 */
	static void executeGrantSuperCommand(const std::vector<String>& args);
	
	/**
	 * 
	 * @param args
	 */
	static void executeKickCommand(const std::vector<String>& args);
	
	/**
	 * 
	 * @param args 
	 */
	static void executeBanIPCommand(const std::vector<String>& args);
	
	/**
	 *
	 * @param args
	 */
	static void executeUnbanIPCommand(const std::vector<String>& args);
	
	/**
	 * 
	 * @param args
	 */
	static void executeBanAccountCommand(const std::vector<String>& args);
	
	/**
	 * 
	 * @param args
	 */
	static void executeUnbanAccountCommand(const std::vector<String>& args);
	
	/**
	 * Pauses console, so it doesn't quit before we can see the output.
	 */
	static void pause();

	/**
	 * Cross-platform version of getch(). Gets last pressed key.
	 * @return Return last pressed key.
	 */
	static s32 getch();

private:
	static bool _quitRequest;
	static String _command;
};

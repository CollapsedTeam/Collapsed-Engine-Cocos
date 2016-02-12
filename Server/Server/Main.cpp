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
#include "MySQL.h"
#include "Server.h"

s32 exit()
{
	Console::writeLine(Info("Releasing clients..."));
	server->releaseClients();

	Console::writeLine(Info("Releasing remainings of guilds..."));
	server->releaseGuilds();

	Console::writeLine(Info("Releasing classes..."));
	server->releaseClasses();

	Console::writeLine(Info("Releasing dialogues..."));
	server->releaseDialogues();

	Console::writeLine(Info("Shutdowning socket..."));
	server->shutdown(2500u);

	Console::writeLine(Info("Closing connection with MySQL database..."));
	MySQL::closeConnection();

	Console::writeLine(Info("Releasing console settings..."));
	Console::release();

	Console::writeLine(Info("Closing file logger..."));
	FileLogger::instance.close();

	delete server;
	return 0;
}

s32 main()
{
	Console::writeLine("-= Collapsed Engine Server =-");
	try {
		server = new Server();
		server->initialize();
		Console::writeLine(Info("Press escape or type 'exit' or 'quit' command to close "
			"server without memory leaks!"));
		server->mainLoop();
	} catch (const Error& error) {
		Console::writeLine(error);
	} catch (cchar* ex) {
		Console::writeLine(Error(ex));
	} catch (const soci::mysql_soci_error& ex) {
		Error error;
		error.setWhat("MySQL error %u - %s", ex.err_num_, ex.what());
		Console::writeLine(error);
	} catch (const std::system_error& ex) {
		Console::writeLine(Error(ex.what()));
	} 
#if !(defined(DEBUG) || defined(_DEBUG))
	catch (const std::bad_cast& ex) {
		Console::writeLine(Error(ex.what()));
	} catch (const std::out_of_range& ex) {
		Console::writeLine(Error(ex.what()));
	} catch (...) {
		Console::writeLine(Error("Unknown error occurred!"));
	}
#endif
	return exit();
}
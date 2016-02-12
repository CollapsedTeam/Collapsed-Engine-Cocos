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

class Client;

class Module
{
public:
	/**
	 * Handles unknown packet.
	 * @param packet Packet with data.
	 */
	static void unknown(Packet* packet);

public:
	/**
	 * 
	 * @param guid Client's connection ID.
	 * @param msg Message's ID.
	 */
	static void sendMessagePacket(const NetGUID& guid, Shared::Message msg);

protected:
	static bool isNotNullAndIsPlaying(Client* client);

	static bool isNotNullAndIsPlayingAndInGuild(Client* client);

	static bool isNotNullAndIsPlayingAndNotInGuild(Client* client);
};

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

class Guild;
class GuildMember;

class GuildModule : public Module
{
public: /* Network Handle */
	/**
	 * 
	 * @param packet
	 * @param bitStream
	 */
	static void createGuild(Packet* packet, BitStream* bitStream);
	
	/**
	 * 
	 * @param packet
	 * @param bitStream
	 */
	static void setGuildDescription(Packet* packet, BitStream* bitStream);

	/**
	 *
	 * @param packet
	 * @param bitStream
	 */
	static void setGuildRankName(Packet* packet, BitStream* bitStream);

	/**
	 *
	 * @param packet
	 * @param bitStream
	 */
	static void inviteCharacterToGuild(Packet* packet, BitStream* bitStream);
	
	/**
	 *
	 * @param packet 
	 * @param bitStream 
	 */
	static void guildInvitationAnswer(Packet* packet, BitStream* bitStream);

	/**
	 * 
	 * @param packet 
	 * @param bitStream
	 */
	static void setGuildNote(Packet* packet, BitStream* bitStream);
	
	/**
	 * 
	 * @param packet
	 * @param bitStream
	 */
	static void promoteGuildMember(Packet* packet, BitStream* bitStream);

	/**
	 * 
	 * @param packet
	 * @param bitStream 
	 */
	static void degradeGuildMember(Packet* packet, BitStream* bitStream);

	/**
	 * 
	 * @param packet 
	 * @param bitStream 
	 */
	static void kickGuildMember(Packet* packet, BitStream* bitStream);

	/**
	 * 
	 * @param packet 
	 * @param bitStream 
	 */
	static void leaveGuild(Packet* packet, BitStream* bitStream);

public: /* Database */
	static void deleteGuild(Guild* guild);

	static void insertGuildMember(Guild* guild, GuildMember* guildMember);

	static void setGuildDescription(Guild* guild);

	static void setGuildRanksNames(Guild* guild);

	static void setGuildMemberNote(GuildMember* member);

	static void setGuildMemberRank(GuildMember* member);

	static void deleteGuildMember(GuildMember* guildMember);

	static void deleteAllGuildMembers(Guild* guild);

	static bool isGuildNameExists(const String& guildName);

	static bool isGuildTagExists(const String& guildTag);
};

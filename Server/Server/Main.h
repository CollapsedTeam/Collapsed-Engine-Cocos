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

#include <Jinra/Jinra.h>
#include <Jinra/BitStream.h>
#include <Jinra/NetServer.h>
#include <Jinra/NetStatistics.h>
#include <Jinra/MessageIdentifiers.h>
#include <Jinra/Random.h>
#include <Jinra/Sleep.h>
#include <Jinra/GetTime.h>

#include <soci.h>
#include <mysql/soci-mysql.h>

#if defined(_WIN32)
#include <conio.h>
#if defined(_MSC_VER)
#include <vld.h>
#endif // defined(_MSC_VER)
#endif // defined(_WIN32)

#include <SimpleIni.h>
#include <oolua/oolua.h>

#include <Typedefs.h>

#include <SharedGlobal.h>
#include <SharedClass.h>
#include <SharedCharacter.h>
#include <SharedGuild.h>

#include "Config.h"
#include "Utils.h"
#include "Point.h"
#include "Info.h"
#include "Warn.h"
#include "Debug.h"
#include "Error.h"
#include "Console.h"
#include "FileLogger.h"
#include "Module.h"

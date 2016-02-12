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
#include "QueriesQueue.h"
#include "MySQL.h"

void QueriesQueue::addQueryData(const QueryData& queryData)
{
	// We don't want to have multiple queries that do exactly the same thing 
	// with the same data.
	if (std::find(_queriesData.begin(), _queriesData.end(), queryData) != _queriesData.end())
		return;
	_queriesData.push_back(queryData);
}

void QueriesQueue::flush(u32 from)
{
	for (u32 i = from; i < _queriesData.size(); ++i) {
		MySQL::doQueryData(_queriesData[i]);
		_queriesData.erase(_queriesData.begin() + i);
	}
}

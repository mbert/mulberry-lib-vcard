/*
    Copyright (c) 2007 Cyrus Daboo. All rights reserved.
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
	CVCardAttribute.cpp

	Author:
	Description:	<describe the CVCardAttribute class here>
*/

#include "CVCardAttribute.h"

#include <iostream>

using namespace vCard;

bool CVCardAttribute::ContainsValue(const cdstring& value, bool casei) const
{
	for(cdstrvect::const_iterator iter = mValues.begin(); iter != mValues.end(); iter++)
	{
		if ((*iter).compare(value, casei) == 0)
			return true;
	}
	
	return false;
}

void CVCardAttribute::Generate(std::ostream& os) const
{
	os << mName << "=";

	for(cdstrvect::const_iterator iter = mValues.begin(); iter != mValues.end(); iter++)
	{
		if (iter != mValues.begin())
			os << ',';

		// Look for quoting
		if ((*iter).find_first_of(":;,") != cdstring::npos)
			os << '"' << *iter << '"';
		else
			os << *iter;
	}
}

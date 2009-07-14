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
	CVCardN.cpp

	Author:
	Description:	<describe the CVCardN class here>
*/

#include "CVCardN.h"

#include "CVCardUtils.h"

using namespace vCard;

void CVCardN::_copy_CVCardN(const CVCardN& copy)
{
	for(int i = eLast; i < eMaxIndex; i++)
		mItems[i] = copy.mItems[i];
}

cdstring CVCardN::GetFullName(bool with_titles) const
{
	cdstring result;

	if (with_titles)
		AppendFullNameItem(result, mItems[ePrefix]);
	
	AppendFullNameItem(result, mItems[eFirst]);
	AppendFullNameItem(result, mItems[eMiddle]);
	AppendFullNameItem(result, mItems[eLast]);

	if (with_titles)
		AppendFullNameItem(result, mItems[eSuffix]);
	
	return result;
}

void CVCardN::AppendFullNameItem(cdstring& result, const cdstring& item) const
{
	if (!item.empty())
	{
		if (!result.empty())
			result += " ";
		result += item;
	}
}

// Assume name of format [first] *(middle) last
void CVCardN::SetFullName(const cdstring& data)
{
	// Look for first name
	cdstring::size_type start = 0;
	cdstring::size_type end = data.find(' ');
	if (end != cdstring::npos)
	{
		// Grab first name
		mItems[eFirst].assign(data, start, end - start);
		start = end + 1;
		
		// Look for last name portion
		end = data.find_last_of(' ', start);
		if (end > start)
		{
			mItems[eMiddle].assign(data, start, end - start);
			end++;
		}
		else
			end = start;
		
		mItems[eLast].assign(data, end, cdstring::npos);
	}
	else
		mItems[eLast] = data;
}

void CVCardN::Parse(const cdstring& data)
{
	// Find each unescaped ;
	const char* start = data.c_str();
	const char* end = start;
	uint32_t ctr = 0;
	while(ctr < eMaxIndex)
	{
		switch(*end)
		{
		case 0:
		case ';':
			// Grab the relevant portion of the data
			if (end > start)
			{
				cdstring result(start, end - start);
				mItems[ctr] = CVCardUtils::DecodeTextValue(result);
			}
			
			// Update items
			if (*end == 0)
			{
				ctr = eMaxIndex;
			}
			else
			{
				ctr++;
				end++;
				start = end;
			}
			break;
		case '\\':
			// Bump over next character as well
			end++;
			if (*end) end++;
			break;
		default:
			end++;
		}
	}
}

void CVCardN::Generate(std::ostream& os) const
{
	for(int i = eLast; i < eMaxIndex; i++)
	{
		if (i != 0)
			os << ";";
		CVCardUtils::WriteTextValue(os, mItems[i]);
	}
}

bool CVCardN::Search(const cdstring& pattern) const
{
	for(int i = eLast; i < eMaxIndex; i++)
	{
		if (mItems[i].PatternMatch(pattern))
			return true;
	}
	
	return false;
}

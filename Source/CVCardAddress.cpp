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
	CVCardAddress.cpp

	Author:
	Description:	<describe the CVCardAddress class here>
*/

#include "CVCardAddress.h"

#include "CVCardUtils.h"

using namespace vCard;

void CVCardAddress::_copy_CVCardAddress(const CVCardAddress& copy)
{
	for(int i = ePOBox; i < eMaxIndex; i++)
		mItems[i] = copy.mItems[i];
}

cdstring CVCardAddress::GetFullAddress() const
{
	cdstring result;

	for(int i = ePOBox; i < eMaxIndex; i++)
	{
		AppendFullAddressItem(result, mItems[i]);
	}
	
	return result;
}

void CVCardAddress::AppendFullAddressItem(cdstring& result, const cdstring& item) const
{
	if (!item.empty())
	{
		if (!result.empty())
			result += os_endl;
		result += item;
	}
}

// Assume same format as above
void CVCardAddress::SetFullAddress(const cdstring& data)
{
	// Split into list of items
	cdstrvect items;
	data.split(os_endl, items);
	
	if (items.size() == eMaxIndex)
	{
		for(int i = ePOBox; i < eMaxIndex; i++)
			mItems[i] = items[i];
	}
	else if (items.size() == eMaxIndex - 1)
	{
		for(int i = eExtended; i < eMaxIndex; i++)
			mItems[i] = items[i-1];
	}
	else
	{
		for(int i = 0; i < items.size(); i++)
			mItems[eStreet + i] = items[i];
	}
}

void CVCardAddress::Parse(const cdstring& data)
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

void CVCardAddress::Generate(std::ostream& os) const
{
	for(int i = ePOBox; i < eMaxIndex; i++)
	{
		if (i != 0)
			os << ";";
		CVCardUtils::WriteTextValue(os, mItems[i]);
	}
}

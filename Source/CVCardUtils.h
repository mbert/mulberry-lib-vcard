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
	CVCardUtils.h

	Author:
	Description:	<describe the CVCardUtils class here>
*/

#ifndef CVCardUtils_H
#define CVCardUtils_H

#include <iostream>

#include "cdstring.h"


namespace vCard {

class CVCardUtils
{
public:
	static bool	ReadFoldedLine(std::istream& is, cdstring& line1, cdstring& line2);

	static void WriteTextValue(std::ostream& os, const cdstring& value);
	static cdstring DecodeTextValue(const cdstring& value);

	// Date/time calcs
	static int32_t	DaysInMonth(const int32_t month, const int32_t year);
	static int32_t	DaysUptoMonth(const int32_t month, const int32_t year);
	static int32_t	IsLeapYear(const int32_t year);
	static int32_t	LeapDaysSince1970(const int32_t year_offset);

	static int32_t	GetLocalTimezoneOffsetSeconds();

private:
	CVCardUtils() {}
	~CVCardUtils() {}

};

}	// namespace vCard

#endif	// CVCardUtils_H

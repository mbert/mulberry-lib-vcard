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
	CVCardLocale.h

	Author:
	Description:	class that returns localised strings for days, months etc
*/

#ifndef CVCardLocale_H
#define CVCardLocale_H

#include "cdstring.h"


namespace vCard {

class CVCardLocale
{
public:
	enum EStringLength
	{
		eLong,
		eShort,
		eAbbreviated
	};
	static const cdstring& GetDay(int32_t day, EStringLength strl = eLong);		// 0..6 - Sunday - Saturday
	static const cdstring& GetMonth(int32_t month, EStringLength strl = eLong);	// 1..12 - January - December
	static bool Use24HourTime();												// Use 24 hour time display
	static bool UseDDMMDate();													// Use dd/mm order for dates

private:
	static bool s24HourTime;
	static bool sDDMMDate;

	CVCardLocale() {}
	~CVCardLocale() {}

};

}	// namespace vCard

#endif	// CVCardLocale_H

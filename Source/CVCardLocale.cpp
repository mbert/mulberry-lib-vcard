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
	CVCardLocale.cpp

	Author:
	Description:	<describe the CVCardLocale class here>
*/

#include "CVCardLocale.h"

#if defined(__MULBERRY) || defined(__MULBERRY_CONFIGURE)
#include "CXStringResources.h"
#else
#include "CStringResources.h"
#endif

using namespace vCard;

bool CVCardLocale::s24HourTime = false;
bool CVCardLocale::sDDMMDate = false;

// 0..6 - Sunday - Saturday
const cdstring& CVCardLocale::GetDay(int32_t day, EStringLength strl)
{
	const char* cLongDays[] = { "Day::Sunday::Long", "Day::Monday::Long", "Day::Tuesday::Long", "Day::Wednesday::Long", "Day::Thursday::Long", "Day::Friday::Long", "Day::Saturday::Long" };
	const char* cShortDays[] = { "Day::Sunday::Short", "Day::Monday::Short", "Day::Tuesday::Short", "Day::Wednesday::Short", "Day::Thursday::Short", "Day::Friday::Short", "Day::Saturday::Short" };
	const char* cAbbrevDays[] = { "Day::Sunday::Abbrev", "Day::Monday::Abbrev", "Day::Tuesday::Abbrev", "Day::Wednesday::Abbrev", "Day::Thursday::Abbrev", "Day::Friday::Abbrev", "Day::Saturday::Abbrev" };
	const char** cDays = NULL;

	switch(strl)
	{
	case eLong:
		cDays = cLongDays;
		break;
	case eShort:
		cDays = cShortDays;
		break;
	case eAbbreviated:
		cDays = cAbbrevDays;
		break;
	}
	return rsrc::GetString(cDays[day]);
}

// 1..12 - January - December
const cdstring& CVCardLocale::GetMonth(int32_t month, EStringLength strl)
{
	const char* cLongMonths[] = { "", "Month::January::Long", "Month::February::Long", "Month::March::Long", "Month::April::Long", "Month::May::Long", "Month::June::Long", "Month::July::Long", "Month::August::Long", "Month::September::Long", "Month::October::Long", "Month::November::Long", "Month::December::Long" };
	const char* cShortMonths[] = { "", "Month::January::Short", "Month::February::Short", "Month::March::Short", "Month::April::Short", "Month::May::Short", "Month::June::Short", "Month::July::Short", "Month::August::Short", "Month::September::Short", "Month::October::Short", "Month::November::Short", "Month::December::Short" };
	const char* cAbbrevMonths[] = { "", "Month::January::Abbrev", "Month::February::Abbrev", "Month::March::Abbrev", "Month::April::Abbrev", "Month::May::Abbrev", "Month::June::Abbrev", "Month::July::Abbrev", "Month::August::Abbrev", "Month::September::Abbrev", "Month::October::Abbrev", "Month::November::Abbrev", "Month::December::Abbrev" };
	const char** cMonths = NULL;

	switch(strl)
	{
	case eLong:
		cMonths = cLongMonths;
		break;
	case eShort:
		cMonths = cShortMonths;
		break;
	case eAbbreviated:
		cMonths = cAbbrevMonths;
		break;
	}
	return rsrc::GetString(cMonths[month]);
}

// Use 24 hour time display
bool CVCardLocale::Use24HourTime()
{
	static bool _init_done = false;
	
	if (!_init_done)
	{
#if __dest_os == __mac_os || __dest_os == __mac_os_x
#if defined(__MULBERRY) || defined(__MULBERRY_CONFIGURE)
		// Get current locale
		CFLocaleRef locale = ::CFLocaleCopyCurrent();
		if (locale != NULL)
		{
			// Get formatter for current locale
			CFDateFormatterRef formatter = ::CFDateFormatterCreate(kCFAllocatorDefault, locale, kCFDateFormatterLongStyle, kCFDateFormatterLongStyle);
			if (formatter != NULL)
			{
				CFStringRef format = ::CFDateFormatterGetFormat(formatter);
				if (format != NULL)
				{
					// Look for specific characters in string
					CFRange found = ::CFStringFind(format, CFSTR("H"), 0);
					if (found.location == kCFNotFound)
					{
						found = ::CFStringFind(format, CFSTR("K"), 0);
					}
					s24HourTime = (found.location != kCFNotFound);
				}
				
				::CFRelease(formatter);
			}
			::CFRelease(locale);
		}
#endif
#elif __dest_os == __win32_os
		TCHAR buffer[256];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, buffer, 256);
#ifdef _UNICODE
		s24HourTime = (::wcschr(buffer, 'H') != 0);
#endif
#else
		time_t date = ::time(NULL);
		struct tm* now_tm = ::localtime(&date);
		now_tm->tm_hour = 23;
		cdstring result;
		result.reserve(256);
		::strftime(result.c_str_mod(), 256,"%X", now_tm);
		s24HourTime = (result[0UL] == '2');
#endif

		_init_done = true;
	}
	
	return s24HourTime;
}

// Use dd/mm order for dates
bool CVCardLocale::UseDDMMDate()
{
	static bool _init_done_date = false;
	
	if (!_init_done_date)
	{
#if __dest_os == __mac_os || __dest_os == __mac_os_x
#if defined(__MULBERRY) || defined(__MULBERRY_CONFIGURE)
		// Get current locale
		CFLocaleRef locale = ::CFLocaleCopyCurrent();
		if (locale != NULL)
		{
			// Get formatter for current locale
			CFDateFormatterRef formatter = ::CFDateFormatterCreate(kCFAllocatorDefault, locale, kCFDateFormatterShortStyle, kCFDateFormatterShortStyle);
			if (formatter != NULL)
			{
				CFStringRef format = ::CFDateFormatterGetFormat(formatter);
				if (format != NULL)
				{
					// Look for specific characters in string
					sDDMMDate = ::CFStringHasPrefix(format, CFSTR("d"));
				}
				
				::CFRelease(formatter);
			}
			::CFRelease(locale);
		}
#endif
#elif __dest_os == __win32_os
		TCHAR buffer[256];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDATE, buffer, 256);
#ifdef _UNICODE
		sDDMMDate = (::wcschr(buffer, '1') != 0);
#endif
#else
		time_t now = ::time(NULL);
		struct tm* now_tm = ::localtime(&now);
		now_tm->tm_mon = 0;
		now_tm->tm_mday = 31;
		cdstring result;
		result.reserve(256);
		::strftime(result.c_str_mod(), 256, "%x", now_tm);

		// If first character is a digit assuming day is first in this lcoale
		sDDMMDate = (result[0UL] == '3');
#endif

		_init_done_date = true;
	}
	
	return sDDMMDate;
}

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
	CVCardDateTime.cpp

	Author:
	Description:	<describe the CVCardDateTime class here>
*/

#include "CVCardDateTime.h"

#include "CVCardLocale.h"
#include "CVCardUtils.h"

#include <ctime>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace vCard;

void CVCardDateTime::_init_CVCardDateTime()
{
	mYear = 1970;
	mMonth = 1;
	mDay = 1;

	mHours = 0;
	mMinutes = 0;
	mSeconds = 0;

	mTimezoneOffset = 0;

	mDateOnly = false;
	mTimeOnly = false;
}

void CVCardDateTime::_copy_CVCardDateTime(const CVCardDateTime& copy)
{
	mYear = copy.mYear;
	mMonth = copy.mMonth;
	mDay = copy.mDay;

	mHours = copy.mHours;
	mMinutes = copy.mMinutes;
	mSeconds = copy.mSeconds;

	mTimezoneOffset = copy.mTimezoneOffset;

	mDateOnly = copy.mDateOnly;
	mTimeOnly = copy.mTimeOnly;
}

int CVCardDateTime::CompareDateTime(const CVCardDateTime& comp) const
{
	// If either are date only, then just do date compare
	if (mDateOnly || comp.mDateOnly)
	{
		if (mYear == comp.mYear)
		{
			if (mMonth == comp.mMonth)
			{
				if (mDay == comp.mDay)
					return 0;
				else
					return mDay < comp.mDay ? -1 : 1;
			}
			else
				return mMonth < comp.mMonth ? -1 : 1;
		}
		else
			return mYear < comp.mYear ? -1 : 1;
	}
	
	// If they have the same timezone do simple compare - no posix calc needed
	else if (mTimezoneOffset == mTimezoneOffset)
	{
		if (mYear == comp.mYear)
		{
			if (mMonth == comp.mMonth)
			{
				if (mDay == comp.mDay)
				{
					if (mHours == comp.mHours)
					{
						if (mMinutes == comp.mMinutes)
						{
							if (mSeconds == comp.mSeconds)
							{
								return 0;
							}
							else
								return mSeconds < comp.mSeconds ? -1 : 1;
						}
						else
							return mMinutes < comp.mMinutes ? -1 : 1;
					}
					else
						return mHours < comp.mHours ? -1 : 1;
				}
				else
					return mDay < comp.mDay ? -1 : 1;
			}
			else
				return mMonth < comp.mMonth ? -1 : 1;
		}
		else
			return mYear < comp.mYear ? -1 : 1;
	}
	else
	{
		int64_t posix1 = GetPosixTime();
		int64_t posix2 = comp.GetPosixTime();
		if (posix1 == posix2)
			return 0;
		else
			return posix1 < posix2 ? -1 : 1;
	}
}

int64_t CVCardDateTime::GetPosixTime() const
{
	int64_t result = 0;

	// Add hour/mins/secs
	result = (mHours * 60LL + mMinutes) * 60LL + mSeconds;

	// Number of days since 1970
	result += DaysSince1970() * 24LL * 60LL * 60LL;

	// Adjust for timezone offset
	result -= mTimezoneOffset;

	return result;
}

int32_t	CVCardDateTime::DaysSince1970() const
{
	// Add days betweenn 1970 and current year (ignoring leap days)
	int32_t result = (mYear - 1970) * 365L;

	// Add leap days between years
	result += CVCardUtils::LeapDaysSince1970(mYear - 1970);
	
	// Add days in current year up to current month (includes leap day for current year as needed)
	result += CVCardUtils::DaysUptoMonth(mMonth, mYear);
	
	// Add days in month
	result += mDay - 1;
	
	return result;
}

// Get current date (in local timezone)
CVCardDateTime CVCardDateTime::GetToday()
{
	// Get from posix time
	time_t now = time(NULL);
	struct tm* now_tm = std::localtime(&now);
	
	return CVCardDateTime(now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday);
}

// Get current date-time (in local timezone)
CVCardDateTime CVCardDateTime::GetNow()
{
	// Get from posix time
	time_t now = time(NULL);
	struct tm* now_tm = std::localtime(&now);
	
	return CVCardDateTime(now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday, now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);
}

// Get current date-time (in UTC)
CVCardDateTime CVCardDateTime::GetNowUTC()
{
	// Get from posix time
	time_t now = time(NULL);
	struct tm* now_tm = std::gmtime(&now);
	
	return CVCardDateTime(now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday, now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);
}

cdstring CVCardDateTime::GetLocaleDate(ELocaleDate locale) const
{
	// Determine whether to use mm/dd or dd/mm format
	bool ddmm = CVCardLocale::UseDDMMDate();

	struct tm stm;
	std::memset(&stm, 0, sizeof(struct tm));
	stm.tm_year = GetYear() - 1900;
	stm.tm_mon = GetMonth() - 1;
	stm.tm_mday = GetDay();
	stm.tm_wday = 0;

	char buf[1024];
	switch(locale)
	{
	case eFullDate:
		std::strftime(buf, 1024, "%A, %B %d, %Y", &stm);
		break;
	case eAbbrevDate:
		std::strftime(buf, 1024, "%a, %b %d, %Y", &stm);
		break;
	case eNumericDate:
		std::strftime(buf, 1024, ddmm ? "%d/%m/%Y" : "%m/%d/%Y", &stm);
		break;
	case eFullDateNoYear:
		std::strftime(buf, 1024, "%A, %B %d", &stm);
		break;
	case eAbbrevDateNoYear:
		std::strftime(buf, 1024, "%a, %b %d", &stm);
		break;
	case eNumericDateNoYear:
		std::strftime(buf, 1024, ddmm ? "%d/%m" : "%m/%d", &stm);
		break;
	}
	
	return buf;
}

cdstring CVCardDateTime::GetTime(bool with_seconds, bool am_pm, bool tzid) const
{
	cdstring buf;
	buf.reserve(32);
	int32_t adjusted_hour = mHours;

	if (am_pm)
	{
		bool am = true;
		if (adjusted_hour >= 12)
		{
			adjusted_hour -= 12;
			am = false;
		}
		if (adjusted_hour == 0)
			adjusted_hour = 12;
		
		if (with_seconds)
			::snprintf(buf, 32, "%ld:%02ld:%02ld", adjusted_hour, mMinutes, mSeconds);
		else
			::snprintf(buf, 32, "%ld:%02ld", adjusted_hour, mMinutes);
		buf += (am ? " AM" : " PM");
	}
	else
	{
		if (with_seconds)
			::snprintf(buf, 32, "%02ld:%02ld:%02ld", mHours, mMinutes, mSeconds);
		else
			::snprintf(buf, 32, "%02ld:%02ld", mHours, mMinutes);
	}
	
	if (tzid)
	{
		cdstring desc = TimeZoneDescriptor();
		if (!desc.empty())
		{
			buf += " ";
			buf += desc;
		}
	}

	return buf;
}

cdstring CVCardDateTime::GetLocaleDateTime(ELocaleDate locale, bool with_seconds, bool am_pm, bool tzid) const
{
	return GetLocaleDate(locale) + " " + GetTime(with_seconds, am_pm, tzid);
}

cdstring CVCardDateTime::GetText() const
{
	cdstring buf;
	buf.reserve(32);
	if (mDateOnly)
	{
		::snprintf(buf, 32, "%4d%02d%02d", mYear, mMonth, mDay);
	}
	else if (mTimeOnly)
	{
		::snprintf(buf, 32, "%4d%02d%02d", mHours, mMinutes, mSeconds);
		buf += TimeZoneDescriptor();
	}
	else
	{
		::snprintf(buf, 32, "%4d%02d%02dT%02d%02d%02d", mYear, mMonth, mDay, mHours, mMinutes, mSeconds);
		buf += TimeZoneDescriptor();
	}

	return buf;
}

cdstring CVCardDateTime::TimeZoneDescriptor() const
{
	if (mTimezoneOffset == 0)
		return cdstring("Z");
	else
	{
		cdstring buf;
		buf.reserve(8);
		int32_t tzoffset = mTimezoneOffset;
		if (tzoffset < 0)
			tzoffset = -tzoffset;
		int32_t tz_hours = tzoffset / 60;
		int32_t tz_mins = tzoffset % 60;
		::snprintf(buf, 8, (mTimezoneOffset > 0) ? "+%02d%02d" : "-%02d%02d", tz_hours, tz_mins);
		return buf;
	}
}

void CVCardDateTime::ParseDate(const cdstring& data)
{
	uint32_t offset = 0;
	ParseDateTxt(data, offset);
}

void CVCardDateTime::ParseTime(const cdstring& data)
{
	uint32_t offset = 0;
	ParseTimeTxt(data, offset);
}

void CVCardDateTime::ParseDateTime(const cdstring& data)
{
	uint32_t offset = 0;
	uint32_t max_offset = data.length();

	ParseDateTxt(data, offset);
	if (offset >= max_offset) return;
	if (data[offset] == 'T')
	{
		offset++;
		if (offset >= max_offset) return;
		ParseTimeTxt(data, offset);
	}
}

void CVCardDateTime::ParseDateTxt(const cdstring& data, uint32_t& offset)
{
	// parse format YYYY["-"]MM["-"]DD

	uint32_t max_offset = data.length();
	if (offset >= max_offset) return;

	// Get year
	char buf[256];
	::strncpy(buf, data.c_str() + offset, 4);
	buf[4] = 0;
	mYear = strtol(buf, NULL, 10);
	offset += 4;
	if (offset >= max_offset) return;

	// Look for delimiter
	if (data[offset] == '-')
	{
		offset++;
		if (offset >= max_offset) return;
	}

	// Get month
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	mMonth = strtol(buf, NULL, 10);
	offset += 2;
	if (offset >= max_offset) return;

	// Look for delimiter
	if (data[offset] == '-')
	{
		offset++;
		if (offset >= max_offset) return;
	}

	// Get day
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	mDay = strtol(buf, NULL, 10);
	offset += 2;
}

void CVCardDateTime::ParseTimeTxt(const cdstring& data, uint32_t& offset)
{
	// parse format HH[":"]MM[":"]SS[,n]([Z]/(("+"/"-")hh[":"]mm)]

	uint32_t max_offset = data.length();
	if (offset >= max_offset) return;

	// Get hour
	char buf[256];
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	mHours = strtol(buf, NULL, 10);
	offset += 2;
	if (offset >= max_offset) return;

	// Look for delimiter
	if (data[offset] == ':')
	{
		offset++;
		if (offset >= max_offset) return;
	}

	// Get minutes
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	mMinutes = strtol(buf, NULL, 10);
	offset += 2;
	if (offset >= max_offset) return;

	// Look for delimiter
	if (data[offset] == ':')
	{
		offset++;
		if (offset >= max_offset) return;
	}

	// Get seconds
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	mSeconds = strtol(buf, NULL, 10);
	offset += 2;
	if (offset >= max_offset) return;
	
	// Look for fractional seconds
	if (data[offset] == ',')
	{
		// Bump over number
		offset++;
		if (offset >= max_offset) return;
		while(isdigit(data[offset]))
		{
			offset++;
			if (offset >= max_offset) return;
		}
		if (offset >= max_offset) return;
	}
	
	// Look for "Z"
	if (data[offset] == 'Z')
		return;
	
	// Look for zone
	bool negative = (data[offset] == '-');
	offset++;
	if (offset >= max_offset) return;
	
	int32_t tz_hours = 0;
	int32_t tz_mins = 0;

	// Get tz hour
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	tz_hours = strtol(buf, NULL, 10);
	offset += 2;
	if (offset >= max_offset) return;

	// Look for delimiter
	if (data[offset] == ':')
	{
		offset++;
		if (offset >= max_offset) return;
	}

	// Get tz minutes
	::strncpy(buf, data.c_str() + offset, 2);
	buf[2] = 0;
	tz_mins = strtol(buf, NULL, 10);
	offset += 2;
	
	// Set timezone
	mTimezoneOffset = (negative ? -1 : 1) * ((tz_hours * 60) + tz_mins);
}

void CVCardDateTime::Generate(std::ostream& os) const
{
	os << GetText().c_str();
}

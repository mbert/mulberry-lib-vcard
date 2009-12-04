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
	CVCardDateTime.h

	Author:
	Description:	<describe the CVCardDateTime class here>
*/

#ifndef CVCardDateTime_H
#define CVCardDateTime_H

#include <stdint.h>
#include <ostream>
#include <vector>

#include "cdstring.h"


namespace vCard {

class CVCardDateTime
{
public:
	enum EDayOfWeek
	{
		eSunday,
		eMonday,
		eTuesday,
		eWednesday,
		eThursday,
		eFriday,
		eSaturday
	};

	enum ELocaleDate
	{
		eFullDate,
		eAbbrevDate,
		eNumericDate,
		
		eFullDateNoYear,
		eAbbrevDateNoYear,
		eNumericDateNoYear
	};

	CVCardDateTime()
		{ _init_CVCardDateTime(); }
	CVCardDateTime(int32_t year, int32_t month, int32_t day)
		{ _init_CVCardDateTime(); mYear = year; mMonth = month; mDay = day; mDateOnly = true; }
	CVCardDateTime(int32_t year, int32_t month, int32_t day, int32_t hours, int32_t minutes, int32_t seconds, int32_t tzoffset = 0)
		{ _init_CVCardDateTime(); mYear = year; mMonth = month; mDay = day; mHours = hours; mMinutes = minutes; mSeconds = seconds; mTimezoneOffset = tzoffset; }
	CVCardDateTime(const CVCardDateTime& copy)
		{ _copy_CVCardDateTime(copy); }
	virtual ~CVCardDateTime() {}

	CVCardDateTime& operator=(const CVCardDateTime& copy)
		{ if (this != &copy) _copy_CVCardDateTime(copy); return *this; }

	int operator==(const CVCardDateTime& comp) const
		{ return CompareDateTime(comp) == 0 ? 1 : 0; }
	int operator!=(const CVCardDateTime& comp) const
		{ return CompareDateTime(comp) != 0 ? 1 : 0; }
	int operator>=(const CVCardDateTime& comp) const
		{ return CompareDateTime(comp) >= 0 ? 1 : 0; }
	int operator<=(const CVCardDateTime& comp) const
		{ return CompareDateTime(comp) <= 0 ? 1 : 0; }
	int operator>(const CVCardDateTime& comp) const
		{ return CompareDateTime(comp) > 0 ? 1 : 0; }
	int operator<(const CVCardDateTime& comp) const
		{ return CompareDateTime(comp) < 0 ? 1 : 0; }

	int CompareDateTime(const CVCardDateTime& comp) const;
	bool CompareDate(const CVCardDateTime& comp) const
		{ return (mYear == comp.mYear) && (mMonth == comp.mMonth) && (mDay == comp.mDay); }
	int64_t GetPosixTime() const;

	bool IsDateOnly() const
		{ return mDateOnly; }
	void SetDateOnly(bool date_only)
		{ mDateOnly = date_only; }
	bool IsTimeOnly() const
		{ return mTimeOnly; }
	void SetTimeOnly(bool time_only)
		{ mTimeOnly = time_only; }

	int32_t GetYear() const
		{ return mYear; }
	void SetYear(int32_t year)
		{ if (mYear != year) { mYear = year; } }

	int32_t GetMonth() const
		{ return mMonth; }
	void SetMonth(int32_t month)
		{ if (mMonth != month) { mMonth = month; } }

	int32_t GetDay() const
		{ return mDay; }
	void SetDay(int32_t day)
		{ if (mDay != day) { mDay = day; } }

	void SetHHMMSS(int32_t hours, int32_t minutes, int32_t seconds)
		{ if ((mHours != hours) || (mMinutes != minutes) || (mSeconds != seconds)) { mHours = hours; mMinutes = minutes; mSeconds = seconds; } }

	int32_t GetHours() const
		{ return mHours; }
	void SetHours(int32_t hours)
		{ if (mHours != hours) { mHours = hours; } }

	int32_t GetMinutes() const
		{ return mMinutes; }
	void SetMinutes(int32_t minutes)
		{ if (mMinutes != minutes) { mMinutes = minutes; } }

	int32_t GetSeconds() const
		{ return mSeconds; }
	void SetSeconds(int32_t seconds)
		{ if (mSeconds != seconds) { mSeconds = seconds; } }

	int32_t GetTimezoneOffset() const
		{ return mTimezoneOffset; }
	void SetTimezoneOffset(int32_t tzoffset)
		{ if (mTimezoneOffset != tzoffset) { mTimezoneOffset = tzoffset; } }

	void SetToday()
		{ *this = GetToday(); }
	static CVCardDateTime GetToday();
	void SetNow()
		{ *this = GetNow(); }
	static CVCardDateTime GetNow();
	static CVCardDateTime GetNowUTC();

	cdstring GetLocaleDate(ELocaleDate locale) const;
	cdstring GetTime(bool with_seconds, bool am_pm, bool tzid = false) const;
	cdstring GetLocaleDateTime(ELocaleDate locale, bool with_seconds, bool am_pm, bool tzid = false) const;
	cdstring GetText() const;

	void ParseDate(const cdstring& data);
	void ParseTime(const cdstring& data);
	void ParseDateTime(const cdstring& data);
	void Generate(std::ostream& os) const;

protected:
	int32_t			mYear;		// full 4-digit year
	int32_t			mMonth;		// 1...12
	int32_t			mDay;		// 1...31

	int32_t			mHours;		// 0...23
	int32_t			mMinutes;	// 0...59
	int32_t			mSeconds;	// 0...60

	int32_t			mTimezoneOffset;

	bool			mDateOnly;
	bool			mTimeOnly;

private:
	void _init_CVCardDateTime();
	void _copy_CVCardDateTime(const CVCardDateTime& copy);

	void ParseDateTxt(const cdstring& data, uint32_t& offset);
	void ParseTimeTxt(const cdstring& data, uint32_t& offset);
	
	cdstring TimeZoneDescriptor() const;
	
	int32_t	DaysSince1970() const;

};

typedef std::vector<CVCardDateTime> CVCardDateTimeList;

}	// namespace vCard

#endif	// CVCardDateTime_H

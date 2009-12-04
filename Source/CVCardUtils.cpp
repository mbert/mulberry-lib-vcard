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
	CVCardUtils.cpp

	Author:
	Description:	<describe the CVCardUtils class here>
*/

#include "CVCardUtils.h"

#include <strstream>

using namespace vCard;

bool CVCardUtils::ReadFoldedLine(std::istream& is, cdstring& line1, cdstring& line2)
{
	// If line2 already has data, transfer that into line1
	if (line2.length() != 0)
		line1 = line2;
	else
	{
		// Fill first line
		getline(is, line1);
		if (is.fail())
			return false;
	}

	// Now loop looking ahead at the next line to see if it is folded
	while(true)
	{
		// Get next line
		getline(is, line2);
		if (is.fail())
			return true;

		// Does it start with a space => folded
		if ((line2.length() != 0) && isspace(line2[0UL]))
			// Copy folded line (without space) to current line and cycle for more
			line1.append(line2, 1, line2.length() - 1);
		else
			// Not folded - just exit loop
			break;
	}

	return true;
}

// Write out vCard encoded text value
void CVCardUtils::WriteTextValue(std::ostream& os, const cdstring& value)
{
	cdstring::size_type start_pos = 0;
	cdstring::size_type end_pos = value.find_first_of("\r\n;\\,", start_pos);
	cdstring::size_type size_pos = value.length();
	if (end_pos != cdstring::npos)
	{
		while(true)
		{
			// Write current segment
			os.write(value.c_str() + start_pos, end_pos - start_pos);

			// Write escape
			os << '\\';
			switch(value[end_pos])
			{
			case '\r':
				os << 'r';
				break;
			case '\n':
				os << 'n';
				break;
			case ';':
				os << ';';
				break;
			case '\\':
				os << '\\';
				break;
			case ',':
				os << ',';
				break;
			}

			// Bump past escapee and look for next segment
			start_pos = end_pos + 1;

			end_pos = value.find_first_of("\r\n;\\,", start_pos);
			if (end_pos == cdstring::npos)
			{
				os.write(value.c_str() + start_pos, size_pos - start_pos);
				break;
			}
		}
	}
	else
		os.write(value.c_str(), size_pos);
}

// Decode vCard encoded text value
cdstring CVCardUtils::DecodeTextValue(const cdstring& value)
{
	std::ostrstream os;

	cdstring::size_type start_pos = 0;
	cdstring::size_type end_pos = value.find_first_of("\\", start_pos);
	cdstring::size_type size_pos = value.length();
	if (end_pos != cdstring::npos)
	{
		while(true)
		{
			// Write current segment upto but not including the escape char
			os.write(value.c_str() + start_pos, end_pos - start_pos);

			// Bump to escapee char but not past the end
			end_pos++;
			if (end_pos >= size_pos)
				break;

			// Unescape
			switch(value[end_pos])
			{
			case 'r':
				os << '\r';
				break;
			case 'n':
				os << '\n';
				break;
			case ';':
				os << ';';
				break;
			case '\\':
				os << '\\';
				break;
			case ',':
				os << ',';
				break;
			}

			// Bump past escapee and look for next segment (not past the end)
			start_pos = end_pos + 1;
			if (start_pos >= size_pos)
				break;

			end_pos = value.find_first_of("\\", start_pos);
			if (end_pos == cdstring::npos)
			{
				os.write(value.c_str() + start_pos, size_pos - start_pos);
				break;
			}
		}
	}
	else
		os.write(value.c_str(), size_pos);

	os << std::ends;
	cdstring temp;
	temp.steal(os.str());
	return temp;

}

int32_t CVCardUtils::DaysInMonth(const int32_t month, const int32_t year)
{
	// NB month is 1..12 so use dummy value at start of array to avoid index adjustment
	int32_t days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int32_t days_in_month_leap[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	return IsLeapYear(year) ? days_in_month_leap[month] : days_in_month[month];
}

int32_t CVCardUtils::DaysUptoMonth(const int32_t month, const int32_t year)
{
	// NB month is 1..12 so use dummy value at start of array to avoid index adjustment
	int32_t days_upto_month[] = { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	int32_t days_upto_month_leap[] = { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

	return IsLeapYear(year) ? days_upto_month_leap[month] : days_upto_month[month];
}

int32_t CVCardUtils::IsLeapYear(const int32_t year)
{
	if (year <= 1752)
		return (year % 4 == 0);
	else
		return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

int32_t CVCardUtils::LeapDaysSince1970(const int32_t year_offset)
{
	if (year_offset > 2)
		return (year_offset + 1) / 4;
	else if (year_offset < -1)
		return (year_offset - 2) / 4;
	else
		return 0;
}

int32_t CVCardUtils::GetLocalTimezoneOffsetSeconds()
{
	int32_t diff = 0;

#if __dest_os == __mac_os || __dest_os == __mac_os_x
	// Must correct for daylight savings as MSL does not
	MachineLocation loc;
	ReadLocation(&loc);

	diff = loc.u.gmtDelta & 0x00FFFFFF;
	if (diff & 0x00800000)
		diff |= 0xFF000000;
#elif __dest_os == __win32_os
	// Replacement code reads time zone info direct from OS (code copied from MSL)
	// Note this is still wrong as Win32s does not support this API call
	TIME_ZONE_INFORMATION tzi;
	DWORD result = ::GetTimeZoneInformation(&tzi);
	switch(result)
	{
	case TIME_ZONE_ID_UNKNOWN:
		diff = -(tzi.Bias * 60);
		break;
	case TIME_ZONE_ID_STANDARD:
		diff = -((tzi.Bias + tzi.StandardBias) * 60);
		break;
	case TIME_ZONE_ID_DAYLIGHT:
		diff = -((tzi.Bias + tzi.DaylightBias) * 60);
		break;
	default:;
	}
#else
	// Get struct tm's for local and UTC
	time_t local = ::time(NULL);
	struct tm utc_now = *::gmtime(&local);
	struct tm local_now = *::localtime(&local);

	// Stupid correction because mktime needs this!
	utc_now.tm_isdst = -1;

	// Convert struct tm's to time_t's
	time_t utcnow = ::mktime(&utc_now);
	time_t localnow = ::mktime(&local_now);

	// Get offset in time_t's
	diff = (int32_t) ::difftime(localnow, utcnow);
#endif

	return diff;
}

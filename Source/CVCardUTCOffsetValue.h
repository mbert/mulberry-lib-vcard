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
	CVCardUTCOffsetValue.h

	Author:
	Description:	<describe the CVCardUTCOffsetValue class here>
*/

#ifndef CVCardUTCOffsetValue_H
#define CVCardUTCOffsetValue_H

#include "CVCardValue.h"

#include <stdint.h>


namespace vCard {

class CVCardUTCOffsetValue : public CVCardValue
{
public:
	CVCardUTCOffsetValue()
	{ mValue = 0; }
	CVCardUTCOffsetValue(const int32_t value)
		{ mValue = value; }
	CVCardUTCOffsetValue(const CVCardUTCOffsetValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardUTCOffsetValue(copy); }
	virtual ~CVCardUTCOffsetValue() {}

	CVCardUTCOffsetValue& operator=(const CVCardUTCOffsetValue& copy)
		{ if (this != &copy) _copy_CVCardUTCOffsetValue(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardUTCOffsetValue(*this); }

	virtual EVCardValueType GetType() const
		{ return eValueType_UTC_Offset; }

	virtual void Parse(const cdstring& data);
	virtual void Generate(std::ostream& os) const;

	int32_t GetValue() const
		{ return mValue; }
	void SetValue(const int32_t value)
		{ mValue = value; }

	virtual bool Search(const cdstring& pattern) const;

protected:
	int32_t mValue;

private:
	void _copy_CVCardUTCOffsetValue(const CVCardUTCOffsetValue& copy)
		{ mValue = copy.mValue; }
};

}	// namespace vCard

#endif	// CVCardUTCOffsetValue_H

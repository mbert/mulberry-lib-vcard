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
	CVCardDateTimeValue.h

	Author:
	Description:	<describe the CVCardDateTimeValue class here>
*/

#ifndef CVCardDateTimeValue_H
#define CVCardDateTimeValue_H

#include "CVCardValue.h"

#include "CVCardDateTime.h"


namespace vCard {

class CVCardDateTimeValue : public CVCardValue
{
public:
	CVCardDateTimeValue() {}
	CVCardDateTimeValue(const CVCardDateTime& value)
		{ mValue = value; }
	CVCardDateTimeValue(const CVCardDateTimeValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardDateTimeValue(copy); }
	virtual ~CVCardDateTimeValue() {}

	CVCardDateTimeValue& operator=(const CVCardDateTimeValue& copy)
		{ if (this != &copy) _copy_CVCardDateTimeValue(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardDateTimeValue(*this); }

	virtual EVCardValueType GetType() const
		{ return mValue.IsDateOnly() ? eValueType_Date : (mValue.IsTimeOnly() ? eValueType_Time : eValueType_DateTime); }

	virtual void Parse(const cdstring& data);
	virtual void Generate(std::ostream& os) const
		{ mValue.Generate(os); }

	CVCardDateTime& GetValue()
		{ return mValue; }
	const CVCardDateTime& GetValue() const
		{ return mValue; }
	void SetValue(const CVCardDateTime& value)
		{ mValue = value; }

	virtual bool Search(const cdstring& pattern) const;

protected:
	CVCardDateTime mValue;

private:
	void _copy_CVCardDateTimeValue(const CVCardDateTimeValue& copy)
		{ mValue = copy.mValue; }
};

}	// namespace vCard

#endif	// CVCardDateTimeValue_H

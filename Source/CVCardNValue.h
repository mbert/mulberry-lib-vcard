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
	CVCardNValue.h

	Author:
	Description:	<describe the CVCardNValue class here>
*/

#ifndef CVCardNValue_H
#define CVCardNValue_H

#include "CVCardValue.h"

#include "CVCardN.h"


namespace vCard {

class CVCardNValue : public CVCardValue
{
public:
	CVCardNValue() {}
	CVCardNValue(const cdstring& value)
	{
		mValue.SetFullName(value);
	}
	CVCardNValue(const CVCardN& value)
	{
		mValue = value;
	}
	CVCardNValue(const CVCardNValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardNValueValue(copy); }
	virtual ~CVCardNValue() {}

	CVCardNValue& operator=(const CVCardNValue& copy)
		{ if (this != &copy) _copy_CVCardNValueValue(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardNValue(*this); }

	virtual EVCardValueType GetType() const
		{ return eValueType_N; }

	virtual void Parse(const cdstring& data)
	{
		mValue.Parse(data);
	}
	virtual void Generate(std::ostream& os) const
	{
		mValue.Generate(os);
	}

	CVCardN& GetValue()
		{ return mValue; }
	const CVCardN& GetValue() const
		{ return mValue; }
	void SetValue(const CVCardN& value)
		{ mValue = value; }

	virtual bool Search(const cdstring& pattern) const;

protected:
	CVCardN mValue;

private:
	void _copy_CVCardNValueValue(const CVCardNValue& copy)
		{ mValue = copy.mValue; }

};

}	// namespace vCard

#endif	// CVCardNValue_H

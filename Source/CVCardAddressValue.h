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
	CVCardAddressValue.h

	Author:
	Description:	<describe the CVCardAddressValue class here>
*/

#ifndef CVCardAddressValue_H
#define CVCardAddressValue_H

#include "CVCardValue.h"

#include "CVCardAddress.h"


namespace vCard {

class CVCardAddressValue : public CVCardValue
{
public:
	CVCardAddressValue() {}
	CVCardAddressValue(const cdstring& value)
	{
		mValue.SetFullAddress(value);
	}
	CVCardAddressValue(const CVCardAddress& value)
	{
		mValue = value;
	}
	CVCardAddressValue(const CVCardAddressValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardAddressValueValue(copy); }
	virtual ~CVCardAddressValue() {}

	CVCardAddressValue& operator=(const CVCardAddressValue& copy)
		{ if (this != &copy) _copy_CVCardAddressValueValue(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardAddressValue(*this); }

	virtual EVCardValueType GetType() const
		{ return eValueType_Address; }

	virtual void Parse(const cdstring& data)
	{
		mValue.Parse(data);
	}
	virtual void Generate(std::ostream& os) const
	{
		mValue.Generate(os);
	}

	CVCardAddress& GetValue()
		{ return mValue; }
	const CVCardAddress& GetValue() const
		{ return mValue; }
	void SetValue(const CVCardAddress& value)
		{ mValue = value; }

	virtual bool Search(const cdstring& pattern) const;

protected:
	CVCardAddress mValue;

private:
	void _copy_CVCardAddressValueValue(const CVCardAddressValue& copy)
		{ mValue = copy.mValue; }

};

}	// namespace vCard

#endif	// CVCardAddressValue_H

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
	CVCardDummyValue.h

	Author:
	Description:	<describe the CVCardDummyValue class here>
*/

#ifndef CVCardDummyValue_H
#define CVCardDummyValue_H

#include "CVCardValue.h"


namespace vCard {

class CVCardDummyValue : public CVCardValue
{
public:
	CVCardDummyValue(EVCardValueType type)
		{ mType = type; }
	CVCardDummyValue(EVCardValueType type, const cdstring& value)
		{ mType = type; mValue = value; }
	CVCardDummyValue(const CVCardDummyValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardDummyValue(copy); }
	virtual ~CVCardDummyValue() {}

	CVCardDummyValue& operator=(const CVCardDummyValue& copy)
		{ if (this != &copy) _copy_CVCardDummyValue(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardDummyValue(*this); }

	virtual EVCardValueType GetType() const
		{ return mType; }

	virtual void Parse(const cdstring& data);
	virtual void Generate(std::ostream& os) const;

	cdstring& GetValue()
		{ return mValue; }
	const cdstring& GetValue() const
		{ return mValue; }
	void SetValue(const cdstring& value)
		{ mValue = value; }

	virtual bool Search(const cdstring& pattern) const;

protected:
	EVCardValueType		mType;
	cdstring			mValue;

private:
	void _copy_CVCardDummyValue(const CVCardDummyValue& copy)
		{ mType = copy.mType; mValue = copy.mValue; }
};

}	// namespace vCard

#endif	// CVCardDummyValue_H

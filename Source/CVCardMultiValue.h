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
	CVCardMultiValue.h

	Author:
	Description:	<describe the CVCardMultiValue class here>
*/

#ifndef CVCardMultiValue_H
#define CVCardMultiValue_H

#include "CVCardValue.h"

namespace vCard {

class CVCardMultiValue : public CVCardValue
{
public:
	CVCardMultiValue(EVCardValueType type)
		{ mType = type; }
	CVCardMultiValue(const CVCardMultiValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardMultiValue(copy); }
	virtual ~CVCardMultiValue()
		{ _tidy_CVCardMultiValue(); }

	CVCardMultiValue& operator=(const CVCardMultiValue& copy)
		{ if (this != &copy) _copy_CVCardMultiValue(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardMultiValue(*this); }

	virtual EVCardValueType GetType() const
		{ return mType; }
	
	const CVCardValueList& GetValues() const
		{ return mValues; }
	CVCardValueList& GetValues()
		{ return mValues; }
	void AddValue(CVCardValue* value)
		{ mValues.push_back(value); }

	virtual void Parse(const cdstring& data);
	virtual void Generate(std::ostream& os) const;

	virtual bool Search(const cdstring& pattern) const;

private:
	EVCardValueType		mType;
	CVCardValueList		mValues;

	void _copy_CVCardMultiValue(const CVCardMultiValue& copy);
	void _tidy_CVCardMultiValue();
};

}	// namespace vCard

#endif	// CVCardMultiValue_H

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
	CVCardTextValue.h

	Author:
	Description:	<describe the CVCardTextValue class here>
*/

#ifndef CVCardTextValue_H
#define CVCardTextValue_H

#include "CVCardPlainTextValue.h"


namespace vCard {

class CVCardTextValue : public CVCardPlainTextValue
{
public:
	CVCardTextValue() {}
	CVCardTextValue(const cdstring& value) :
		CVCardPlainTextValue(value) {}
	CVCardTextValue(const CVCardTextValue& copy)
		 : CVCardPlainTextValue(copy) {}
	virtual ~CVCardTextValue() {}

	CVCardTextValue& operator=(const CVCardTextValue& copy)
		{ if (this != &copy) CVCardPlainTextValue::operator=(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardTextValue(*this); }

	virtual EVCardValueType GetType() const
		{ return eValueType_Text; }

	virtual void Parse(const cdstring& data);
	virtual void Generate(std::ostream& os) const;

};

}	// namespace vCard

#endif	// CVCardValue_H

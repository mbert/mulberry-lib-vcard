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
	CVCardURIValue.h

	Author:
	Description:	<describe the CVCardURIValue class here>
*/

#ifndef CVCardURIValue_H
#define CVCardURIValue_H

#include "CVCardPlainTextValue.h"


namespace vCard {

class CVCardURIValue : public CVCardPlainTextValue
{
public:
	CVCardURIValue() {}
	CVCardURIValue(const cdstring& value) :
		CVCardPlainTextValue(value) {}
	CVCardURIValue(const CVCardURIValue& copy)
		 : CVCardPlainTextValue(copy) {}
	virtual ~CVCardURIValue() {}

	CVCardURIValue& operator=(const CVCardURIValue& copy)
		{ if (this != &copy) CVCardPlainTextValue::operator=(copy); return *this; }

	virtual CVCardValue* clone()
		{ return new CVCardURIValue(*this); }

	virtual EVCardValueType GetType() const
		{ return eValueType_URI; }

};

}	// namespace vCard

#endif	// CVCardURIValue_H

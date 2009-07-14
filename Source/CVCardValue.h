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
	CVCardValue.h

	Author:
	Description:	<describe the CVCardValue class here>
*/

#ifndef CVCardValue_H
#define CVCardValue_H

#include <iostream>
#include <vector>

#include "cdstring.h"


namespace vCard {

class CVCardValue
{
public:
	enum EVCardValueType
	{
		eValueType_Address,
		eValueType_Binary,
		eValueType_Date,
		eValueType_DateTime,
		eValueType_Float,
		eValueType_N,
		eValueType_Phone,
		eValueType_Text,
		eValueType_Time,
		eValueType_URI,
		eValueType_UTC_Offset,
		eValueType_MultiValue,
		eValueType_XName
	};

	static CVCardValue* CreateFromType(EVCardValueType type);

	CVCardValue() {}
	virtual ~CVCardValue() {}

	virtual CVCardValue* clone() = 0;

	virtual EVCardValueType GetType() const = 0;

	virtual void Parse(const cdstring& data) = 0;
	virtual void Generate(std::ostream& os) const = 0;
	
	virtual bool Search(const cdstring& pattern) const = 0;
};

typedef std::vector<CVCardValue*> CVCardValueList;

}	// namespace vCard

#endif	// CVCardValue_H

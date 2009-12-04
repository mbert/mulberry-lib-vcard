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
	CVCardValue.cpp

	Author:
	Description:	<describe the CVCardValue class here>
*/

#include "CVCardValue.h"

#include "CVCardAddressValue.h"
#include "CVCardDateTimeValue.h"
#include "CVCardDummyValue.h"
#include "CVCardNValue.h"
#include "CVCardTextValue.h"
#include "CVCardURIValue.h"
#include "CVCardUTCOffsetValue.h"

using namespace vCard;

CVCardValue* CVCardValue::CreateFromType(EVCardValueType type)
{
	// Create the type
	switch(type)
	{
	case eValueType_Address:
		return new CVCardAddressValue;
	case eValueType_Date:
	case eValueType_DateTime:
	case eValueType_Time:
		return new CVCardDateTimeValue;
	case eValueType_N:
		return new CVCardNValue;
	case eValueType_Phone:
	case eValueType_Text:
		return new CVCardTextValue;
	case eValueType_URI:
		return new CVCardURIValue;
	case eValueType_UTC_Offset:
		return new CVCardUTCOffsetValue;
	default:
		return new CVCardDummyValue(type);
	}
}

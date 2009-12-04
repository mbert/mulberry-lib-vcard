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
	CVCardComponentBase.cpp

	Author:
	Description:	<describe the CVCardComponentBase class here>
*/

#include "CVCardComponentBase.h"

#include "CVCardDateTimeValue.h"
#include "CVCardMultiValue.h"
#include "CVCardPlainTextValue.h"
#include "CVCardUTCOffsetValue.h"

using namespace vCard;

// Integers can be read from varios types of value
bool CVCardComponentBase::LoadValue(const char* value_name, int32_t& value, CVCardValue::EVCardValueType type) const
{
	if (GetProperties().count(value_name))
	{
		switch(type)
		{
		case CVCardValue::eValueType_UTC_Offset:
		{
			const CVCardUTCOffsetValue* uvalue = (*GetProperties().find(value_name)).second.GetUTCOffsetValue();
			if (uvalue != NULL)
			{
				value = uvalue->GetValue();
				return true;
			}
			break;
		}
		default:;
		}
	}

	return false;
}

bool CVCardComponentBase::LoadValue(const char* value_name, cdstring& value) const
{
	if (GetProperties().count(value_name))
	{
		const CVCardPlainTextValue* tvalue = (*GetProperties().find(value_name)).second.GetTextValue();
		if (tvalue != NULL)
		{
			value = tvalue->GetValue();
			return true;
		}
	}

	return false;
}

bool CVCardComponentBase::LoadValue(const char* value_name, CVCardDateTime& value) const
{
	if (GetProperties().count(value_name))
	{
		const CVCardDateTimeValue* dtvalue = (*GetProperties().find(value_name)).second.GetDateTimeValue();
		if (dtvalue != NULL)
		{
			value = dtvalue->GetValue();
			return true;
		}
	}

	return false;
}

void CVCardComponentBase::WriteProperties(std::ostream& os) const
{
	for(CVCardPropertyMap::const_iterator iter = mProperties.begin(); iter != mProperties.end(); iter++)
		(*iter).second.Generate(os);
}

bool CVCardComponentBase::LoadPrivateValue(const char* value_name, cdstring& value)
{
	// Read it in from properties list and then delete the property from the main list
	bool result = LoadValue(value_name, value);
	if (result)
		RemoveProperties(value_name);
	return result;
}

void CVCardComponentBase::WritePrivateProperty(std::ostream& os, const cdstring& key, const cdstring& value) const
{
	CVCardProperty prop(key, value);
	prop.Generate(os);
}

CVCardPropertyMap& CVCardComponentBase::GetProperties()
{
	return mProperties;
}
const CVCardPropertyMap& CVCardComponentBase::GetProperties() const
{
	return mProperties;
}
void CVCardComponentBase::SetProperties(const CVCardPropertyMap& props)
{
	 mProperties = props;
}

void CVCardComponentBase::AddProperty(const CVCardProperty& prop)
{
	CVCardPropertyMap::iterator result = mProperties.insert(CVCardPropertyMap::value_type(prop.GetName(), prop));
}

bool CVCardComponentBase::HasProperty(const cdstring& prop) const
{
	return mProperties.count(prop) > 0;
}

uint32_t CVCardComponentBase::CountProperty(const cdstring& prop) const
{
	return mProperties.count(prop);
}

void CVCardComponentBase::RemoveProperties(const cdstring& prop)
{
	mProperties.erase(prop);
}

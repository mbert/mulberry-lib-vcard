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
	CVCardProperty.cpp

	Author:
	Description:	<describe the CVCardProperty class here>
*/

#include "CVCardProperty.h"

#include "CVCardAddressValue.h"
#include "CVCardDateTimeValue.h"
#include "CVCardDefinitions.h"
#include "CVCardMultiValue.h"
#include "CVCardNValue.h"
#include "CVCardPlainTextValue.h"
#include "CVCardURIValue.h"
#include "CVCardUTCOffsetValue.h"
#include "CVCardValue.h"
#include "CStringUtils.h"

#include <iostream>
#include <memory>
#include <strstream>

using namespace vCard;

CVCardProperty::CValueTypeMap CVCardProperty::sDefaultValueTypeMap;
CVCardProperty::CValueTypeMap CVCardProperty::sValueTypeMap;
CVCardProperty::CEquivalentTypeMap CVCardProperty::sEquivalentTypeMap;
CVCardProperty::CTypeValueMap CVCardProperty::sTypeValueMap;
CVCardProperty::CMultiValues CVCardProperty::sMultiValues;

void CVCardProperty::_init_attr_value(const cdstring& txt, CVCardValue::EVCardValueType value_type)
{
	// Value
	mValue = CVCardValue::CreateFromType(value_type);
	CVCardPlainTextValue* tvalue = dynamic_cast<CVCardPlainTextValue*>(mValue);
	if (tvalue != NULL)
		tvalue->SetValue(txt);

	// Attributes
	SetupValueAttribute();
}

void CVCardProperty::_init_attr_value(const CVCardAddress& addr)
{
	// Value
	mValue = new CVCardAddressValue(addr);

	// Attributes
	SetupValueAttribute();
}

void CVCardProperty::_init_attr_value(const CVCardDateTime& dt)
{
	// Value
	mValue = new CVCardDateTimeValue(dt);

	// Attributes
	SetupValueAttribute();
}

void CVCardProperty::_init_attr_value(const CVCardDateTimeList& dtl)
{
	// Value
	mValue = new CVCardMultiValue((dtl.size() > 0) && dtl.front().IsDateOnly() ? CVCardValue::eValueType_Date : CVCardValue::eValueType_DateTime);
	for(CVCardDateTimeList::const_iterator iter = dtl.begin(); iter != dtl.end(); iter++)
	{
		static_cast<CVCardMultiValue*>(mValue)->AddValue(new CVCardDateTimeValue(*iter));
	}

	// Attributes
	SetupValueAttribute();
}

void CVCardProperty::_init_attr_value(const CVCardN& n)
{
	// Value
	mValue = new CVCardNValue(n);

	// Attributes
	SetupValueAttribute();
}

void CVCardProperty::_init_map()
{
	// Only if empty
	if (sDefaultValueTypeMap.empty())
	{
		// 2426 Properties
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_NAME, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_PROFILE, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_SOURCE, CVCardValue::eValueType_URI));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_FN, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_N, CVCardValue::eValueType_N));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_NICKNAME, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_PHOTO, CVCardValue::eValueType_Binary));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_BDAY, CVCardValue::eValueType_Date));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_ADR, CVCardValue::eValueType_Address));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_LABEL, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_TEL, CVCardValue::eValueType_Phone));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_EMAIL, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_MAILER, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_TZ, CVCardValue::eValueType_UTC_Offset));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_GEO, CVCardValue::eValueType_Float));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_TITLE, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_ROLE, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_LOGO, CVCardValue::eValueType_Binary));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_AGENT, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_ORG, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_CATEGORIES, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_NOTE, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_PRODID, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_REV, CVCardValue::eValueType_DateTime));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_SORT_STRING, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_SOUND, CVCardValue::eValueType_Binary));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_UID, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_URL, CVCardValue::eValueType_URI));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_VERSION, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_CLASS, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_KEY, CVCardValue::eValueType_Text));
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_IMPP, CVCardValue::eValueType_URI));
		
		// Extensions
		sDefaultValueTypeMap.insert(CValueTypeMap::value_type(cVCardProperty_CALENDAR_ADDRESS, CVCardValue::eValueType_URI));
	}

	// Only if empty
	if (sValueTypeMap.empty())
	{
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_BINARY, CVCardValue::eValueType_Binary));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_DATE, CVCardValue::eValueType_Date));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_DATE_TIME, CVCardValue::eValueType_DateTime));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_FLOAT, CVCardValue::eValueType_Float));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_TEXT, CVCardValue::eValueType_Text));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_TIME, CVCardValue::eValueType_Time));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_URI, CVCardValue::eValueType_URI));
		sValueTypeMap.insert(CValueTypeMap::value_type(cVCardValue_UTC_OFFSET, CVCardValue::eValueType_UTC_Offset));

	}

	if (sEquivalentTypeMap.empty())
	{
		// These are types that are the same as a 'standard' type, but have slightly different encoding rules
		sEquivalentTypeMap.insert(CEquivalentTypeMap::value_type(CVCardValue::eValueType_Address, CVCardValue::eValueType_Text));
		sEquivalentTypeMap.insert(CEquivalentTypeMap::value_type(CVCardValue::eValueType_N, CVCardValue::eValueType_Text));
		sEquivalentTypeMap.insert(CEquivalentTypeMap::value_type(CVCardValue::eValueType_Phone, CVCardValue::eValueType_Text));
	}

	if (sTypeValueMap.empty())
	{
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Address, cVCardValue_TEXT));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Binary, cVCardValue_BINARY));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Date, cVCardValue_DATE));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_DateTime, cVCardValue_DATE_TIME));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Float, cVCardValue_FLOAT));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_N, cVCardValue_TEXT));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Phone, cVCardValue_TEXT));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Text, cVCardValue_TEXT));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_Time, cVCardValue_TIME));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_URI, cVCardValue_URI));
		sTypeValueMap.insert(CTypeValueMap::value_type(CVCardValue::eValueType_UTC_Offset, cVCardValue_UTC_OFFSET));
	}

	if (sMultiValues.empty())
	{
		sMultiValues.insert(cVCardProperty_NICKNAME);
		sMultiValues.insert(cVCardProperty_CATEGORIES);
	}
}

bool CVCardProperty::HasAttributeValue(const cdstring& attr, const cdstring& value, bool casei) const
{
	// Get iterators for <begin, end| range
	std::pair<CVCardAttributeMap::const_iterator, CVCardAttributeMap::const_iterator> found = mAttributes.equal_range(attr);
	for(CVCardAttributeMap::const_iterator iter = found.first; iter != found.second; iter++)
	{
		if ((*iter).second.ContainsValue(value, casei))
			return true;
	}
	
	return false;
}

void CVCardProperty::AddAttribute(const CVCardAttribute& attr)
{
	mAttributes.insert(CVCardAttributeMap::value_type(attr.GetName(), attr));
}

void CVCardProperty::RemoveAttributes(const cdstring& attr)
{
	mAttributes.erase(attr);
}

const CVCardAddressValue* CVCardProperty::GetAddressValue() const
{
	return dynamic_cast<CVCardAddressValue*>(mValue);
}

const CVCardDateTimeValue* CVCardProperty::GetDateTimeValue() const
{
	return dynamic_cast<CVCardDateTimeValue*>(mValue);
}

const CVCardMultiValue* CVCardProperty::GetMultiValue() const
{
	return dynamic_cast<CVCardMultiValue*>(mValue);
}

const CVCardNValue* CVCardProperty::GetNValue() const
{
	return dynamic_cast<CVCardNValue*>(mValue);
}

const CVCardPlainTextValue* CVCardProperty::GetTextValue() const
{
	return dynamic_cast<CVCardPlainTextValue*>(mValue);
}

const CVCardURIValue* CVCardProperty::GetURIValue() const
{
	return dynamic_cast<CVCardURIValue*>(mValue);
}

const CVCardUTCOffsetValue* CVCardProperty::GetUTCOffsetValue() const
{
	return dynamic_cast<CVCardUTCOffsetValue*>(mValue);
}

bool CVCardProperty::Parse(cdstring& data)
{
	char* p = const_cast<char*>(data.c_str());

	// Look for attribute or value delimiter
	{
		std::auto_ptr<char> prop_name(::strduptokenstr(&p, ";:"));
		if ((prop_name.get() == NULL) || (*p == 0))
			return false;

		// We have the name
		mName.assign(prop_name.get());
		
		// See if its a group
		cdstring::size_type pos = mName.find('.');
		if (pos != cdstring::npos)
		{
			// Get group
			mGroup.assign(mName, 0, pos);

			// Strip out group
			mName.erase(0, pos + 1);
		}
	}

	// Now loop getting data
	bool done = false;
	while(!done && (*p != 0))
	{
		switch(*p)
		{
		case ';':
			// Parse attribute
			{
				// Move past delimiter
				p++;

				// Get quoted string or token
				std::auto_ptr<char> attribute_name(::strduptokenstr(&p, "="));
				if (attribute_name.get() == NULL)
					return false;
				p++;
				std::auto_ptr<char> attribute_value(::strduptokenstr(&p, ":;,"));
				if (attribute_value.get() == NULL)
					return false;

				// Now add attribute value
				CVCardAttribute attrvalue(attribute_name.get(), attribute_value.get());

				// Look for additional values
				while(*p == ',')
				{
					p++;
					std::auto_ptr<char> attribute_value2(::strduptokenstr(&p, ":;,"));
					if (attribute_value2.get() != NULL)
						attrvalue.AddValue(attribute_value2.get());
				}
				mAttributes.insert(CVCardAttributeMap::value_type(attrvalue.GetName(), attrvalue));
			}
			break;
		case ':':
			CreateValue(p + 1);
			done = true;
			break;
		default:;
		}
	}
	
	// We must have a value of some kind
	return (mValue != NULL);
}

void CVCardProperty::CreateValue(const char* data)
{
	// Tidy first
	delete mValue;
	mValue = NULL;

	// Get value type from property name
	CVCardValue::EVCardValueType type = CVCardValue::eValueType_Text;
	CVCardValue::EVCardValueType default_type = CVCardValue::eValueType_Text;
	_init_map();
	CValueTypeMap::const_iterator found = sDefaultValueTypeMap.find(mName);
	if (found != sDefaultValueTypeMap.end())
		default_type = type = (*found).second;

	// Check whether custom value is set
	if (mAttributes.count(cVCardAttribute_VALUE))
	{
		CValueTypeMap::const_iterator found = sValueTypeMap.find(GetAttributeValue(cVCardAttribute_VALUE));
		if (found != sValueTypeMap.end())
			type = (*found).second;
		
		// Now check equivalent default types and map from specified type to default
		CEquivalentTypeMap::const_iterator found2 = sEquivalentTypeMap.find(default_type);
		if ((found2 != sEquivalentTypeMap.end()) && ((*found2).second == type))
			type = default_type;
	}

	// Check for multivalued
	if (sMultiValues.count(mName))
	{
		mValue = new CVCardMultiValue(type);
	}
	else
	{
		// Create the type
		mValue = CVCardValue::CreateFromType(type);
	}

	// Now parse the data
	mValue->Parse(data);
}

// Make sure current VALUE= attribute matches the actual data we have
// or is absent if default value
void CVCardProperty::SetupValueAttribute()
{
	mAttributes.erase(cVCardAttribute_VALUE);

	// Only if we have a value right now
	if (mValue == NULL)
		return;

	// See if current type is default for this property
	_init_map();
	CValueTypeMap::const_iterator found = sDefaultValueTypeMap.find(mName);
	if (found != sDefaultValueTypeMap.end())
	{
		CVCardValue::EVCardValueType default_type = (*found).second;
		if (default_type != mValue->GetType())
		{
			// Check to see if the current value type is equivalent to the default
			CEquivalentTypeMap::const_iterator found2 = sEquivalentTypeMap.find(default_type);
			if ((found2 == sEquivalentTypeMap.end()) || ((*found2).second != mValue->GetType()))
			{
				// Find the appropriate VALUE= value for this value and add the attribute
				CTypeValueMap::const_iterator found3 = sTypeValueMap.find(mValue->GetType());
				if (found3 != sTypeValueMap.end())
				{
					mAttributes.insert(CVCardAttributeMap::value_type(cVCardAttribute_VALUE, CVCardAttribute(cVCardAttribute_VALUE, (*found3).second)));
				}
			}
		}
	}
}

void CVCardProperty::Generate(std::ostream& os) const
{
	const_cast<CVCardProperty*>(this)->SetupValueAttribute();

	// Must write to temp buffer and then wrap
	std::ostrstream sout;
	
	if (!mGroup.empty())
		sout << mGroup << ".";
	sout << mName;

	// Write all attributes
	for(CVCardAttributeMap::const_iterator iter = mAttributes.begin(); iter != mAttributes.end(); iter++)
	{
		sout << ";";
		(*iter).second.Generate(sout);
	}

	// Write value
	sout << ":";
	if (mValue)
		mValue->Generate(sout);
	
	sout << std::ends;
	cdstring temp;
	temp.steal(sout.str());
	if (temp.length() < 75)
		os << temp.c_str();
	else
	{
		const char* p = temp.c_str();
		const char* q = p + temp.length();
		while(p < q)
		{
			if (p != temp.c_str())
			{
				os << std::endl << " ";
			}

			int bytes = q - p;
			if (bytes > 74)
				bytes = 74;
			os.write(p, bytes);
			p += bytes;
		}
	}

	os << std::endl;
}


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
	CVCardProperty.h

	Author:
	Description:	<describe the CVCardProperty class here>
*/

#ifndef CVCardProperty_H
#define CVCardProperty_H

#include "CVCardAttribute.h"
#include "CVCardValue.h"

#include <stdint.h>
#include <map>
#include <set>

#include "cdstring.h"


namespace vCard {

typedef uint32_t	CVCardRef;	// Unique reference to object

class CVCardAddressValue;
class CVCardDateTimeValue;
class CVCardMultiValue;
class CVCardNValue;
class CVCardPlainTextValue;
class CVCardURIValue;
class CVCardUTCOffsetValue;

class CVCardAddress;
class CVCardDateTime;
typedef std::vector<CVCardDateTime> CVCardDateTimeList;
class CVCardN;

class CVCardProperty
{
public:
	CVCardProperty()
		{ _init_CVCardProperty(); }
	CVCardProperty(const cdstring& name, const cdstring& text_value, CVCardValue::EVCardValueType value_type = CVCardValue::eValueType_Text)
		{ mName = name; _init_attr_value(text_value, value_type); }
	CVCardProperty(const cdstring& name, const CVCardAddress& addr)
		{ mName = name; _init_attr_value(addr); }
	CVCardProperty(const cdstring& name, const CVCardDateTime& dt)
		{ mName = name; _init_attr_value(dt); }
	CVCardProperty(const cdstring& name, const CVCardDateTimeList& dtl)
		{ mName = name; _init_attr_value(dtl); }
	CVCardProperty(const cdstring& name, const CVCardN& n)
		{ mName = name; _init_attr_value(n); }
	CVCardProperty(const CVCardProperty& copy)
		{ _init_CVCardProperty(); _copy_CVCardProperty(copy); }
	virtual ~CVCardProperty()
		{ _tidy_CVCardProperty(); }

	CVCardProperty& operator=(const CVCardProperty& copy)
		{ if (this != &copy) _copy_CVCardProperty(copy); return *this; }

	cdstring& GetGroup()
		{ return mGroup; }
	const cdstring& GetGroup() const
		{ return mGroup; }
	void SetGroup(const cdstring& group)
		{ mGroup = group; }

	cdstring& GetName()
		{ return mName; }
	const cdstring& GetName() const
		{ return mName; }
	void SetName(const cdstring& name)
		{ mName = name; }

	CVCardAttributeMap& GetAttributes()
		{ return mAttributes; }
	const CVCardAttributeMap& GetAttributes() const
		{ return mAttributes; }
	void SetAttributes(const CVCardAttributeMap& attributes)
		{ mAttributes = attributes; }
	bool HasAttribute(const cdstring& attr) const
	{
		return mAttributes.count(attr) != 0;
	}
	bool HasAttributeValue(const cdstring& attr, const cdstring& value, bool casei = false) const;
	const cdstring& GetAttributeValue(const cdstring& attr) const
	{
		return (*mAttributes.find(attr)).second.GetFirstValue();
	}

	void AddAttribute(const CVCardAttribute& attr);
	void RemoveAttributes(const cdstring& attr);

	CVCardValue* GetValue()
		{ return mValue; }
	const CVCardValue* GetValue() const
		{ return mValue; }

	const CVCardAddressValue* GetAddressValue() const;
	const CVCardDateTimeValue* GetDateTimeValue() const;
	const CVCardMultiValue* GetMultiValue() const;
	const CVCardNValue* GetNValue() const;
	const CVCardPlainTextValue* GetTextValue() const;
	const CVCardURIValue* GetURIValue() const;
	const CVCardUTCOffsetValue* GetUTCOffsetValue() const;

	bool Parse(cdstring& data);
	void Generate(std::ostream& os) const;

protected:
	cdstring					mGroup;
	cdstring					mName;
	CVCardAttributeMap			mAttributes;
	CVCardValue*				mValue;

	typedef std::map<cdstring, CVCardValue::EVCardValueType>	CValueTypeMap;
	static CValueTypeMap		sDefaultValueTypeMap;
	static CValueTypeMap		sValueTypeMap;

	typedef std::map<CVCardValue::EVCardValueType, cdstring>	CTypeValueMap;
	static CTypeValueMap		sTypeValueMap;

	typedef std::map<CVCardValue::EVCardValueType, CVCardValue::EVCardValueType>	CEquivalentTypeMap;
	static CEquivalentTypeMap	sEquivalentTypeMap;

	typedef cdstrset			CMultiValues;
	static CMultiValues			sMultiValues;

private:
	void _init_CVCardProperty()
		{ mValue = NULL; }
	void _copy_CVCardProperty(const CVCardProperty& copy)
		{ _tidy_CVCardProperty(); mGroup = copy.mGroup; mName = copy.mName; mAttributes = copy.mAttributes; mValue = copy.mValue->clone(); }
	void _tidy_CVCardProperty()
		{ delete mValue; mValue = NULL; }
	void _init_map();

	void CreateValue(const char* data);
	void SetupValueAttribute();


	// Creation
	void _init_attr_value(const cdstring& txt, CVCardValue::EVCardValueType value_type = CVCardValue::eValueType_Text);
	void _init_attr_value(const CVCardAddress& addr);
	void _init_attr_value(const CVCardDateTime& dt);
	void _init_attr_value(const CVCardDateTimeList& dtl);
	void _init_attr_value(const CVCardN& n);
};

typedef std::multimap<cdstring, CVCardProperty, case_insensitive_cdstring> CVCardPropertyMap;
typedef std::vector<CVCardProperty> CVCardPropertyList;

}	// namespace vCard

#endif	// CVCardProperty_H

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
	CVCardComponentBase.h

	Author:
	Description:	<describe the CVCardComponentBase class here>
*/

#ifndef CVCardComponentBase_H
#define CVCardComponentBase_H

#include "CVCardProperty.h"

#include <stdint.h>
#include <iostream>
#include <map>

#include "cdstring.h"


namespace vCard {

class CVCardDateTime;

class CVCardComponentBase
{
public:
	CVCardComponentBase() {}
	CVCardComponentBase(const CVCardComponentBase& copy)
		{ _copy_CVCardComponentBase(copy); }
	virtual ~CVCardComponentBase() {}

	CVCardComponentBase& operator=(const CVCardComponentBase& copy)
		{ if (this != &copy) _copy_CVCardComponentBase(copy); return *this; }

	CVCardPropertyMap& GetProperties();
	const CVCardPropertyMap& GetProperties() const;
	void SetProperties(const CVCardPropertyMap& props);

	void AddProperty(const CVCardProperty& prop);
	bool HasProperty(const cdstring& prop) const;
	uint32_t CountProperty(const cdstring& prop) const;
	void RemoveProperties(const cdstring& prop);

	bool GetProperty(const cdstring& prop, int32_t& value, CVCardValue::EVCardValueType type = CVCardValue::eValueType_UTC_Offset) const
	{
		return LoadValue(prop, value, type);
	}
	bool GetProperty(const cdstring& prop, cdstring& value) const
	{
		return LoadValue(prop, value);
	}
	bool GetProperty(const cdstring& prop, CVCardDateTime& value) const
	{
		return LoadValue(prop, value);
	}

	virtual void Finalise() = 0;

	virtual void Generate(std::ostream& os, bool for_cache = false) const = 0;

#ifdef _GCC_2_95
public:
#else
protected:
#endif
	CVCardPropertyMap		mProperties;

	virtual bool	LoadValue(const char* value_name, int32_t& value, CVCardValue::EVCardValueType type = CVCardValue::eValueType_UTC_Offset) const;
	virtual bool	LoadValue(const char* value_name, cdstring& value) const;
	virtual bool	LoadValue(const char* value_name, CVCardDateTime& value) const;
	
	void	WriteProperties(std::ostream& os) const;

	bool	LoadPrivateValue(const char* value_name, cdstring& value);
	void	WritePrivateProperty(std::ostream& os, const cdstring& key, const cdstring& value) const;

private:
	void	_copy_CVCardComponentBase(const CVCardComponentBase& copy)
		{ mProperties = copy.mProperties; }
};

}	// namespace vCard

#endif	// CVCardComponentBase_H

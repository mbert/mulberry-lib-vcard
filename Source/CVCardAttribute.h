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
	CVCardAttribute.h

	Author:
	Description:	<describe the CVCardAttribute class here>
*/

#ifndef CVCardAttribute_H
#define CVCardAttribute_H

#include <map>
#include <vector>

#include "cdstring.h"


namespace vCard {

class CVCardAttribute
{
public:
	CVCardAttribute() {}
	CVCardAttribute(const cdstring& name, const cdstring& value)
		{ mName = name; mValues.push_back(value); }
	CVCardAttribute(const CVCardAttribute& copy)
		{ _copy(copy); }
	virtual ~CVCardAttribute() {}

	CVCardAttribute& operator=(const CVCardAttribute& copy)
		{ if (this != &copy) _copy(copy); return *this; }

	cdstring& GetName()
		{ return mName; }
	const cdstring& GetName() const
		{ return mName; }
	void SetName(const cdstring& name)
		{ mName = name; }

	const cdstring& GetFirstValue() const
		{ return mValues.front(); }
	cdstrvect& GetValues()
		{ return mValues; }
	const cdstrvect& GetValues() const
		{ return mValues; }
	void AddValue(const cdstring& value)
		{ mValues.push_back(value); }

	bool ContainsValue(const cdstring& value, bool casei = false) const;

	void Generate(std::ostream& os) const;

protected:
	cdstring		mName;
	cdstrvect		mValues;

private:
	void _copy(const CVCardAttribute& copy)
		{ mName = copy.mName; mValues = copy.mValues; }
};

typedef std::multimap<cdstring, CVCardAttribute, case_insensitive_cdstring> CVCardAttributeMap;

}	// namespace vCard

#endif	// CVCardAttribute_H

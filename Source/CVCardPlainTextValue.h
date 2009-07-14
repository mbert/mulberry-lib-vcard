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
	CVCardPlainTextValue.h

	Author:
	Description:	<describe the CVCardPlainTextValue class here>
*/

#ifndef CVCardPlainTextValue_H
#define CVCardPlainTextValue_H

#include "CVCardValue.h"


namespace vCard {

class CVCardPlainTextValue : public CVCardValue
{
public:
	CVCardPlainTextValue() {}
	CVCardPlainTextValue(const cdstring& value)
		{ mValue = value; }
	CVCardPlainTextValue(const CVCardPlainTextValue& copy)
		 : CVCardValue(copy)
		{ _copy_CVCardPlainTextValue(copy); }
	virtual ~CVCardPlainTextValue() {}

	CVCardPlainTextValue& operator=(const CVCardPlainTextValue& copy)
		{ if (this != &copy) _copy_CVCardPlainTextValue(copy); return *this; }

	virtual void Parse(const cdstring& data);
	virtual void Generate(std::ostream& os) const;

	cdstring& GetValue()
		{ return mValue; }
	const cdstring& GetValue() const
		{ return mValue; }
	void SetValue(const cdstring& value)
		{ mValue = value; }

	virtual bool Search(const cdstring& pattern) const;

protected:
	cdstring mValue;

private:
	void _copy_CVCardPlainTextValue(const CVCardPlainTextValue& copy)
		{ mValue = copy.mValue; }
};

}	// namespace vCard

#endif	// CVCardPlainTextValue_H

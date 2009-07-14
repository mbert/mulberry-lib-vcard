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
	CVCardN.h

	Author:
	Description:	<describe the CVCardN class here>
*/

#ifndef CVCardN_H
#define CVCardN_H

#include <ostream>

#include "cdstring.h"


namespace vCard {

class CVCardN
{
public:
	CVCardN() {}
	CVCardN(const cdstring& fullname)
	{
		SetFullName(fullname);
	}
	CVCardN(const cdstring& first, const cdstring& last)
	{
		mItems[eFirst] = first;
		mItems[eLast] = last;
	}
	CVCardN(const CVCardN& copy)
		{ _copy_CVCardN(copy); }
	virtual ~CVCardN() {}

	CVCardN& operator=(const CVCardN& copy)
		{ if (this != &copy) _copy_CVCardN(copy); return *this; }

	const cdstring& GetFirst() const
		{ return mItems[eFirst]; }
	void SetFirst(const cdstring& first)
		{ mItems[eFirst] = first; }

	const cdstring& GetLast() const
		{ return mItems[eLast]; }
	void SetLast(const cdstring& last)
		{ mItems[eLast] = last; }

	const cdstring& GetMiddle() const
		{ return mItems[eMiddle]; }
	void SetMiddle(const cdstring& middle)
		{ mItems[eMiddle] = middle; }

	const cdstring& GetPrefix() const
		{ return mItems[ePrefix]; }
	void SetPrefix(const cdstring& prefix)
		{ mItems[ePrefix] = prefix; }

	const cdstring& GetSuffix() const
		{ return mItems[eSuffix]; }
	void SetSuffix(const cdstring& suffix)
		{ mItems[eSuffix] = suffix; }

	cdstring GetFullName(bool with_titles = false) const;
	void SetFullName(const cdstring& data);

	void Parse(const cdstring& data);
	void Generate(std::ostream& os) const;

	bool Search(const cdstring& pattern) const;

protected:
	enum
	{
		eLast = 0,
		eFirst,
		eMiddle,
		ePrefix,
		eSuffix,
		eMaxIndex
	};
	cdstring		mItems[eMaxIndex];

private:
	void _copy_CVCardN(const CVCardN& copy);

	void AppendFullNameItem(cdstring& result, const cdstring& item) const;

};

}	// namespace vCard

#endif	// CVCardN_H

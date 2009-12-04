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
	CVCardAddress.h

	Author:
	Description:	<describe the CVCardAddress class here>
*/

#ifndef CVCardAddress_H
#define CVCardAddress_H

#include <ostream>

#include "cdstring.h"


namespace vCard {

class CVCardAddress
{
public:
	CVCardAddress() {}
	CVCardAddress(const cdstring& fulladdr)
	{
		SetFullAddress(fulladdr);
	}
	CVCardAddress(const CVCardAddress& copy)
		{ _copy_CVCardAddress(copy); }
	virtual ~CVCardAddress() {}

	CVCardAddress& operator=(const CVCardAddress& copy)
		{ if (this != &copy) _copy_CVCardAddress(copy); return *this; }

	const cdstring& GetPOBox() const
		{ return mItems[ePOBox]; }
	void SetPOBox(const cdstring& pobox)
		{ mItems[ePOBox] = pobox; }

	const cdstring& GetExtended() const
		{ return mItems[eExtended]; }
	void SetExtended(const cdstring& extended)
		{ mItems[eExtended] = extended; }

	const cdstring& GetStreet() const
		{ return mItems[eStreet]; }
	void SetStreet(const cdstring& street)
		{ mItems[eStreet] = street; }

	const cdstring& GetLocality() const
		{ return mItems[eLocality]; }
	void SetLocality(const cdstring& locality)
		{ mItems[eLocality] = locality; }

	const cdstring& GetRegion() const
		{ return mItems[eRegion]; }
	void SetRegion(const cdstring& region)
		{ mItems[eRegion] = region; }

	const cdstring& GetPostalCode() const
		{ return mItems[ePostalCode]; }
	void SetPostalCode(const cdstring& pocode)
		{ mItems[ePostalCode] = pocode; }

	const cdstring& GetCountry() const
		{ return mItems[eCountry]; }
	void SetCountry(const cdstring& country)
		{ mItems[eCountry] = country; }

	cdstring GetFullAddress() const;
	void SetFullAddress(const cdstring& data);

	void Parse(const cdstring& data);
	void Generate(std::ostream& os) const;

protected:
	enum
	{
		ePOBox = 0,
		eExtended,
		eStreet,
		eLocality,
		eRegion,
		ePostalCode,
		eCountry,
		eMaxIndex
	};
	cdstring		mItems[eMaxIndex];

private:
	void _copy_CVCardAddress(const CVCardAddress& copy);

	void AppendFullAddressItem(cdstring& result, const cdstring& item) const;

};

}	// namespace vCard

#endif	// CVCardAddress_H

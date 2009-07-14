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
	CVCardComponent.h

	Author:
	Description:	<describe the CVCardComponent class here>
*/

#ifndef CVCardComponent_H
#define CVCardComponent_H

#include "CVCardComponentBase.h"

#include "CVCardAddressBookRef.h"
#include "CVCardValue.h"

#include <stdint.h>
#include <iostream>
#include <vector>

#include "cdstring.h"


namespace vCard {

class CVCardComponent;
typedef std::vector<CVCardComponent*> CVCardComponentList;

class CVCardAddressBook;

class CVCardComponent : public CVCardComponentBase
{
public:
	enum EComponentType
	{
		eVCARD
	};

	typedef CVCardComponent* (*CreateComponentPP)(const CVCardAddressBookRef& adbk);

	CVCardComponent(const CVCardAddressBookRef& adbk)
		{ mAddressBookRef = adbk; mChanged = false; }
	CVCardComponent(const CVCardComponent& copy) :
		CVCardComponentBase(copy)
		{ mChanged = false; _copy_CVCardComponent(copy); }
	virtual ~CVCardComponent();

	CVCardComponent& operator=(const CVCardComponent& copy)
		{
			if (this != &copy)
			{
				_copy_CVCardComponent(copy);
				CVCardComponentBase::operator=(copy);
			}
			return *this;
		}

	virtual CVCardComponent* clone() const = 0;

	virtual EComponentType GetType() const = 0;
	virtual const cdstring& GetBeginDelimiter() const = 0;
	virtual const cdstring& GetEndDelimiter() const = 0;

	virtual bool AddComponent(CVCardComponent* comp);
			void RemoveComponent(CVCardComponent* comp);

	void SetAddressBook(const CVCardAddressBookRef& ref)
		{ mAddressBookRef = ref; }
	const CVCardAddressBookRef& GetAddressBook() const
		{ return mAddressBookRef; }

	virtual const cdstring& GetMapKey() const
		{ return mUID; }
	virtual cdstring GetMasterKey() const
		{ return mUID; }

	cdstring& GetUID()
	{
		return mUID;
	}
	const cdstring& GetUID() const
	{
		return mUID;
	}
	virtual void SetUID(const cdstring& uid);

	const cdstring& GetRURL() const
	{
		return mRURL;
	}
	void SetRURL(const cdstring& rurl)
	{
		mRURL = rurl;
	}
	void GenerateRURL();

	const cdstring& GetETag() const
	{
		return mETag;
	}
	void SetETag(const cdstring& etag)
	{
		mETag = etag;
	}

	bool GetChanged() const
	{
		return mChanged;
	}
	void SetChanged(bool changed)
	{
		mChanged = changed;
	}

	virtual void InitRevision();
	virtual void UpdateRevision();

	virtual void Added();
	virtual void Removed();
	virtual void Duplicated();
	virtual void Changed();

	virtual void Finalise();

	virtual bool CanGenerateInstance() const
	{
		return true;
	}
	virtual void Generate(std::ostream& os, bool for_cache = false) const;

protected:
	CVCardAddressBookRef		mAddressBookRef;
	cdstring					mUID;
	
	// CardDAV stuff
	cdstring					mRURL;
	cdstring					mETag;
	bool						mChanged;

private:
	void	_copy_CVCardComponent(const CVCardComponent& copy);
};

}	// namespace vCard

#endif	// CVCardComponent_H

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
	CVCardAddressBook.h

	Author:
	Description:	<describe the CVCardAddressBook class here>
*/

#ifndef CVCardAddressBook_H
#define CVCardAddressBook_H

#include "CVCardComponentBase.h"
#include "CBroadcaster.h"

#include "CVCardAddressBookRef.h"
#include "CVCardComponentRecord.h"
#include "CVCardComponent.h"
#include "CVCardComponentDB.h"

#include <iostream>


namespace vCard {

class CVCardProperty;
class CVCardVCard;

class CVCardAddressBook : public CVCardComponentBase, public CBroadcaster
{
public:
	// Messages for broadcast
	enum
	{
		// Regular messages
		eBroadcast_Changed = 'calc',
		eBroadcast_Edit = 'edit',
		eBroadcast_Closed = 'cald',
		
		// Feedback messages
		eBroadcast_AddedComponent = 'adde',
		eBroadcast_ChangedComponent = 'chan',
		eBroadcast_RemovedComponent = 'remo'
		
	};

	class CComponentAction
	{
	public:
		enum EOperation
		{
			eAdded,
			eChanged,
			eRemoved
		};

		CComponentAction(EOperation operation, CVCardAddressBook& adbk, const CVCardComponent& component) :
			mOperation(operation), mAddressBook(adbk), mComponent(component) {}
		
		~CComponentAction()  {}
		
		EOperation GetOperation() const
		{
			return mOperation;
		}
		
		CVCardAddressBook& GetAddressBook() const
		{
			return mAddressBook;
		}
		
		const CVCardComponent& GetComponent() const
		{
			return mComponent;
		}
	
	private:
		EOperation					mOperation;
		CVCardAddressBook&			mAddressBook;
		const CVCardComponent&		mComponent;
	};

	static CVCardAddressBook* GetVCardAddressBook(const CVCardAddressBookRef& ref);

	CVCardAddressBook();
	virtual ~CVCardAddressBook();

	const CVCardAddressBookRef& GetRef() const
		{ return mVCardAddressBookRef; }

	void Clear();

	cdstring& GetName()
		{ return mName; }
	const cdstring& GetName() const
		{ return mName; }
	void SetName(const cdstring& name)
		{ mName = name; }
	void EditName(const cdstring& name);

	cdstring& GetDescription()
		{ return mDescription; }
	const cdstring& GetDescription() const
		{ return mDescription; }
	void SetDescription(const cdstring& description)
		{ mDescription = description; }
	void EditDescription(const cdstring& description);

	virtual void Finalise();

	bool					Parse(std::istream& is);
	CVCardVCard*			ParseComponent(std::istream& is, const cdstring& rurl, const cdstring& etag);
	virtual void			Generate(std::ostream& os, bool for_cache = false) const;
	virtual void			GenerateOne(std::ostream& os, const CVCardVCard& comp) const;

	// Get components
	const CVCardComponentDB& GetVCards() const
	{
		return mVCard;
	}

	// Disconnected support
	const cdstring& GetETag() const
	{
		return mETag;
	}
	void SetETag(const cdstring& etag)
	{
		mETag = etag;
	}

	const CVCardComponentRecordDB& GetRecording() const
	{
		return mRecordDB;
	}
	void ClearRecording()
	{
		mRecordDB.clear();
	}
	bool NeedsSync() const
	{
		return !mRecordDB.empty();
	}

	void	ParseCache(std::istream& is);
	void	GenerateCache(std::ostream& os) const;

	// Add/remove components
	void	ChangedComponent(CVCardComponent* comp);

	void	AddNewVCard(CVCardVCard* vcard, bool moved = false);
	void	RemoveVCard(CVCardVCard* vcard, bool delete_it = true);

	void AddCard(CVCardComponent* comp);
	const CVCardVCard* GetCardByKey(const cdstring& mapkey) const;
	CVCardVCard* GetCardByKey(const cdstring& mapkey);
	void RemoveCardByKey(const cdstring& mapkey);

	bool	IsReadOnly() const
	{
		return mReadOnly;
	}
	void	SetReadOnly(bool ro = true)
	{
		mReadOnly = ro;
	}

	// Change state
	bool	IsDirty() const
	{
		return mDirty;
	}
	void	SetDirty(bool dirty = true)
	{
		mDirty = dirty;
	}

	bool	IsTotalReplace() const
	{
		return mTotalReplace;
	}
	void	SetTotalReplace(bool replace = true)
	{
		mTotalReplace = replace;
	}

protected:
	typedef std::map<CVCardAddressBookRef, CVCardAddressBook*>	CVCardAddressBookRefMap;

	static CVCardAddressBookRefMap	sVCardAddressBooks;
	static CVCardAddressBookRef 	sVCardAddressBookRefCtr;

	CVCardAddressBookRef			mVCardAddressBookRef;

	bool					mReadOnly;
	bool					mDirty;
	bool					mTotalReplace;

	cdstring				mName;
	cdstring				mDescription;
	
	CVCardComponentDB		mVCard;
	
	// Pseudo properties used for disconnected cache
	cdstring					mETag;
	CVCardComponentRecordDB		mRecordDB;
};

typedef std::vector<CVCardAddressBook*> CVCardAddressBookList;

}	// namespace vCard

#endif	// CVCardAddressBook_H

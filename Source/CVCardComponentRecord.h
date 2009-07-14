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
	CVCardComponentRecord.h

	Author:
	Description:	records changes in components in a cached calendar
*/

#ifndef CVCardComponentRecord_H
#define CVCardComponentRecord_H

#include <map>
#include <vector>

#include "cdstring.h"

#include "CVCardAddressBookRef.h"


namespace xmllib 
{
class XMLDocument;
class XMLNode;
};

namespace vCard {

class CVCardComponentRecord;
typedef std::map<cdstring, CVCardComponentRecord> CVCardComponentRecordDB;

class CVCardAddressBook;

class CVCardComponent;

class CVCardComponentRecord
{
public:
	enum ERecordAction
	{
		eAdded	 		= 1L << 0,
		eChanged		= 1L << 1,
		eRemoved		= 1L << 2,	// Removed a component not previously recorded as being added
		eRemovedAdded	= 1L << 3	// Removed a component previously recorded as being new
	};

	static void RecordAction(CVCardComponentRecordDB& recorder, const CVCardComponent* item, ERecordAction action);

	CVCardComponentRecord(ERecordAction action, const cdstring& uid, const cdstring& rurl, const cdstring& etag)
	{
		mAction = action;
		mUID = uid;
		mRURL = rurl;
		mETag = etag;
	}
	CVCardComponentRecord(const CVCardComponent* item, ERecordAction action);
	CVCardComponentRecord(const CVCardComponentRecord& copy)
	{
		_copy_CVCardComponentRecord(copy);
	}
	virtual ~CVCardComponentRecord();

	CVCardComponentRecord& operator=(const CVCardComponentRecord& copy)
	{
		if (this != &copy)
		{
			_copy_CVCardComponentRecord(copy);
		}
		return *this;
	}
	
	ERecordAction GetAction() const
	{
		return mAction;
	}
	void SetAction(ERecordAction action)
	{
		mAction = action;
	}

	const cdstring& GetUID() const
	{
		return mUID;
	}
	const cdstring& GetRURL() const
	{
		return mRURL;
	}
	const cdstring& GetETag() const
	{
		return mETag;
	}

	void WriteXML(xmllib::XMLDocument* doc, xmllib::XMLNode* parent, const cdstring& mapkey) const;
	static void ReadXML(CVCardComponentRecordDB& recorder, const xmllib::XMLNode* node);

protected:
	ERecordAction				mAction;
	cdstring					mUID;
	cdstring					mRURL;
	cdstring					mETag;

private:
	void	_copy_CVCardComponentRecord(const CVCardComponentRecord& copy)
	{
		mAction = copy.mAction; mUID = copy.mUID; mRURL = copy.mRURL; mETag = copy.mETag;
	}
};

}	// namespace vCard

#endif	// CVCardComponentRecord_H

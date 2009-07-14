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
	CVCardSync.h

	Author:
	Description:	Synchronise two vCardendars
*/

#ifndef CVCardSync_H
#define CVCardSync_H

#include "CVCardAddressBook.h"
#include "CVCardDateTime.h"

#include "cdstring.h"

namespace vCard {

class CVCardSync
{
	class CVCardSyncData
	{
	public:
		CVCardSyncData(const cdstring& mapkey) :
			mMapKey(mapkey)
		{
		}
		CVCardSyncData(const CVCardSyncData& copy)
		{
			_copy_CVCardSyncData(copy);
		}
		~CVCardSyncData()
		{
		}
		
		CVCardSyncData& operator=(const CVCardSyncData& copy)
		{
			if (this != &copy) _copy_CVCardSyncData(copy); return *this;
		}
		
		int operator==(const CVCardSyncData& other) const
		{
			return mMapKey == other.mMapKey;
		}
	
		int operator<(const CVCardSyncData& other) const
		{
			return mMapKey < other.mMapKey;
		}
	
		const cdstring& GetMapKey() const
		{
			return mMapKey;
		}
		
	private:
		cdstring 		mMapKey;

		void _copy_CVCardSyncData(const CVCardSyncData& copy)
		{
			mMapKey = copy.mMapKey;
		}
	};
	typedef std::vector<CVCardSyncData> CVCardSyncDataList;

public:
	CVCardSync(CVCardAddressBook& src1, const CVCardAddressBook& src2)
		: mAdbk1(src1), mAdbk2(src2) {}
	~CVCardSync() {}

	void Sync();

	static int CompareComponentVersions(const CVCardComponent* comp1, const CVCardComponent* comp2);

protected:
	CVCardAddressBook&				mAdbk1;
	const CVCardAddressBook&		mAdbk2;

	void GetAllKeys(const CVCardAddressBook& adbk, CVCardSyncDataList& keys);

	void RemoveKeys(CVCardSyncDataList& keys, const CVCardComponentRecordDB& recorded, unsigned long filter);
};

}	// namespace vCard

#endif	// CVCardSync_H

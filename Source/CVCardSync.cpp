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
	CVCardSync.cpp

	Author:
	Description:	Synchronise two vCardendars
*/

#include "CVCardSync.h"

#include "CVCardDefinitions.h"
#include "CVCardVCard.h"

#include <algorithm>

using namespace vCard;

void CVCardSync::Sync()
{
	// Do this for each component list in turn:
	
	// 1. Get set of UIDs for each component in adbk1
	// 2. Get set of UIDs for each component in adbk2
	// 3. Determine set of keys in adbk1 but not in adbk2
	//  3.1 Remove from this set the ones that are recorded as new (those are new components added to adbk1)
	//  3.2 Remove the components for the remaining keys from adbk1 (they are components deleted on the server)
	//  3.3 Redo set of UIDs for each component in adbk1 if it changed in 3.2
	// 4. Determine set of keys in adbk2 but not in adbk1
	//  4.1 Remove from this set the ones recorded as deleted in adbk1 (those are deleted components to be removed)
	//  4.2 Copy the components for the remaining keys from adbk2 to adbk1 (these are new components on the server)
	// 5. Determine overlapping keys in adbk1 and adbk2
	//  5.1 Compare REV for each overlapping pair
	//    5.1.1 If adbk1 and adbk2 have REV properties use the most recent
	//    5.1.2 If adbk1 has REV or neither have REV leave adbk1 item alone
	//    5.1.3 If adbk2 has REV then replace adbk1 item with adbk2 item
	// 6 Broadcast adbk1 changes
	// Done!
	
	bool adbk1_changed = false;

	// Step 1
	CVCardSyncDataList keys1;
	GetAllKeys(mAdbk1, keys1);
	std::sort(keys1.begin(), keys1.end());

	// Step 2
	CVCardSyncDataList keys2;
	GetAllKeys(mAdbk2, keys2);
	std::sort(keys2.begin(), keys2.end());
	
	// Step 3
	CVCardSyncDataList keyset;
	std::set_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::back_inserter<CVCardSyncDataList>(keyset));
	
	// Step 3.1
	RemoveKeys(keyset, mAdbk1.GetRecording(), CVCardComponentRecord::eAdded);
	
	// Step 3.2
	for(CVCardSyncDataList::const_iterator iter = keyset.begin(); iter != keyset.end(); iter++)
	{
		mAdbk1.RemoveCardByKey((*iter).GetMapKey());
		adbk1_changed = true;
	}
	
	// Step 3.3
	if (keyset.size() != 0)
	{
		keys1.clear();
		GetAllKeys(mAdbk1, keys1);
		std::sort(keys1.begin(), keys1.end());
	}
	
	// Step 4
	keyset.clear();
	std::set_difference(keys2.begin(), keys2.end(), keys1.begin(), keys1.end(), std::back_inserter<CVCardSyncDataList>(keyset));
	
	// Step 4.1
	RemoveKeys(keyset, mAdbk1.GetRecording(), CVCardComponentRecord::eRemoved | CVCardComponentRecord::eRemovedAdded);

	// Step 4.2
	for(CVCardSyncDataList::const_iterator iter = keyset.begin(); iter != keyset.end(); iter++)
	{
		const CVCardComponent* comp = mAdbk2.GetCardByKey((*iter).GetMapKey());
		if (comp != NULL)
		{
			CVCardComponent* new_comp = comp->clone();
			new_comp->SetAddressBook(mAdbk1.GetRef());
			mAdbk1.AddCard(new_comp);
			adbk1_changed = true;
		}
	}
	
	// Step 5
	keyset.clear();
	{
		// This code is std::set_intersection except that we do out action when the items are equal

		CVCardSyncDataList::const_iterator first1 = keys1.begin();
		CVCardSyncDataList::const_iterator last1 = keys1.end();
		CVCardSyncDataList::const_iterator first2 = keys2.begin();
		CVCardSyncDataList::const_iterator last2 = keys2.end();

		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
				++first2;
			else if (*first1 < *first2)
				++first1;
			else
			{
				// Items match
				
				// Step 5.1
				// Get REV for each component
				const CVCardComponent* comp1 = mAdbk1.GetCardByKey((*first1).GetMapKey());
				const CVCardComponent* comp2 = mAdbk2.GetCardByKey((*first2).GetMapKey());
				
				CVCardDateTime dt1;
				bool has_last_rev1 = (comp1 != NULL) ? comp1->GetProperty(cVCardProperty_REV, dt1) : false;
				
				CVCardDateTime dt2;
				bool has_last_rev2 = (comp2 != NULL) ? comp2->GetProperty(cVCardProperty_REV, dt2) : false;
				
				//    5.1.3.1
				if (has_last_rev1 && has_last_rev2)
				{
					if (dt2 > dt1)
					{
						// Copy one from server
						CVCardComponent* new_comp = comp2->clone();
						new_comp->SetAddressBook(mAdbk1.GetRef());
						mAdbk1.AddCard(new_comp);
					}
				}
				//    5.1.3.2
				else if (has_last_rev1 || (!has_last_rev1 && !has_last_rev2))
				{
					// Leave alone
				}
				//    5.1.3.3
				else if (has_last_rev2)
				{
					// Copy one from server
					CVCardComponent* new_comp = comp2->clone();
					new_comp->SetAddressBook(mAdbk1.GetRef());
					mAdbk1.AddCard(new_comp);
				}

				++first1;
				++first2;
			}
		}
	}

	// Step 6
	if (adbk1_changed) ;
}

// NB Assumes components are the same but different versions
int CVCardSync::CompareComponentVersions(const CVCardComponent* comp1, const CVCardComponent* comp2)
{
	// Compare REV
	CVCardDateTime dt1;
	bool has_last_rev1 = (comp1 != NULL) ? comp1->GetProperty(cVCardProperty_REV, dt1) : false;
	
	CVCardDateTime dt2;
	bool has_last_rev2 = (comp2 != NULL) ? comp2->GetProperty(cVCardProperty_REV, dt2) : false;
	
	if (has_last_rev1 && has_last_rev2)
	{
		if (dt1 == dt2)
			return 0;
		return dt1 < dt2 ? -1 : 1;
	}

	else if (has_last_rev1)
	{
		return 1;
	}

	else if (has_last_rev2)
	{
		return -1;
	}
	else
		return 0;
}

// Get keys of all components
void CVCardSync::GetAllKeys(const CVCardAddressBook& adbk, CVCardSyncDataList& keys)
{
	for(CVCardComponentDB::const_iterator iter = adbk.GetVCards().begin(); iter != adbk.GetVCards().end(); iter++)
	{
		keys.push_back(CVCardSyncData((*iter).second->GetMapKey()));
	}
}

void CVCardSync::RemoveKeys(CVCardSyncDataList& keys, const CVCardComponentRecordDB& recorded, unsigned long filter)
{
	// Find recorded keys matching filter
	CVCardSyncDataList recorded_keys;
	for(CVCardComponentRecordDB::const_iterator iter = recorded.begin(); iter != recorded.end(); iter++)
	{
		if (((*iter).second.GetAction() & filter) != 0)
			recorded_keys.push_back(CVCardSyncData((*iter).first));
	}
	
	// Now remove recorded keys from key set
	if (recorded_keys.size() != 0)
	{
		CVCardSyncDataList result;
		std::set_difference(keys.begin(), keys.end(), recorded_keys.begin(), recorded_keys.end(), std::back_inserter<CVCardSyncDataList>(result));
		if (result.size() != keys.size())
			keys = result;
	}
}

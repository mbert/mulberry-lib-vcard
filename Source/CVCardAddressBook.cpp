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
	CVCardAddressBook.cpp

	Author:
	Description:	<describe the CVCardAddressBook class here>
*/

#include "CVCardAddressBook.h"

#include "CVCardDefinitions.h"
#include "CVCardTextValue.h"
#include "CVCardVCard.h"
#include "CVCardUtils.h"

#include "XMLDocument.h"
#include "XMLNode.h"
#include "XMLSAXSimple.h"

#include <algorithm>
#include <cstring>
#include <memory>

using namespace vCard;

CVCardAddressBook::CVCardAddressBookRefMap CVCardAddressBook::sVCardAddressBooks;
CVCardAddressBookRef CVCardAddressBook::sVCardAddressBookRefCtr = 1;

CVCardAddressBook* CVCardAddressBook::GetVCardAddressBook(const CVCardAddressBookRef& ref)
{
	CVCardAddressBookRefMap::iterator found = sVCardAddressBooks.find(ref);
	if (found != sVCardAddressBooks.end())
		return (*found).second;
	else
		return NULL;
}

CVCardAddressBook::CVCardAddressBook()
{
	mVCardAddressBookRef = sVCardAddressBookRefCtr++;
	sVCardAddressBooks.insert(CVCardAddressBookRefMap::value_type(mVCardAddressBookRef, this));

	mReadOnly = false;
	mDirty = false;
}

CVCardAddressBook::~CVCardAddressBook()
{
	// Broadcast closing before removing components
	Broadcast_Message(eBroadcast_Closed, this);

	// Clean up the map items
	mVCard.RemoveAllComponents();

	sVCardAddressBooks.erase(mVCardAddressBookRef);
}

void CVCardAddressBook::Clear()
{
	mName = cdstring::null_str;
	mDescription = cdstring::null_str;
	mReadOnly = false;
	mDirty = false;

	// Remove recording of old data - but leave the etag as-is
	ClearRecording();

	// Remove existing calendar properties then add back the defaults
	mProperties.clear();

	// Clean up the map items
	mVCard.RemoveAllComponents();
}

void CVCardAddressBook::Finalise()
{
}

void CVCardAddressBook::EditName(const cdstring& name)
{
	if (mName != name)
	{
		// Updated cached value
		mName = name;

		// Mark as dirty
		SetDirty();
		
		// Broadcast change
		Broadcast_Message(eBroadcast_Edit, this);
	}
}

void CVCardAddressBook::EditDescription(const cdstring& description)
{
	if (mDescription != description)
	{
		// Updated cached value
		mDescription = description;

		// Mark as dirty
		SetDirty();
		
		// Broadcast change
		Broadcast_Message(eBroadcast_Edit, this);
	}
}

bool CVCardAddressBook::Parse(std::istream& is)
{
	bool result = false;

	enum EParserState
	{
		eLookForVCard,
		eGetComponentProperty,
	};

	EParserState state = eLookForVCard;

	// Get lines looking for start of calendar
	cdstring line1;
	cdstring line2;
	CVCardComponent* comp = NULL;

	while(!is.fail() && CVCardUtils::ReadFoldedLine(is, line1, line2))
	{
		switch(state)
		{
		case eLookForVCard:
			// Look for start
			if (line1.compare(cVCardComponent_BEGINVCARD) == 0)
			{
				// Next state
				state = eGetComponentProperty;

				// Indicate success at this point
				result = true;
				
				comp = new CVCardVCard(GetRef());
			}
			break;
		case eGetComponentProperty:
			// Look for end of current component
			if (line1.compare(cVCardComponent_ENDVCARD) == 0)
			{
				// Finalise the component (this caches data from the properties)
				comp->Finalise();

				// Check for valid component
				if (!mVCard.AddComponent(comp))
					delete comp;
				comp = NULL;

				// Reset state
				state = eLookForVCard;
			}
			else
			{
				// Parse attribute/value and store in component
				CVCardProperty prop;
				if (prop.Parse(line1))
					comp->AddProperty(prop);
			}
			break;
		}
	}

	return result;
}

vCard::CVCardVCard* CVCardAddressBook::ParseComponent(std::istream& is, const cdstring& rurl, const cdstring& etag)
{
	CVCardVCard* result = NULL;

	enum EParserState
	{
		eLookForVCard,
		eGetComponentProperty,
		eGotVCard
	};

	EParserState state = eLookForVCard;

	// Get lines looking for start of calendar
	cdstring line1;
	cdstring line2;
	CVCardVCard* comp = NULL;

	while(!is.fail() && CVCardUtils::ReadFoldedLine(is, line1, line2))
	{
		switch(state)
		{
		case eLookForVCard:
			// Look for start
			if (line1.compare(cVCardComponent_BEGINVCARD) == 0)
			{
				// Next state
				state = eGetComponentProperty;
				
				comp = new CVCardVCard(GetRef());
				result = comp;
			}
			break;
		case eGetComponentProperty:
			// Look for end of current component
			if (line1.compare(cVCardComponent_ENDVCARD) == 0)
			{
				// Finalise the component (this caches data from the properties)
				comp->Finalise();
				comp->SetRURL(rurl);
				comp->SetETag(etag);

				// Check for valid component
				if (!mVCard.AddComponent(comp))
				{
					if (result == comp)
						result = NULL;
					delete comp;
					comp = NULL;
					state = eLookForVCard;
				}
				else
					// Reset state
					state = eGotVCard;
			}
			else
			{
				// Parse attribute/value and store in component
				CVCardProperty prop;
				if (prop.Parse(line1))
					comp->AddProperty(prop);
			}
			break;
		default:;
		}
		
		// Exit if we have one - ignore all the rest
		if (state == eGotVCard)
			break;
	}
	
	return result;
}

void CVCardAddressBook::Generate(std::ostream& os, bool for_cache) const
{
	// Write entire DB
	for(CVCardComponentDB::const_iterator iter = mVCard.begin(); iter != mVCard.end(); iter++)
		(*iter).second->Generate(os, for_cache);
}

void CVCardAddressBook::GenerateOne(std::ostream& os, const CVCardVCard& comp) const
{
	// Write the component
	comp.Generate(os);
}

// Get components based on requirements

void CVCardAddressBook::ChangedComponent(CVCardComponent* comp)
{
	// Calendar has changed
	SetDirty();
	
	// Record change
	CVCardComponentRecord::RecordAction(mRecordDB, comp, CVCardComponentRecord::eChanged);
	
	// Broadcast change
	CComponentAction action(CComponentAction::eChanged, *this, *comp);
	Broadcast_Message(eBroadcast_ChangedComponent, &action);
}

void CVCardAddressBook::AddNewVCard(CVCardVCard* vcard, bool moved)
{
	// Do not init props if moving
	if (!moved)
	{
		// Make sure UID is set and unique
		cdstring uid;
		vcard->SetUID(uid);
		
		// Init REV to now
		vcard->InitRevision();
	}

	mVCard.AddComponent(vcard);

	// Calendar has changed
	SetDirty();
	
	// Record change
	CVCardComponentRecord::RecordAction(mRecordDB, vcard, CVCardComponentRecord::eAdded);
	
	// Broadcast change
	CComponentAction action(CComponentAction::eAdded, *this, *vcard);
	Broadcast_Message(eBroadcast_AddedComponent, &action);
}

void CVCardAddressBook::RemoveVCard(CVCardVCard* vcard, bool delete_it)
{
	// Record change  before delete occurs
	CVCardComponentRecord::RecordAction(mRecordDB, vcard, CVCardComponentRecord::eRemoved);

	// Remove from the map (do not delete here - wait until after broadcast)
	mVCard.RemoveComponent(vcard, false);
	
	// Calendar has changed
	SetDirty();
	
	// Broadcast change
	CComponentAction action(CComponentAction::eRemoved, *this, *vcard);
	Broadcast_Message(eBroadcast_RemovedComponent, &action);
	
	// Delete it here after all changes
	if (delete_it)
		delete vcard;
}

void CVCardAddressBook::AddCard(CVCardComponent* comp)
{
	// Just add it without doing anything as this is a copy being made during sync'ing
	if (!mVCard.AddComponent(comp))
		delete comp;
}

const CVCardVCard* CVCardAddressBook::GetCardByKey(const cdstring& mapkey) const
{
	CVCardComponentDB::const_iterator found = mVCard.find(mapkey);
	if (found != mVCard.end())
		return static_cast<const CVCardVCard*>((*found).second);
	else
		return NULL;
}

CVCardVCard* CVCardAddressBook::GetCardByKey(const cdstring& mapkey)
{
	CVCardComponentDB::iterator found = mVCard.find(mapkey);
	if (found != mVCard.end())
		return static_cast<CVCardVCard*>((*found).second);
	else
		return NULL;
}

void CVCardAddressBook::RemoveCardByKey(const cdstring& mapkey)
{
	CVCardVCard* result = GetCardByKey(mapkey);
	if (result != NULL)
	{
		mVCard.RemoveComponent(result, true);
	}
}

#pragma mark ____________________________Disconnected

// XML DTD
/*
	<!ELEMENT calendarstate	(etag, recordlist) >
	<!ATTLIST calendarstate	version			CDATA	#REQUIRED >
	
	<!ELEMENT etag			(#PCDATA) >
	
	<!ELEMENT recordlist	(record*) >
	
	<!ELEMENT record		(uid) >
	<!ATTLIST action		(added|changed|removed|removedadded) #REQUIRED
			  mapkey		CDATA #REQUIRED >

	<!ELEMENT uid			(#PCDATA) >
	
*/

static const char* cXMLElement_calendarstate	= "calendarstate";
static const char* cXMLAttribute_version		= "version";

static const char* cXMLElement_etag				= "etag";
static const char* cXMLElement_sync_token		= "sync-token";

static const char* cXMLElement_recordlist		= "recordlist";

#if 0
static const char* cXMLElement_record			= "record";
static const char* cXMLAttribute_action		= "action";
static const char* cXMLAttribute_action_added			= "added";
static const char* cXMLAttribute_action_changed		= "changed";
static const char* cXMLAttribute_action_removed		= "removed";
static const char* cXMLAttribute_action_removedadded	= "removedadded";
static const char* cXMLAttribute_mapkey		= "mapkey";

static const char* cXMLElement_uid				= "uid";
#endif

void CVCardAddressBook::ParseCache(std::istream& is)
{
	// Init the cached data first
	mETag = cdstring::null_str;
	mSyncToken = cdstring::null_str;
	mRecordDB.clear();

	// XML parse the data
	xmllib::XMLSAXSimple parser;
	parser.ParseStream(is);

	// See if we got any valid XML
	if (parser.Document())
	{
		// Check root node
		xmllib::XMLNode* root = parser.Document()->GetRoot();
		if (root->Name() != cXMLElement_calendarstate)
			return;
		
		// Get ETag
		const xmllib::XMLNode* etagnode = root->GetChild(cXMLElement_etag);
		if (etagnode == NULL)
			return;
		etagnode->DataValue(mETag);
		
		// Get sync-token
		const xmllib::XMLNode* synctokennode = root->GetChild(cXMLElement_sync_token);
		if (synctokennode != NULL)
			synctokennode->DataValue(mSyncToken);
		
		// Get recordlist node
		const xmllib::XMLNode* recordlistnode = root->GetChild(cXMLElement_recordlist);
		if (recordlistnode != NULL)
		{
			// Now look at each child
			for(xmllib::XMLNodeList::const_iterator iter = recordlistnode->Children().begin(); iter != recordlistnode->Children().end(); iter++)
			{
				CVCardComponentRecord::ReadXML(mRecordDB, *iter);
			}
		}
	}
}

void CVCardAddressBook::GenerateCache(std::ostream& os) const
{
	// Create XML document object
	std::auto_ptr<xmllib::XMLDocument> doc(new xmllib::XMLDocument);
	
	// Root element is the preferences element
	doc->GetRoot()->SetName(cXMLElement_calendarstate);
	doc->GetRoot()->AddAttribute(cXMLAttribute_version, "1");
	
	// Create etag child node
	new xmllib::XMLNode(doc.get(), doc->GetRoot(), cXMLElement_etag, GetETag());
	
	// Create sync-token child node
	new xmllib::XMLNode(doc.get(), doc->GetRoot(), cXMLElement_sync_token, GetSyncToken());
	
	// Create recordlist child node
	xmllib::XMLNode* recordlistnode = new xmllib::XMLNode(doc.get(), doc->GetRoot(), cXMLElement_recordlist);
	
	// Now add each node recorded item
	for(CVCardComponentRecordDB::const_iterator iter = mRecordDB.begin(); iter != mRecordDB.end(); iter++)
	{
		(*iter).second.WriteXML(doc.get(), recordlistnode, (*iter).first);
	}
	
	// Write to stream
	doc->Generate(os);
}

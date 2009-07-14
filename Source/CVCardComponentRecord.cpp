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
	CVCardComponentRecord.cpp

	Author:
	Description:	records changes in components in a cached calendar
*/

#include "CVCardComponentRecord.h"

#include "CVCardComponent.h"

#include "XMLDocument.h"
#include "XMLNode.h"

using namespace vCard;

void CVCardComponentRecord::RecordAction(CVCardComponentRecordDB& recorder, const CVCardComponent* item, ERecordAction action)
{
	// See if item is already present
	cdstring key = item->GetMapKey();
	CVCardComponentRecordDB::iterator found = recorder.find(key);
	if (found == recorder.end())
	{
		// Add new record
		CVCardComponentRecord record(item, action);
		recorder.insert(CVCardComponentRecordDB::value_type(item->GetMapKey(), record));
	}
	else
	{
		// Check type of existing record and base current recording on that
		switch((*found).second.GetAction())
		{
		case eAdded:
			switch(action)
			{
			case eAdded:
				// This is a bug - cannot add the component when its already present
				// Just ignore the current record request
				break;
			case eChanged:
				// A changed new component is a new component so just ignore current record request
				break;
			case eRemoved:
				// A removed new component should be recorded differently than a removed old component
				(*found).second.SetAction(eRemovedAdded);
				break;
			default:;
			}
			break;
		case eChanged:
			switch(action)
			{
			case eAdded:
				// This is a bug - cannot add the component when its already present
				// Just ignore the current record request
				break;
			case eChanged:
				// Already recorded as changed so just ignore current record request
				break;
			case eRemoved:
				// A removed changed component is removed so modify existing record
				(*found).second.SetAction(eRemoved);
				break;
			default:;
			}
			break;
		case eRemoved:
		case eRemovedAdded:
			// This is a bug - cannot change a component already removed
			// Just ignore the current record request
			break;
		default:;
		}
		
	}
}

CVCardComponentRecord::CVCardComponentRecord(const CVCardComponent* item, ERecordAction action)
{
	mAction = action;
	mUID = item->GetUID();
	mRURL = item->GetRURL();
	mETag = item->GetETag();
}

CVCardComponentRecord::~CVCardComponentRecord()
{
}

static const char* cXMLElement_record			= "record";
static const char* cXMLAttribute_action		= "action";
static const char* cXMLAttribute_action_added			= "added";
static const char* cXMLAttribute_action_changed		= "changed";
static const char* cXMLAttribute_action_removed		= "removed";
static const char* cXMLAttribute_action_removedadded	= "removedadded";
static const char* cXMLAttribute_mapkey		= "mapkey";

static const char* cXMLElement_uid				= "uid";

static const char* cXMLElement_rurl			= "rurl";

static const char* cXMLElement_etag			= "etag";

void CVCardComponentRecord::WriteXML(xmllib::XMLDocument* doc, xmllib::XMLNode* parent, const cdstring& mapkey) const
{
	// Create new node
	xmllib::XMLNode* xmlnode = new xmllib::XMLNode(doc, parent, cXMLElement_record);
	
	// Set action attribute
	switch(mAction)
	{
	case eAdded:
		xmlnode->AddAttribute(cXMLAttribute_action, cXMLAttribute_action_added);
		break;
	case eChanged:
		xmlnode->AddAttribute(cXMLAttribute_action, cXMLAttribute_action_changed);
		break;
	case eRemoved:
		xmlnode->AddAttribute(cXMLAttribute_action, cXMLAttribute_action_removed);
		break;
	case eRemovedAdded:
		xmlnode->AddAttribute(cXMLAttribute_action, cXMLAttribute_action_removedadded);
		break;
	default:;
	}
	
	// Set map key attribute
	xmlnode->AddAttribute(cXMLAttribute_mapkey, mapkey);
	
	// Add UID child node
	new xmllib::XMLNode(doc, xmlnode, cXMLElement_uid, mUID);
	
	// Add RURL child node
	new xmllib::XMLNode(doc, xmlnode, cXMLElement_rurl, mRURL);
	
	// Add ETAG child node
	new xmllib::XMLNode(doc, xmlnode, cXMLElement_etag, mETag);
	
}

void CVCardComponentRecord::ReadXML(CVCardComponentRecordDB& recorder, const xmllib::XMLNode* xmlnode)
{
	// Must have right type of node
	if (xmlnode->Name() != cXMLElement_record)
		return;
	
	// Get action
	ERecordAction action;
	cdstring temp;
	if (!xmlnode->AttributeValue(cXMLAttribute_action, temp))
		return;
	if (temp == cXMLAttribute_action_added)
		action = eAdded;
	else if (temp == cXMLAttribute_action_changed)
		action = eChanged;
	else if (temp == cXMLAttribute_action_removed)
		action = eRemoved;
	else if (temp == cXMLAttribute_action_removedadded)
		action = eRemovedAdded;
	
	// Get mapkey
	cdstring mapkey;
	if (!xmlnode->AttributeValue(cXMLAttribute_mapkey, mapkey))
		return;

	// Get child nodes
	cdstring uid;
	cdstring rurl;
	cdstring etag;
	
	const xmllib::XMLNode* node = xmlnode->GetChild(cXMLElement_uid);
	if (node != NULL)
		node->DataValue(uid);
	
	node = xmlnode->GetChild(cXMLElement_rurl);
	if (node != NULL)
		node->DataValue(rurl);
	
	node = xmlnode->GetChild(cXMLElement_etag);
	if (node != NULL)
		node->DataValue(etag);
	
	// Now create the new record and add to DB
	CVCardComponentRecord record(action, uid, rurl, etag);
	recorder.insert(CVCardComponentRecordDB::value_type(mapkey, record));
}

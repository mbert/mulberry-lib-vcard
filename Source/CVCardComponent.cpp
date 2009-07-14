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
	CVCardComponent.cpp

	Author:
	Description:	<describe the CVCardComponent class here>
*/

#include "CVCardComponent.h"

#include "CVCardAddressBook.h"
#include "CVCardDateTimeValue.h"
#include "CVCardDefinitions.h"

#ifdef __MULBERRY
#include "CTCPSocket.h"
#endif

#include <algorithm>

using namespace vCard;

CVCardComponent::~CVCardComponent()
{
}

void CVCardComponent::_copy_CVCardComponent(const CVCardComponent& copy)
{
	mAddressBookRef = copy.mAddressBookRef;
	mUID = copy.mUID;

	mETag = copy.mETag;
	mRURL = copy.mRURL;
	mChanged = copy.mChanged;
}

bool CVCardComponent::AddComponent(CVCardComponent* comp)
{
	// Sub-classes decide what can be embedded
	return false;
}

void CVCardComponent::RemoveComponent(CVCardComponent* comp)
{
}

void CVCardComponent::SetUID(const cdstring& uid)
{
	if (uid.length())
		mUID = uid;
	else
	{
		// Get left-side of UID (first 24 chars of MD5 digest of time, clock and ctr)
		static unsigned long ctr = 1;
		cdstring lhs_txt;
		lhs_txt.reserve(256);
		::snprintf(lhs_txt.c_str_mod(), 256, "%lu.%lu.%lu", (time_t) clock(), time(NULL), ctr++);
		cdstring lhs;
		lhs_txt.md5(lhs);
		lhs[24UL] = 0;

		// Get right side (domain) of message-id
		cdstring rhs;
#ifdef __MULBERRY
		cdstring host = CTCPSocket::TCPGetLocalHostName();
		host.trimspace();
		if (host.length())
		{
			// Must put IP numbers inside [..]
			if (CTCPSocket::TCPIsHostName(host))
				rhs = host;
			else
			{
				rhs = "[";
				rhs += host;
				rhs += "]";
			}
		}
		else
#endif
		{
			// Use app name
			cdstring domain("mulberry");
			domain += cdstring(ctr);
				
			// Use first 24 chars of MD5 digest of the domain as the right-side of message-id
			domain.md5(rhs);
			rhs[24UL] = 0;
		}

		// Generate the UID string
		cdstring uid;
		uid += lhs;
		uid += "@";
		uid += rhs;

		mUID = uid;
	}

	RemoveProperties(cVCardProperty_UID);

	CVCardProperty prop(cVCardProperty_UID, mUID);
	AddProperty(prop);
}

void CVCardComponent::GenerateRURL()
{
	// Format is:
	//
	// <<hash code>> *("-0"> .vcs
	if (mRURL.empty())
	{
		// Generate hash code
		cdstring hash;
		hash += GetMapKey();
		hash += ":";

		CVCardDateTime dt;
		if (LoadValue(cVCardProperty_REV, dt))
		{
			hash += dt.GetText();
		}
		
		hash.md5(mRURL);
		
		// Truncate at 16 chars
		hash.erase(16);
	}
	else
	{
		// Strip off .vcf
		mRURL.erase(mRURL.rfind(".vcf", cdstring::npos, 4, true));
	}
	
	// Add trailer
	mRURL += "-0.vcf";
}

void CVCardComponent::InitRevision()
{
	RemoveProperties(cVCardProperty_REV);

	CVCardProperty prop(cVCardProperty_REV, CVCardDateTime::GetNowUTC());
	AddProperty(prop);
}

void CVCardComponent::UpdateRevision()
{
	RemoveProperties(cVCardProperty_REV);

	CVCardProperty prop(cVCardProperty_REV, CVCardDateTime::GetNowUTC());
	AddProperty(prop);
}

void CVCardComponent::Added()
{
	mChanged = true;
}

void CVCardComponent::Removed()
{
	mChanged = true;
}

void CVCardComponent::Duplicated()
{
	// Remove SEQ, REV
	// These will be re-created when it is added to the address book
	RemoveProperties(cVCardProperty_UID);
	RemoveProperties(cVCardProperty_REV);

	// Remove the cached values as well
	mUID.clear();
	
	// Reset CardDAV items
	mETag = cdstring::null_str;
	mRURL = cdstring::null_str;
	mChanged = true;
}

void CVCardComponent::Changed()
{
	// Update REV
	UpdateRevision();

	mChanged = true;

	// Mark address book as dirty
	CVCardAddressBook* adbk = CVCardAddressBook::GetVCardAddressBook(GetAddressBook());
	if (adbk != NULL)
		adbk->ChangedComponent(this);
}

void CVCardComponent::Finalise()
{
	// Get UID
	LoadValue(cVCardProperty_UID, mUID);

	// Get CardDAV info if present
	LoadPrivateValue(cVCardProperty_X_PRIVATE_RURL, mRURL);
	LoadPrivateValue(cVCardProperty_X_PRIVATE_ETAG, mETag);
}

void CVCardComponent::Generate(std::ostream& os, bool for_cache) const
{
	// Header
	os << GetBeginDelimiter() << std::endl;

	// Write each property
	WriteProperties(os);
	
	// Do private properties if caching
	if (for_cache)
	{
		if (!mRURL.empty())
			WritePrivateProperty(os, cVCardProperty_X_PRIVATE_RURL, mRURL);
		if (!mETag.empty())
			WritePrivateProperty(os, cVCardProperty_X_PRIVATE_ETAG, mETag);
	}

	// Footer
	os << GetEndDelimiter() << std::endl;
}

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
	CVCardVCard.cpp

	Author:
	Description:	<describe the CVCardVCard class here>
*/

#include "CVCardVCard.h"

#include "CVCardDefinitions.h"

using namespace vCard;

cdstring CVCardVCard::sBeginDelimiter(cVCardComponent_BEGINVCARD);
cdstring CVCardVCard::sEndDelimiter(cVCardComponent_ENDVCARD);

void CVCardVCard::_init_CVCardVCard()
{
}

void CVCardVCard::_copy_CVCardVCard(const CVCardVCard& copy)
{
}

void CVCardVCard::_tidy_CVCardVCard()
{
}

void CVCardVCard::Initialise()
{
	// Do inherited
	//CVCardComponent::Initialise();

	RemoveProperties(cVCardProperty_VERSION);

	CVCardProperty prop(cVCardProperty_VERSION, cVCardValue_VERSION);
	AddProperty(prop);
}

void CVCardVCard::Finalise()
{
	// Do inherited
	CVCardComponent::Finalise();

	if (!HasProperty(cVCardProperty_UID))
	{
		// Force a UID value if one is not present
		
		// What we really want to do is hash the name and use that,
		// but we're not caching that right now so just do normal UID set

		SetUID(cdstring::null_str);
	}
}


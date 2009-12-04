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
	CVCardComponentDB.cpp

	Author:
	Description:	map class to hold UID->component mapping as well as UID->Recurrence instance mapping
*/

#include "CVCardComponentDB.h"

#include "CVCardComponent.h"

using namespace vCard;

bool CVCardComponentDB::AddComponent(CVCardComponent* comp)
{
	// Must have valid UID
	if (comp->GetMapKey().length() == 0)
		return false;

	// Add the component to the addressbook
	bool bresult = true;
	std::pair<CVCardComponentMap::iterator, bool> result = insert(CVCardComponentMap::value_type(comp->GetMapKey(), comp));

	// See if duplicate
	if (!result.second)
	{
		// Do not replace if one already exists
		bresult = false;
	}
	else
		bresult = true;
	
	// Tell component it has now been added
	if (bresult)
		comp->Added();

	return bresult;
}

void CVCardComponentDB::RemoveComponent(CVCardComponent* comp, bool delete_it)
{
	// Tell component it is removed
	comp->Removed();

	// Only if present
	erase(comp->GetMapKey());
	
	// Delete if required
	if (delete_it)
		delete comp;
}

void CVCardComponentDB::RemoveAllComponents()
{
	for(CVCardComponentDB::const_iterator iter = begin(); iter != end(); iter++)
	{
		// Tell component it is removed and delete it
		(*iter).second->Removed();
		delete (*iter).second;
	}
	
	clear();
}

void CVCardComponentDB::ChangedComponent(CVCardComponent* comp)
{
	// Tell component it is changed
	comp->Changed();
}

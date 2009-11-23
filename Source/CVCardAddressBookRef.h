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
	CVCardAddressBookRef.h

	Author:
	Description:	<describe the CICalendar class here>
*/

#ifndef CVCardAddressBookRef_H
#define CVCardAddressBookRef_H

#include <stdint.h>

namespace vCard {

typedef uint32_t	CVCardAddressBookRef;	// Unique reference to object

const CVCardAddressBookRef cAddressBookRef_Invalid = 0;

}	// namespace vCard

#endif	// CVCardAddressBookRef_H

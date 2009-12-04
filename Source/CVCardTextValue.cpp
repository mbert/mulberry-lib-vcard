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
	CVCardTextValue.cpp

	Author:
	Description:	<describe the CVCardTextValue class here>
*/

#include "CVCardTextValue.h"

#include "CVCardUtils.h"

using namespace vCard;

void CVCardTextValue::Parse(const cdstring& data)
{
	mValue = CVCardUtils::DecodeTextValue(data);
}

void CVCardTextValue::Generate(std::ostream& os) const
{
	CVCardUtils::WriteTextValue(os, mValue);
}


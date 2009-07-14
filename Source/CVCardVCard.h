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
	CVCardVCard.h

	Author:
	Description:	<describe the CVCardVCard class here>
*/

#ifndef CVCardVCard_H
#define CVCardVCard_H

#include "CVCardComponent.h"

namespace vCard {

class CVCardVCard: public CVCardComponent
{
public:
	CVCardVCard(const CVCardAddressBookRef& adbk) :
		CVCardComponent(adbk)
		{ _init_CVCardVCard(); }
	CVCardVCard(const CVCardVCard& copy) :
		CVCardComponent(copy)
		{ _copy_CVCardVCard(copy); }
	virtual ~CVCardVCard()
		{ _tidy_CVCardVCard(); }

	virtual CVCardComponent* clone() const
	{
		return new CVCardVCard(*this);
	}

	CVCardVCard& operator=(const CVCardVCard& copy)
		{
			if (this != &copy)
			{
				_copy_CVCardVCard(copy);
				CVCardComponent::operator=(copy);
			}
			return *this;
		}

	virtual EComponentType GetType() const
		{ return eVCARD; }
	virtual const cdstring& GetBeginDelimiter() const
		{ return sBeginDelimiter; }
	virtual const cdstring& GetEndDelimiter() const
		{ return sEndDelimiter; }

	virtual void Initialise();
	virtual void Finalise();

	// Editing

	// Generating info

private:
	static cdstring		sBeginDelimiter;
	static cdstring		sEndDelimiter;

	void	_init_CVCardVCard();
	void	_copy_CVCardVCard(const CVCardVCard& copy);
	void	_tidy_CVCardVCard();
};

}	// namespace vCard

#endif	// CVCardVCard_H

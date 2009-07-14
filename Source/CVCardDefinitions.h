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
	CVCardDefinitions.h

	Author:
	Description:	<describe the CVCardDefinitions class here>
*/

#ifndef CVCardDefinitions_H
#define CVCardDefinitions_H

namespace vCard {

// 2426 Component Header/Footer

extern const char* cVCardComponent_BEGINVCARD;
extern const char* cVCardComponent_ENDVCARD;


// 2426 vCard Properties/Attributes

extern const char* cVCardProperty_NAME;

extern const char* cVCardProperty_PROFILE;
	extern const char* cVCardValue_PROFILE;

extern const char* cVCardProperty_SOURCE;

extern const char* cVCardProperty_FN;

extern const char* cVCardProperty_N;

extern const char* cVCardProperty_NICKNAME;

extern const char* cVCardProperty_PHOTO;

extern const char* cVCardProperty_BDAY;

extern const char* cVCardProperty_ADR;

extern const char* cVCardProperty_LABEL;

extern const char* cVCardProperty_TEL;

extern const char* cVCardProperty_EMAIL;

extern const char* cVCardProperty_MAILER;

extern const char* cVCardProperty_TZ;

extern const char* cVCardProperty_GEO;

extern const char* cVCardProperty_TITLE;

extern const char* cVCardProperty_ROLE;

extern const char* cVCardProperty_LOGO;

extern const char* cVCardProperty_AGENT;

extern const char* cVCardProperty_CALENDAR_ADDRESS;

extern const char* cVCardProperty_ORG;

extern const char* cVCardProperty_CATEGORIES;

extern const char* cVCardProperty_NOTE;

extern const char* cVCardProperty_PRODID;

extern const char* cVCardProperty_REV;

extern const char* cVCardProperty_SORT_STRING;

extern const char* cVCardProperty_SOUND;

extern const char* cVCardProperty_UID;

extern const char* cVCardProperty_URL;

extern const char* cVCardProperty_VERSION;
	extern const char* cVCardValue_VERSION;

extern const char* cVCardProperty_CLASS;
	extern const char* cVCardValue_CLASS_PUBLIC;
	extern const char* cVCardValue_CLASS_PRIVATE;
	extern const char* cVCardValue_CLASS_CONFIDENTIAL;

extern const char* cVCardProperty_KEY;

// 2426 vCard Property Atrributes

// 2425 MIME-DIR Parameters
extern const char* cVCardAttribute_ENCODING;
	extern const char* cVCardAttribute_ENCODING_B;
extern const char* cVCardAttribute_VALUE;
extern const char* cVCardAttribute_LANGUAGE;
extern const char* cVCardAttribute_CONTEXT;

// draft-jennings-impp-vcard-05
extern const char* cVCardProperty_IMPP;

// 2426 vCard Parameters
extern const char* cVCardAttribute_TYPE;

	extern const char* cVCardAttribute_TYPE_PREF;

	extern const char* cVCardAttribute_TYPE_TEL_HOME;
	extern const char* cVCardAttribute_TYPE_TEL_WORK;
	extern const char* cVCardAttribute_TYPE_TEL_VOICE;
	extern const char* cVCardAttribute_TYPE_TEL_FAX;
	extern const char* cVCardAttribute_TYPE_TEL_MSG;
	extern const char* cVCardAttribute_TYPE_TEL_CELL;
	extern const char* cVCardAttribute_TYPE_TEL_PAGER;
	extern const char* cVCardAttribute_TYPE_TEL_BBS;
	extern const char* cVCardAttribute_TYPE_TEL_MODEM;
	extern const char* cVCardAttribute_TYPE_TEL_CAR;
	extern const char* cVCardAttribute_TYPE_TEL_ISDN;
	extern const char* cVCardAttribute_TYPE_TEL_VIDEO;
	extern const char* cVCardAttribute_TYPE_TEL_PCS;

	extern const char* cVCardAttribute_TYPE_EMAIL_INTERNET;
	extern const char* cVCardAttribute_TYPE_EMAIL_X400;

	extern const char* cVCardAttribute_TYPE_KEY_X509;
	extern const char* cVCardAttribute_TYPE_KEY_PGP;

	extern const char* cVCardAttribute_TYPE_ADR_DOM;
	extern const char* cVCardAttribute_TYPE_ADR_INTL;
	extern const char* cVCardAttribute_TYPE_ADR_POSTAL;
	extern const char* cVCardAttribute_TYPE_ADR_PARCEL;
	extern const char* cVCardAttribute_TYPE_ADR_HOME;
	extern const char* cVCardAttribute_TYPE_ADR_WORK;

// draft-jennings-impp-vcard-05 Parameters
	extern const char* cVCardAttribute_TYPE_IMPP_PERSONAL;
	extern const char* cVCardAttribute_TYPE_IMPP_BUSINESS;
	extern const char* cVCardAttribute_TYPE_IMPP_HOME;
	extern const char* cVCardAttribute_TYPE_IMPP_WORK;
	extern const char* cVCardAttribute_TYPE_IMPP_MOBILE;

// Value types

// 2425 MIME-DIR
extern const char* cVCardValue_URI;
extern const char* cVCardValue_TEXT;
extern const char* cVCardValue_DATE;
extern const char* cVCardValue_TIME;
extern const char* cVCardValue_DATE_TIME;
extern const char* cVCardValue_INTEGER;
extern const char* cVCardValue_BOOLEAN;
extern const char* cVCardValue_FLOAT;

// 2426 vCard
extern const char* cVCardValue_BINARY;
extern const char* cVCardValue_UTC_OFFSET;

// Parameter Values

// 2425 MIME-DIR

extern const char* cVCardValue_BOOLEAN_TRUE;
extern const char* cVCardValue_BOOLEAN_FALSE;

// Mulberry extensions
extern const char* cVCardProperty_X_PRIVATE_RURL;
extern const char* cVCardProperty_X_PRIVATE_ETAG;


}	// namespace vCard

#endif	// CVCardDefinitions_H

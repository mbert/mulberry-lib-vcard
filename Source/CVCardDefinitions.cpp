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
	CVCardDefinitions.cpp

	Author:
	Description:	<describe the CVCardDefinitions class here>
*/

#include "CVCardDefinitions.h"

namespace vCard
{

// 2426 Component Header/Footer

const char* cVCardComponent_BEGINVCARD = "BEGIN:VCARD";
const char* cVCardComponent_ENDVCARD = "END:VCARD";


// 2426 vCard Properties/Attributes

const char* cVCardProperty_NAME = "NAME";

const char* cVCardProperty_PROFILE = "PROFILE";
	const char* cVCardValue_PROFILE = "VCARD";

const char* cVCardProperty_SOURCE = "SOURCE";

const char* cVCardProperty_FN = "FN";

const char* cVCardProperty_N = "N";

const char* cVCardProperty_NICKNAME = "NICKNAME";

const char* cVCardProperty_PHOTO = "PHOTO";

const char* cVCardProperty_BDAY = "BDAY";

const char* cVCardProperty_ADR = "ADR";

const char* cVCardProperty_LABEL = "LABEL";

const char* cVCardProperty_TEL = "TEL";

const char* cVCardProperty_EMAIL = "EMAIL";

const char* cVCardProperty_MAILER = "MAILER";

const char* cVCardProperty_TZ = "TZ";

const char* cVCardProperty_GEO = "GEO";

const char* cVCardProperty_TITLE = "TITLE";

const char* cVCardProperty_ROLE = "ROLE";

const char* cVCardProperty_LOGO = "LOGO";

const char* cVCardProperty_AGENT = "AGENT";

const char* cVCardProperty_CALENDAR_ADDRESS = "X-CALENDAR-ADDRESS";

const char* cVCardProperty_ORG = "ORG";

const char* cVCardProperty_CATEGORIES = "CATEGORIES";

const char* cVCardProperty_NOTE = "NOTE";

const char* cVCardProperty_PRODID = "PRODID";

const char* cVCardProperty_REV = "REV";

const char* cVCardProperty_SORT_STRING = "SORT-STRING";

const char* cVCardProperty_SOUND = "SOUND";

const char* cVCardProperty_UID = "UID";

const char* cVCardProperty_URL = "URL";

const char* cVCardProperty_VERSION = "VERSION";
	const char* cVCardValue_VERSION = "3.0";

const char* cVCardProperty_CLASS = "CLASS";
	const char* cVCardValue_CLASS_PUBLIC = "PUBLIC";
	const char* cVCardValue_CLASS_PRIVATE = "PRIVATE";
	const char* cVCardValue_CLASS_CONFIDENTIAL = "CONFIDENTIAL";

const char* cVCardProperty_KEY = "KEY";

// draft-jennings-impp-vcard-05
const char* cVCardProperty_IMPP = "IMPP";

// 2426 vCard Property Atrributes

// 2425 MIME-DIR Parameters
const char* cVCardAttribute_ENCODING = "ENCODING";
	const char* cVCardAttribute_ENCODING_B = "b";
const char* cVCardAttribute_VALUE = "VALUE";
const char* cVCardAttribute_LANGUAGE = "LANGUAGE";
const char* cVCardAttribute_CONTEXT = "CONTEXT";

// 2426 vCard Parameters
const char* cVCardAttribute_TYPE = "TYPE";

	const char* cVCardAttribute_TYPE_PREF = "PREF";

	const char* cVCardAttribute_TYPE_TEL_HOME = "HOME";
	const char* cVCardAttribute_TYPE_TEL_WORK = "WORK";
	const char* cVCardAttribute_TYPE_TEL_VOICE = "VOICE";
	const char* cVCardAttribute_TYPE_TEL_FAX = "FAX";
	const char* cVCardAttribute_TYPE_TEL_MSG = "MSG";
	const char* cVCardAttribute_TYPE_TEL_CELL = "CELL";
	const char* cVCardAttribute_TYPE_TEL_PAGER = "PAGER";
	const char* cVCardAttribute_TYPE_TEL_BBS = "BBS";
	const char* cVCardAttribute_TYPE_TEL_MODEM = "MODEM";
	const char* cVCardAttribute_TYPE_TEL_CAR = "CAR";
	const char* cVCardAttribute_TYPE_TEL_ISDN = "ISDN";
	const char* cVCardAttribute_TYPE_TEL_VIDEO = "VIDEO";
	const char* cVCardAttribute_TYPE_TEL_PCS = "PCS";

	const char* cVCardAttribute_TYPE_EMAIL_INTERNET = "INTERNET";
	const char* cVCardAttribute_TYPE_EMAIL_X400 = "X400";

	const char* cVCardAttribute_TYPE_KEY_X509 = "X509";
	const char* cVCardAttribute_TYPE_KEY_PGP = "PGP";

	const char* cVCardAttribute_TYPE_ADR_DOM = "dom";
	const char* cVCardAttribute_TYPE_ADR_INTL = "intl";
	const char* cVCardAttribute_TYPE_ADR_POSTAL = "postal";
	const char* cVCardAttribute_TYPE_ADR_PARCEL = "parcel";
	const char* cVCardAttribute_TYPE_ADR_HOME = "home";
	const char* cVCardAttribute_TYPE_ADR_WORK = "work";

// draft-jennings-impp-vcard-05 Parameters
	const char* cVCardAttribute_TYPE_IMPP_PERSONAL = "PERSONAL";
	const char* cVCardAttribute_TYPE_IMPP_BUSINESS = "BUSINESS";
	const char* cVCardAttribute_TYPE_IMPP_HOME = "HOME";
	const char* cVCardAttribute_TYPE_IMPP_WORK = "WORK";
	const char* cVCardAttribute_TYPE_IMPP_MOBILE = "MOBILE";

// Value types

// 2425 MIME-DIR
const char* cVCardValue_URI = "uri";
const char* cVCardValue_TEXT = "ptext";
const char* cVCardValue_DATE = "date";
const char* cVCardValue_TIME = "time";
const char* cVCardValue_DATE_TIME = "date-time";
const char* cVCardValue_INTEGER = "integer";
const char* cVCardValue_BOOLEAN = "boolean";
const char* cVCardValue_FLOAT = "float";

// 2426 vCard
const char* cVCardValue_BINARY = "binary";
const char* cVCardValue_UTC_OFFSET = "utc-offset";

// Parameter Values

// 2425 MIME-DIR

const char* cVCardValue_BOOLEAN_TRUE = "TRUE";
const char* cVCardValue_BOOLEAN_FALSE = "FALSE";

// Mulberry extensions
const char* cVCardProperty_X_PRIVATE_RURL = "X-MULBERRY-PRIVATE-RURL";
const char* cVCardProperty_X_PRIVATE_ETAG = "X-MULBERRY-PRIVATE-ETAG";


};

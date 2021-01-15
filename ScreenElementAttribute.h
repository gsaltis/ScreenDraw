/*****************************************************************************
 * FILE NAME    : ScreenElementAttribute.h
 * DATE         : January 14 2021
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2021 by Vertiv Company
 *****************************************************************************/
#ifndef _screenelementattribute_h_
#define _screenelementattribute_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "GeneralUtilities/StringUtils.h"
#include "GeneralUtilities/json.h"
#include "GeneralUtilities/JSONOut.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : ScreenElementAttribute
 *****************************************************************************/
struct _ScreenElementAttribute
{
  string                                name;
  string                                value;
  struct _ScreenElementAttribute*       next;
  struct _ScreenElementAttribute*       prev;
};
typedef struct _ScreenElementAttribute ScreenElementAttribute;

/*****************************************************************************!
 * Exported Type : ScreenElementAttributeList
 *****************************************************************************/
struct _ScreenElementAttributeList
{
  ScreenElementAttribute*               head;
  ScreenElementAttribute*               tail;
};
typedef struct _ScreenElementAttributeList ScreenElementAttributeList;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
JSONOut*
ScreenElementAttributeToJSON
(ScreenElementAttribute* InAttribute);

ScreenElementAttribute*
ScreenElementAttributeFromJSON
(json_value* InJSON);

ScreenElementAttribute*
ScreenElementAttributeCreate
();

void
ScreenElementAttributeListAppend
(ScreenElementAttributeList* InList,ScreenElementAttribute* InAttribute);

void
ScreenElementAttributeListDestroy
(ScreenElementAttributeList* InList);

ScreenElementAttributeList*
ScreenElementAttributeListCreate
();

JSONOut*
ScreenElementAttributeListToJSON
(ScreenElementAttributeList* InList);

ScreenElementAttributeList*
ScreenElementAttributeListFromJSON
(json_value* InJSON);

#endif /* _screenelementattribute_h_*/

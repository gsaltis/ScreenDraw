/*****************************************************************************
 * FILE NAME    : ScreenElement.h
 * DATE         : January 14 2021
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2021 by Vertiv Company
 *****************************************************************************/
#ifndef _screenelement_h_
#define _screenelement_h_

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
#include "ScreenElementAttribute.h"
#include "GeneralUtilities/json.h"
#include "GeneralUtilities/JSONOut.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : ScreenElement
 *****************************************************************************/
struct _ScreenElement
{
  string                                name;
  ScreenElementAttributeList*           attributes;
  struct _ScreenElement*                next;
  struct _ScreenElement*                prev;
};
typedef struct _ScreenElement ScreenElement;

/*****************************************************************************!
 * Exported Type : ScreenElementList
 *****************************************************************************/
struct _ScreenElementList
{
  ScreenElement*                        head;
  ScreenElement*                        tail;
};
typedef struct _ScreenElementList ScreenElementList;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
ScreenElement*
ScreenElementFromJSON
(json_value* InJSON);

ScreenElementList*
ScreenElementListCreate
();

void
ScreenElementAddAttribute
(ScreenElement* InElement,ScreenElementAttribute* InAttribute);

void
ScreenElementDestroy
(ScreenElement* InElement);

ScreenElement*
ScreenElementCreate
(string InName);

JSONOut*
ScreenElementListToJSON
(ScreenElementList* InList);

ScreenElementList*
ScreenElementListFromJSON
(json_value* InJSON);

void
ScreenElementListDestroy
(ScreenElementList* InList);

void
ScreenElementListAppend
(ScreenElementList* InList, ScreenElement* InElement);

#endif /* _screenelement_h_*/

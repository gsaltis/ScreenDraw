/*****************************************************************************
 * FILE NAME    : ScreenElement.c
 * DATE         : January 14 2021
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2021 by Vertiv Company
 *****************************************************************************/
/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ScreenElement.h"
#include "GeneralUtilities/MemoryManager.h"
#include "GeneralUtilities/JSONOut.h"
#include "GeneralUtilities/JSONIF.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Function : ScreenElementCreate
 * Purpose  : Create a new ScreenElement structure
 *****************************************************************************/
ScreenElement*
ScreenElementCreate
(string InName)
{
  int                                   n;
  ScreenElement*                        screenElement;
  if ( NULL == InName ) {
    return NULL;
  }
  n = sizeof(ScreenElement);
  screenElement = (ScreenElement*)GetMemory(n);
  memset(screenElement, 0x00, n);
  screenElement->name = StringCopy(InName);
  screenElement->attributes = ScreenElementAttributeListCreate();
  return screenElement;
}

/*****************************************************************************!
 * Function : ScreenElementDestroy
 * Purpose  : Destroy a ScreenElement structure
 *****************************************************************************/
void
ScreenElementDestroy
(ScreenElement* InElement)
{
  if ( NULL == InElement ) {
    return;
  }
  FreeMemory(InElement->name);
  ScreenElementAttributeListDestroy(InElement->attributes);
  FreeMemory(InElement);
}


/*****************************************************************************!
 * Function : ScreenElementAddAttribute
 * Purpose  : Add an attribute to a screen element
 *****************************************************************************/
void
ScreenElementAddAttribute
(ScreenElement* InElement, ScreenElementAttribute* InAttribute)
{
  if ( NULL == InElement || NULL == InAttribute ) {
    return;
  }
  ScreenElementAttributeListAppend(InElement->attributes, InAttribute);
}

/*****************************************************************************!
 * Function : ScreenElementListCreate
 * Purpose  : Create an empty ScreenElementList structure.
 *****************************************************************************/
ScreenElementList*
ScreenElementListCreate
()
{
  int                                   n;
  ScreenElementList*                    list;

  n = sizeof(ScreenElementList);
  list = (ScreenElementList*)GetMemory(n);
  memset(list, 0x00, n);

  return list;
}

/*****************************************************************************!
 * Function : ScreenElementListDestroy
 * Purpose  : Destroy of list of screen elements
 *****************************************************************************/
void
ScreenElementListDestroy
(ScreenElementList* InList)
{
  if ( NULL == InList ) {
    return;
  }
}

/*****************************************************************************!
 * Function : ScreenElementListFromJSON
 * Purpose  : Create a ScreenElementList from a JSON Object
 *****************************************************************************/
ScreenElementList*
ScreenElementListFromJSON
(json_value* InJSON)
{
  ScreenElement*                        screenElement;
  json_value*                           elementObject;
  int                                   i;
  ScreenElementList*                    list;
  if ( NULL == InJSON ) {
    return NULL;
  }

  list = ScreenElementListCreate();

  for ( i = 0 ; i < InJSON->u.array.length ; i++ ) {
    elementObject = InJSON->u.array.values[i];
    screenElement = ScreenElementFromJSON(elementObject);
    if ( screenElement ) {
      ScreenElementListAppend(list, screenElement);
    }
  }
  return list;
}

/*****************************************************************************!
 * Function : ScreenElementListToJSON
 * Purpose  : Create a JSON Object from a ScreenElementList
 *****************************************************************************/
JSONOut*
ScreenElementListToJSON
(ScreenElementList* InList)
{
  if ( NULL == InList ) {
    return NULL;
  }
  return NULL;
}

/*****************************************************************************!
 * Function : ScreenElementFromJSON
 * Purpose  : Create a ScreenElement struct from a JSON object
 *****************************************************************************/
ScreenElement*
ScreenElementFromJSON
(json_value* InJSON)
{
  json_value*                           attributes;
  ScreenElement*                        element;
  string                                name;
  if ( NULL == InJSON ) {
    return NULL;
  }

  name = JSONIFGetString(InJSON, "name");
  if ( NULL == name ) {
    return NULL;
  }
  
  element = ScreenElementCreate(name);
  FreeMemory(name);

  attributes = JSONIFGetArray(InJSON, "attributes");
  
  // The 'create' function creates an empty list but we don't want it
  ScreenElementAttributeListDestroy(element->attributes);

  // Since we are reading it from the JSON file
  element->attributes = ScreenElementAttributeListFromJSON(attributes);
  return element;
}

/*****************************************************************************!
 * Function : ScreenElementListAppend
 * Purpose  : Add an element to a ScreenElementList
 *****************************************************************************/
void
ScreenElementListAppend
(ScreenElementList* InList, ScreenElement* InElement)
{
  if ( NULL == InList || NULL == InElement ) {
    return;
  }

  InElement->prev = InList->tail;
  if ( InList->tail ) {
    InList->tail->next = InElement;
  } else {
    InList->head = InElement;
  }
  InList->tail = InElement;
}

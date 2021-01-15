/*****************************************************************************
 * FILE NAME    : ScreenElementAttribute.c
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
#include "ScreenElementAttribute.h"
#include "GeneralUtilities/MemoryManager.h"
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
 * Function : ScreenElementAttributeListDestroy
 * Purpose  : Destroy a list of screen/element attributes
 *****************************************************************************/
void
ScreenElementAttributeListDestroy
(ScreenElementAttributeList* InList)
{
  if ( NULL == InList ) {
    return;
  }
}

/*****************************************************************************!
 * Function : ScreenElementAttributeListFromJSON
 * Purpose  : Create a ScreenElementAttributeList from a JSON Object
 *****************************************************************************/
ScreenElementAttributeList*
ScreenElementAttributeListFromJSON
(json_value* InJSON)
{
  ScreenElementAttribute*               attribute;
  json_value*                           attributeObject;
  int                                   i;
  ScreenElementAttributeList*           list;
  if ( NULL == InJSON ) {
    return NULL;
  }

  list = ScreenElementAttributeListCreate();

  for ( i = 0 ; i < InJSON->u.array.length ; i++ ) {
    attributeObject = InJSON->u.array.values[i];
    attribute = ScreenElementAttributeFromJSON(attributeObject);
    if ( attribute ) {
      ScreenElementAttributeListAppend(list, attribute);
    }
  }
  return list;
}

/*****************************************************************************!
 * Function : ScreenElementAttributeListToJSON
 * Purpose  : Create a JSON Object from a ScreenElementAttributeList
 *****************************************************************************/
JSONOut*
ScreenElementAttributeListToJSON
(ScreenElementAttributeList* InList)
{
  JSONOut*                              object;
  ScreenElementAttribute*               attribute;
  JSONOut*                              array;
  if ( NULL == InList ) {
    return NULL;
  }

  array = JSONOutCreateArray("attributes");

  for ( attribute = InList->head; attribute ; attribute = attribute->next ) {
    object = ScreenElementAttributeToJSON(attribute);
    JSONOutArrayAddObject(array, object);
  }
  return array;
}

/*****************************************************************************!
 * Function : ScreenElementAttributeListCreate
 * Purpose  : Create an empty ScreenElementAttributeList structure
 *****************************************************************************/
ScreenElementAttributeList*
ScreenElementAttributeListCreate
()
{
  int                                   n;
  ScreenElementAttributeList*           list;

  n = sizeof(ScreenElementAttributeList);
  list = (ScreenElementAttributeList*)GetMemory(n);
  memset(list, 0x00, n);
  return list;
}

/*****************************************************************************!
 * Function : ScreenElementAttributeListAppend
 * Purpose  : Add an item to a ScreenElementAttributeList
 *****************************************************************************/
void
ScreenElementAttributeListAppend
(ScreenElementAttributeList* InList, ScreenElementAttribute* InAttribute)
{
  if ( NULL == InList || NULL == InAttribute ) {
    return;
  }

  InAttribute->prev = InList->tail;
  if ( InList->tail ) {
    InList->tail->next = InAttribute;
  } else {
    InList->head = InAttribute;
  }
  InList->tail = InAttribute;
}


/*****************************************************************************!
 * Function : ScreenElementAttributeCreate
 * Purpose  : Create an empty ScreenElementAttribute structure
 *****************************************************************************/
ScreenElementAttribute*
ScreenElementAttributeCreate
()
{
  int                                   n;
  ScreenElementAttribute*               attribute;

  n = sizeof(ScreenElementAttribute);
  attribute = (ScreenElementAttribute*)GetMemory(n);
  memset(attribute, 0x00, n);
  return attribute;
}


/*****************************************************************************!
 * Function : ScreenElementAttributeFromJSON
 * Purpose  : Create a ScreenElementAttribute struct from a JSON Object
 *****************************************************************************/
ScreenElementAttribute*
ScreenElementAttributeFromJSON
(json_value* InJSON)
{
  string                                value;
  string                                name;
  ScreenElementAttribute*               attribute;
  if ( NULL == InJSON ) {
    return NULL;
  }

  name = JSONIFGetString(InJSON, "name");
  if ( NULL == name ) {
    return NULL;
  }

  value = JSONIFGetString(InJSON, "value");
  if ( NULL == value ) {
    FreeMemory(name);
    return NULL;
  }

  attribute = ScreenElementAttributeCreate();
  attribute->name = name;
  attribute->value = value;
  return attribute;
}


/*****************************************************************************!
 * Function : ScreenElementAttributeToJSON
 * Purpose  : Create a JSON object from a ScreenElementAttribute structure
 *****************************************************************************/
JSONOut*
ScreenElementAttributeToJSON
(ScreenElementAttribute* InAttribute)
{
  JSONOut*                              object;
  if ( NULL == InAttribute ) {
    return NULL;
  }

  object = JSONOutCreateObject(NULL);
  JSONOutObjectAddObjects(object,
                          JSONOutCreateString("name", InAttribute->name),
                          JSONOutCreateString("value", InAttribute->value),
                          NULL);
  return object;
}

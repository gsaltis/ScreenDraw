
/*****************************************************************************
 * FILE NAME    : Screen.c
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
#include "Screen.h"
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
 * Function : ScreenCreate
 * Purpose  : Create an empty screen
 *****************************************************************************/
Screen*
ScreenCreate
()
{
  int                                   n;
  Screen*                               screen;

  n = sizeof(Screen);
  screen = (Screen*)GetMemory(n);
  memset(screen, 0x00, n);
  return screen;
}

/*****************************************************************************!
 * Function : ScreenDestroy
 * Purpose  : Destroy a screen
 *****************************************************************************/
void
ScreenDestroy
(Screen* InScreen)
{
  if ( NULL == InScreen ) {
    return;
  }
  if ( InScreen->name ) {
    FreeMemory(InScreen->name);
  }
  if ( InScreen->elements ) {
    ScreenElementListDestroy(InScreen->elements);
  }

  if ( InScreen->attributes ) {
    ScreenElementAttributeListDestroy(InScreen->attributes);
  }

  FreeMemory(InScreen);
}

/*****************************************************************************!
 * Function : ScreenFromJSON
 * Purpose  : Create a screen from a JSON Object
 *****************************************************************************/
Screen*
ScreenFromJSON
(json_value* InJSON)
{
  json_value*                           attributes;
  json_value*                           elements;
  Screen*                               screen;
  string                                name;
  
  if ( NULL == InJSON ) {
    return NULL;
  }

  name = JSONIFGetString(InJSON, "name");
  if ( NULL == name ) {
    return NULL;
  }
  screen = ScreenCreate();
  ScreenSetName(screen, name);
  FreeMemory(name);

  elements = JSONIFGetArray(InJSON, "elements");
  if ( elements ) {
    screen->elements = ScreenElementListFromJSON(elements);
  }

  attributes = JSONIFGetArray(InJSON, "attributes");
  if ( attributes ) {
    screen->attributes = ScreenElementAttributeListFromJSON(attributes);
  }
  return screen;
}

/*****************************************************************************!
 * Function : ScreenToJSON
 * Purpose  : Create a JSONOut Object from a Screen
 *****************************************************************************/
JSONOut*
ScreenToJSON
(Screen* InScreen)
{
  JSONOut*                              attributes;
  JSONOut*                              elements;
  JSONOut*                              jsonOut;
  if ( NULL == InScreen ) {
    return NULL;
  }

  jsonOut = JSONOutCreateObject(NULL);
  if ( InScreen->name ) {
    JSONOutObjectAddObject(jsonOut, JSONOutCreateString("name", InScreen->name));
  }

  if ( InScreen->elements ) {
    elements = ScreenElementListToJSON(InScreen->elements);
    JSONOutObjectAddObject(jsonOut, elements);
  }

  if ( InScreen->attributes ) {
    attributes = ScreenElementAttributeListToJSON(InScreen->attributes);
    JSONOutObjectAddObject(jsonOut, attributes);
  }
  return jsonOut;
}

/*****************************************************************************!
 * Function : ScreenSetName
 * Purpose  : Set the name associated with a screen
 *****************************************************************************/
void
ScreenSetName
(Screen* InScreen, string InName)
{
  if ( NULL == InScreen || NULL == InName ) {
    return;
  }
  if ( InScreen->name ) {
    FreeMemory(InScreen->name);
  }
  InScreen->name = StringCopy(InName);
}


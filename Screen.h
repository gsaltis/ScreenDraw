/*****************************************************************************
 * FILE NAME    : Screen.h
 * DATE         : January 14 2021
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2021 by Vertiv Company
 *****************************************************************************/
#ifndef _screen_h_
#define _screen_h_

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
#include "ScreenElement.h"
#include "ScreenElementAttribute.h"
#include "GeneralUtilities/json.h"
#include "GeneralUtilities/JSONOut.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : Screen
 *****************************************************************************/
struct _Screen
{
  string                                name;
  ScreenElementList*                    elements;
  ScreenElementAttributeList*           attributes;
};
typedef struct _Screen Screen;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
ScreenSetName
(Screen* InScreen,string InName);

JSONOut*
ScreenToJSON
(Screen* InScreen);

Screen*
ScreenFromJSON
(json_value* InJSON);

void
ScreenDestroy
(Screen* InScreen);

Screen*
ScreenCreate
();

#endif /* _screen_h_*/

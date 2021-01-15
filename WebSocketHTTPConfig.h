/*****************************************************************************
 * FILE NAME    : WebSocketHTTPConfig.h
 * DATE         : January 11 2021
 * COPYRIGHT    : Copyright (C) 2021 by Gregory R Saltis
 *****************************************************************************/
#ifndef _websockethttpconfig_h_
#define _websockethttpconfig_h_

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

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
WebSocketHTTPInitialize
();

void
WebSocketHTTPSetHTTPPortAddress
(string InHTTPPortAddress);

void
WebSocketHTTPSetWebSocketPortAddress
(string InWebSocketPortAddress);

void
WebSocketHTTPSetWWWDirectory
(string InWWWDirectory);

string
WebSocketHTTPGetWebSocketHTTPPortAddress
();

string
WebSocketHTTPGetHTTPPortAddress
();

string
WebSocketHTTPGetWWWDirectory
();

#endif /* _websockethttpconfig_h_*/

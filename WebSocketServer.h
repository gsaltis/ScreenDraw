/*****************************************************************************
 * FILE NAME    : WebSocketServer.h
 * DATE         : January 7 2021
 * PROJECT      : NONE
 * COPYRIGHT    : Copyright (C) 2021 by Gregory R Saltis
 *****************************************************************************/
#ifndef _websocketserver_h_
#define _websocketserver_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/

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
WebSocketServerInitialize
();

pthread_t
WebSocketServerGetThreadID
();

void
WebSocketServerStart
();

void
WebSocketServerCreateInfoScript
();

void
WebSocketServerSetDirectory
(string InWWWDirectory);

#endif // _websocketserver_h_

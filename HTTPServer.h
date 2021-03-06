/*****************************************************************************
 * FILE NAME    : HTTPServer.h
 * DATE         : January 7 2021
 * PROJECT      : NONE
 * COPYRIGHT    : Copyright (C) 2021 by Gregory R Saltis
 *****************************************************************************/
#ifndef _httpserver_h_
#define _httpserver_h_

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
pthread_t
HTTPServerGetThreadID
();

void
HTTPServerStart
();

void
HTTPServerInitialize
();

#endif // _httpserver_h_

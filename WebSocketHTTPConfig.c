/*****************************************************************************
 * FILE NAME    : WebSocketHTTPConfig.c
 * DATE         : January 11 2021
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2021 by Gregory R Saltis
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
#include "WebSocketHTTPConfig.h"
#include "GeneralUtilities/StringUtils.h"
#include "GeneralUtilities/MemoryManager.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
static string
WWWDirectoryDefault = "www";

static string
WWWDirectory = NULL;

static string
WebSocketPortAddressDefault = "8022";

static string
WebSocketPortAddress = NULL;

static string
HTTPPortAddress = NULL;

static string
HTTPPortAddressDefault = "8021";


/*****************************************************************************!
 * Function : WebSocketHTTPInitialize
 *****************************************************************************/
void
WebSocketHTTPInitialize
()
{
  WWWDirectory = StringCopy(WWWDirectoryDefault);
  WebSocketPortAddress = StringCopy(WebSocketPortAddressDefault);
  HTTPPortAddress = StringCopy(HTTPPortAddressDefault);
}

/*****************************************************************************!
 * Function : WebSocketHTTPGetWWWDirectory
 *****************************************************************************/
string
WebSocketHTTPGetWWWDirectory
()
{
  return WWWDirectory;
}

/*****************************************************************************!
 * Function : WebSocketHTTPGetHTTPPortAddress
 *****************************************************************************/
string
WebSocketHTTPGetHTTPPortAddress
()
{
  return HTTPPortAddress;
}

/*****************************************************************************!
 * Function : WebSocketHTTPGetWebSocketHTTPPortAddress
 *****************************************************************************/
string
WebSocketHTTPGetWebSocketHTTPPortAddress
()
{
  return WebSocketPortAddress;
}

/*****************************************************************************!
 * Function : WebSocketHTTPSetWWWDirectory
 *****************************************************************************/
void
WebSocketHTTPSetWWWDirectory
(string InWWWDirectory)
{
  if ( NULL == InWWWDirectory ) {
    return;
  }
  if ( WWWDirectory ) {
    FreeMemory(WWWDirectory);
  }
  WWWDirectory = StringCopy(WWWDirectoryDefault);
}

/*****************************************************************************!
 * Function : WebSocketHTTPSetHTTPPortAddress
 *****************************************************************************/
void
WebSocketHTTPSetHTTPPortAddress
(string InHTTPPortAddress)
{
  if ( NULL == InHTTPPortAddress ) {
    return;
  }
  if ( HTTPPortAddress ) {
    FreeMemory(HTTPPortAddress);
  }
  HTTPPortAddress = StringCopy(HTTPPortAddressDefault);
}

/*****************************************************************************!
 * Function : WebSocketHTTPSetWebSocketPortAddress
 *****************************************************************************/
void
WebSocketHTTPSetWebSocketPortAddress
(string InWebSocketPortAddress)
{
  if ( NULL == InWebSocketPortAddress ) {
    return;
  }
  if ( WebSocketPortAddress ) {
    FreeMemory(WebSocketPortAddress);
  }
  WebSocketPortAddress = StringCopy(WebSocketPortAddressDefault);
  
}

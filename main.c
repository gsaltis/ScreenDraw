/*****************************************************************************
 * FILE NAME    : main.c
 * DATE         : January 7 2021
 * PROJECT      : NONE
 * COPYRIGHT    : Copyright (C) 2021 by Gregory R Saltis
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "main.h"
#include "mainconfig.h"
#include "HTTPServer.h"
#include "UserInterfaceServer.h"
#include "WebSocketServer.h"
#include "WebSocketHTTPConfig.h"
#include "GeneralUtilities/FileUtils.h"
#include "GeneralUtilities/json.h"
#include "GeneralUtilities/JSONOut.h"
#include "Screen.h"
#include "GeneralUtilities/MemoryManager.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void
MainInitialize
();

/*****************************************************************************!
 * Function : main
 *****************************************************************************/
int
main
(int argc, char** argv)
{
  MainInitialize();
  HTTPServerStart();
  pthread_join(HTTPServerGetThreadID(), NULL);
  pthread_join(UserInterfaceServerGetThreadID(), NULL);
  return EXIT_SUCCESS;
}


/*****************************************************************************!
 * Function : MainInitialize
 *****************************************************************************/
void
MainInitialize
()
{
  MainReadScreen();
  WebSocketHTTPInitialize();
  WebSocketServerInitialize();
  HTTPServerInitialize();
  UserInterfaceServerInitialize();
}


/*****************************************************************************!
 * Function : MainReadScreen
 * Purpose  : Read a Screen JSON File
 *****************************************************************************/
void
MainReadScreen
()
{
  string                                s;
  JSONOut*                              jsonOut;
  char                                  jsonError[json_error_max+1];
  Screen*                               screen;
  json_value*                           json;
  int                                   bufferSize;
  char*                                 buffer;
  json_settings                         settings =  { } ;
  
  if ( ! GetFileBuffer("screen.json", &buffer, &bufferSize) ) {
    return;
  }

  json = json_parse_ex(&settings, buffer, bufferSize, jsonError);
  if ( NULL == json ) {
    FreeMemory(buffer);
    fprintf(stderr, "Error parsing \"screen.json\" : %s\n", jsonError);
    return;
  }
  screen = ScreenFromJSON(json);
  json_value_free(json);

  jsonOut = ScreenToJSON(screen);
  s = JSONOutToString(jsonOut, 2);
  printf("%s\n", s);
  FreeMemory(s);
  FreeMemory(buffer);
}

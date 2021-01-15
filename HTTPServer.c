/*****************************************************************************
 * FILE NAME    : HTTPServer.c
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

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "GeneralUtilities/ANSIColors.h"
#include "RPiBaseModules/mongoose.h"
#include "GeneralUtilities/StringUtils.h"
#include "GeneralUtilities/MemoryManager.h"
#include "HTTPServer.h"
#include "WebSocketServer.h"
#include "WebSocketHTTPConfig.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
static pthread_t
HTTPServerThreadID;

static int
HTTPServerPollPeriod = 20;

static struct mg_serve_http_opts
HTTPServerOptions;

static struct mg_connection*
HTTPConnection;

static struct mg_mgr
HTTPManager;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void*
HTTPServerThread
(void* InParameters);

void
HTTPServerEventHandler
(struct mg_connection* InConnection, int InEvent, void* InParameter);

/*****************************************************************************!
 * Function : HTTPServerInit
 *****************************************************************************/
void
HTTPServerInitialize
()
{
}

/*****************************************************************************!
 * Function : HTTPServerStart
 *****************************************************************************/
void
HTTPServerStart
()
{
  if ( pthread_create(&HTTPServerThreadID, NULL, HTTPServerThread, NULL) ) {
    fprintf(stderr, "%s\"Could not start \"HTTP Server Thread\"%s\n", ColorRed, ColorReset);
    exit(EXIT_FAILURE);
  }
}

/*****************************************************************************!
 * Function : HTTPServerThread
 *****************************************************************************/
void*
HTTPServerThread
(void* InParameters)
{
  mg_mgr_init(&HTTPManager, NULL);
  HTTPConnection = mg_bind(&HTTPManager, WebSocketHTTPGetHTTPPortAddress(), HTTPServerEventHandler);

  if ( NULL == HTTPConnection ) {
    fprintf(stderr, "%sFailed to create HTTP server%s\n", ColorBrightRed, ColorReset);
    exit(EXIT_FAILURE);
  }

  mg_set_protocol_http_websocket(HTTPConnection);
  HTTPServerOptions.document_root = WebSocketHTTPGetWWWDirectory();
  HTTPServerOptions.enable_directory_listing = "yes";
  WebSocketServerStart();
  while ( true ) {
    mg_mgr_poll(&HTTPManager, HTTPServerPollPeriod);
  }
}

/*****************************************************************************!
 * Function : HTTPServerEventHandler
 *****************************************************************************/
void
HTTPServerEventHandler
(struct mg_connection* InConnection, int InEvent, void* InParameter)
{
  if ( InEvent == MG_EV_HTTP_REQUEST ) {
    mg_serve_http(InConnection, (struct http_message*)InParameter, HTTPServerOptions);
  }
}

/*****************************************************************************!
 * Function : HTTPServerGetThreadID
 *****************************************************************************/
pthread_t
HTTPServerGetThreadID
()
{
  return HTTPServerThreadID;
}

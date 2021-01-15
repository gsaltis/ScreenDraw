/*****************************************************************************
 * FILE NAME    : UserInterfaceServer.c
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
#include <errno.h>
#include <ctype.h>
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "UserInterfaceServer.h"
#include "GeneralUtilities/StringUtils.h"
#include "RPiBaseModules/linenoise.h"
#include "GeneralUtilities/ANSIColors.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
static pthread_t
UserInterfaceServerID;

static string
UserInterfaceCommandPrompt = "Enter Command : ";

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void*
UserInterfaceServerThread
(void* InParameter);

static StringList*
UserInterfaceParseCommandLine
(string InCommandLine);

static void
UserInterfaceProcessCommand
(StringList* InCommand);

static void
UserInterfaceProcessCommandQuit
(StringList* InCommand);

static void
UserInterfaceProcessCommandHelp
(StringList* InCommand);

/*****************************************************************************!
 * Function : UserInterfaceServerInitialize
 *****************************************************************************/
void
UserInterfaceServerInitialize
()
{
  UserInterfaceServerID = 0;
}

/*****************************************************************************!
 * Function : UserInterfaceServerStart
 *****************************************************************************/
void
UserInterfaceServerStart
()
{
  if ( pthread_create(&UserInterfaceServerID, NULL, UserInterfaceServerThread, NULL) ) {
    fprintf(stderr, "Could not start User Input Server : %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

/*****************************************************************************!
 * Function : UserInterfaceServerThread
 *****************************************************************************/
void*
UserInterfaceServerThread
(void* InParameter)
{
  StringList*                           command;
  string                                userInterfaceString;
  while (true) {
    userInterfaceString = linenoise(UserInterfaceCommandPrompt);
    command = UserInterfaceParseCommandLine(userInterfaceString);
    if ( NULL == command ) {
      continue;
    }
    if ( command->stringCount == 0 ) {
      free(userInterfaceString);
      StringListDestroy(command);
      continue;
    }
    UserInterfaceProcessCommand(command);
    StringListDestroy(command);
    linenoiseHistoryAdd(userInterfaceString);
    free(userInterfaceString);
  }
  return NULL;
}

/*****************************************************************************!
 * Function : UserInterfaceParseCommandLine
 *****************************************************************************/
StringList*
UserInterfaceParseCommandLine
(string InCommandLine)
{
  string                                s;
  int                                   n;
  string                                start;
  string                                end;
  StringList*                           commands;
  enum
  {
   InStart = 1,
   InSpaces,
   InToken,
   InSingleQuotedString,
   InDoubleQuotedString,
   InDone
  } state = InStart;
  
  if ( NULL == InCommandLine ) {
    return NULL;
  }
  if ( StringContainsChar(InCommandLine, '"') ) {
    commands = StringListCreate();
    start = InCommandLine;
    end = start;
    while ( state != InDone ) {
      if ( *end == 0x00 ) {
        switch (state) {
          case InStart : {
            break;
          }
          case InDone : {
            break;
          }
          case InSpaces : {
            break;
          }
          case InToken : {
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            break;
          }
          case InDoubleQuotedString : {
            start++;
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            break;
          }
          case InSingleQuotedString : {
            start++;
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            break;
          }
        }
        state = InDone;
        continue;
      }
      switch (state) {
        case InDone : {
          break;
        }
          
          //!
        case InStart : {
          if ( *start == 0x00 ) {
            state = InDone;
            break;
          }
          if (isspace(*start) ) {
            start++;
            state = InSpaces;
            break;
          }
          if ( *start == '"' ) {
            end = start;
            end++;
            state = InDoubleQuotedString;
            break;
          }
          if  ( *start == '\'' ) {
            end = start;
            end++;
            state = InSingleQuotedString;
            break;
          }
          end = start;
          end++;
          state = InToken;
          break;
        }
          //!
        case InSpaces : {
          if ( *start == 0x00 ) {
            state = InDone;
            break;
          }
          
          if ( isspace(*start) ) {
            start++;
            break;
          }
          if ( *start == '"' ) {
            end = start;
            end++;
            state = InDoubleQuotedString;
            break;
          }
          if  ( *start == '\'' ) {
            end = start;
            end++;
            state = InSingleQuotedString;
            break;
          }
          end = start;
          end++;
          state = InToken;
          break;
        }

          //!
        case InToken : {
          if ( *end == 0x00 ) {
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            state = InDone;
            break;
          }
          
          if ( isspace(*end) ) {
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            start = end;
            state = InSpaces;
            break;
          }
          end++;
          break;
        }

          //!
        case InSingleQuotedString : {
          if (*end == '\\' ) {
            end++;
            if ( *end ) {
              end++;
              break;
            }
            state = InDone;
            break;
          }
          if ( *end == 0x00 ) {
            start++;
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            state = InDone;
            break;
          }
          if ( *end == '\'' ) {
            start++;
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            end++;
            start = end;
            if ( *end == 0x00 ) {
              state = InDone;
              break;
            }
            if ( isspace(*start) ) {
              state = InSpaces;
              end++;
              break;
            }
            if ( *start == '"' ) {
              state = InDoubleQuotedString;
              end++;
              break;
            }
            if ( *start == '\'' ) {
              state = InSingleQuotedString;
              end++;
              break;
            }
            end++;
            state = InToken;
            break;
          }
          end++;
          break;
        }
          
          //!
        case InDoubleQuotedString : {
          if (*end == '\\' ) {
            end++;
            if ( *end ) {
              end++;
              break;
            }
            state = InDone;
            break;
          }
          if ( *end == 0x00 ) {
            start++;
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            state = InDone;
            break;
          }            
          if ( *end == '"' ) {
            start++;
            n = end - start;
            s = StringNCopy(start, n);
            StringListAppend(commands, s);
            end++;
            start = end;
            if ( *end == 0x00 ) {
              state = InDone;
              break;
            }
            if ( isspace(*start) ) {
              state = InSpaces;
              end++;
              break;
            }
            if ( *start == '"' ) {
              state = InDoubleQuotedString;
              end++;
              break;
            }
            if ( *start == '\'' ) {
              state = InSingleQuotedString;
              end++;
              break;
            }
            end++;
            state = InToken;
            break;
          }
          end++;
          break;
        }
      }
    }
  } else {
    commands = StringSplit(InCommandLine, " \t", true);
  }
  return commands;
}

/*****************************************************************************!
 * Function : UserInterfaceProcessCommand
 *****************************************************************************/
void
UserInterfaceProcessCommand
(StringList* InCommand)
{
  string                                command;
  if ( NULL == InCommand ) {
    return;
  }
  if ( InCommand->stringCount == 0 ) {
    return;
  }

  command = InCommand->strings[0];
  if ( StringEqualNoCase(command, "quit") ) {
    UserInterfaceProcessCommandQuit(InCommand);
    return;
  }

  if ( StringEqualNoCase(command, "help") ) {
    UserInterfaceProcessCommandHelp(InCommand);
    return;
  }
}

/*****************************************************************************!
 * Function : UserInterfaceProcessCommandQuit
 *****************************************************************************/
void
UserInterfaceProcessCommandQuit
(StringList* InCommand)
{
  exit(EXIT_SUCCESS);
}

/*****************************************************************************!
 * Function : UserInterfaceProcessCommandHelp
 *****************************************************************************/
void
UserInterfaceProcessCommandHelp
(StringList* InCommand)
{
  exit(EXIT_SUCCESS);
}


/*****************************************************************************!
 * Function : UserInterfaceServerGetThreadID
 *****************************************************************************/
pthread_t
UserInterfaceServerGetThreadID
()
{
  return UserInterfaceServerID;
}

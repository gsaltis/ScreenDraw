/*****************************************************************************
 * FILE NAME    : scripts.js
 * DATE         : January 07 2021
 * COPYRIGHT    : Copyright (C) 2021 by Gregory R Saltis
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var
WebSocketIFID = 0;

var
MessageColorError = "#C00";

var
MessageColorNormal = "#222";

var
SetMessageTimeoutID = 0;

var
Screens = [];

var
Types = [];

var
CurrentScreen = null;

var
ObjectIndex = 0;

var
CommandLinesCache = [];

var
CommandLinesCacheIndex = -1;

/*****************************************************************************!
 * Function : CBSystemInitialize
 *****************************************************************************/
function
CBSystemInitialize
()
{
  WebSocketIFInitialize();
  ResizeLists();
  document.getElementById("InputArea").focus();
  document.getElementById("MainListElementArea").style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : WebSocketIFInitialize
 *****************************************************************************/
function
WebSocketIFInitialize
()
{
  var                                   hostaddress;

  hostaddress = "ws://" + WebSocketIFAddress + ":" + WebSocketIFPort;

  WebSocketIFConnection = new WebSocket(hostaddress);
  WebSocketIFConnection.onopen = function() {
    SetMessage("Connected to " + WebSocketIFAddress + ":" + WebSocketIFPort);
    WebSocketIFSendSimpleRequest("init");
  }

  WebSocketIFConnection.onerror = function() {

  }

  WebSocketIFConnection.onclose = function() {
    SetMessage("Disconnected from " + WebSocketIFAddress + ":" + WebSocketIFPort);
    ShowBlocker();
  }

  WebSocketIFConnection.onmessage = function(InEvent) {
    WebSocketIFHandlePacket(InEvent.data);
  }
}

/*****************************************************************************!
 * Function : WebSocketIFSendSimpleRequest
 *****************************************************************************/
function
WebSocketIFSendSimpleRequest
(InRequest)
{
  var                                   d;
  var                                   request;

  request = {};

  d = new Date();
  
  request.packettype = "request";
  request.packetid = WebSocketIFGetNextID();
  request.time = d.getTime();
  request.type = InRequest;
  request.body = "";

  WebSocketIFSendGeneralRequest(request);
}

/*****************************************************************************!
 * Function : WebSocketIFSendGeneralRequest
 *****************************************************************************/
function
WebSocketIFSendGeneralRequest
(InRequest)
{
  if ( WebSocketIFConnection ) {
    WebSocketIFConnection.send(JSON.stringify(InRequest));
  }
}

/*****************************************************************************!
 * Function : WebSocketIFHandlePacket
 *****************************************************************************/
function
WebSocketIFHandlePacket
(InData)
{
  var                                   packet;
  var                                   packettype;
  
  packet = JSON.parse(InData);
  n = packet.packetid;
  if ( n > 0 ) {
    WebSocketIFID = n;
  }

  packettype = packet.packettype;
  if ( packettype == "request" ) {
    WebSocketIFHandleRequest(packet);
  } else if ( packettype == "response" ) {
    WebSocketIFHandleResponse(packet);
  }
}

/*****************************************************************************!
 * Function : WebSocketIFHandleResponse
 *****************************************************************************/
function
WebSocketIFHandleResponse
(InPacket)
{
  if ( InPacket.status == "Error" ) {
    SetMessageError(InPacket.message);
    return;
  }

  console.log(InPacket);
  if ( InPacket.status == "OK" ) {
    WebSocketIFID = InPacket.packetid + 1;
    if ( InPacket.type == "init" ) {
      WebSocketIFHandleResponseInit(InPacket.body);
      return;
    }
  }
}

/*****************************************************************************!
 * Function : WebSocketIFHandleRequest
 *****************************************************************************/
function
WebSocketIFHandleRequest
(InPacket)
{

}

/*****************************************************************************!
 * Function : SetMessage
 *****************************************************************************/
function
SetMessage
(InMessage, InColor)
{
  var                                   inputArea;

  inputArea = document.getElementById("MainMessageArea");
  inputArea.innerHTML = InMessage;
  inputArea.style.color = InColor;
  SetMessageTimeoutID = setTimeout(SetMessageClear, 10000);
}

/*****************************************************************************!
 * Function : SetMessageClear
 *****************************************************************************/
function
SetMessageClear
()
{
  var                                   inputArea;

  inputArea = document.getElementById("MainMessageArea");
  inputArea.innerHTML = "";
  SetMessageTimeoutID = 0;
}

/*****************************************************************************!
 * Function : SetMessageError
 *****************************************************************************/
function
SetMessageError
(InMessage)
{
  SetMessage(InMessage, MessageColorError);
}

/*****************************************************************************!
 * Function : SetMessageNormal
 *****************************************************************************/
function
SetMessageNormal
(InMessage)
{
  SetMessage(InMessage, MessageColorNormal);
}

/*****************************************************************************!
 * Function : WebSocketIFGetNextID
 *****************************************************************************/
function
WebSocketIFGetNextID
()
{
  return ++WebSocketIFID;
}

/*****************************************************************************!
 * Function : WebSocketIFHandleResponseInit
 *****************************************************************************/
function
WebSocketIFHandleResponseInit
(InPacket)
{
  HideBlocker();
}

/*****************************************************************************!
 * Function : HideBlocker
 *****************************************************************************/
function
HideBlocker
()
{
  var                                   blocker;

  blocker = document.getElementById("MainBlocker");
  blocker.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : ShowBlocker
 *****************************************************************************/
function
ShowBlocker
()
{
  var                                   blocker;

  blocker = document.getElementById("MainBlocker");
  blocker.style.visibility = "visible";
}

/*****************************************************************************!
 * Function : CBMainInputKeyDown
 *****************************************************************************/
function
CBMainInputKeyDown
(InEvent)
{
  if ( InEvent.key == "ArrowUp" ) {
    DisplayPreviousCommandLine();
    return;
  }
  if ( InEvent.key == "End" ) {
    console.log(InEvent.srcElement);
    return;
  }
}

/*****************************************************************************!
 * Function : CBMainInputChange
 *****************************************************************************/
function
CBMainInputChange
()
{
  var                                   input;
  var                                   elements;
  
  input = document.getElementById("InputArea");
  CommandLinesCache.push(input.value);
  if ( CommandLinesCache.length > 100 ) {
    CommandLinesCache.shift();
  }
  elements = GetInputElements(input.value);
  input.value = "";
  ProcessCommandElements(elements);
}

/*****************************************************************************!
 * Function : GetInputElements
 *****************************************************************************/
function
GetInputElements
(InString)
{
  var                                   chars;
  var                                   s;
  var                                   n;
  var                                   i;
  var                                   elements;

  n = InString.length;
  elements = [];
  chars = InString.split("");
  s = "";
  for ( i = 0 ; i < n ; i++ ) {
    if ( chars[i] == "\\" ) {
      i++;
      if ( i < n ) {
        s = s + chars[i];
      }
      continue;
    }
    if ( chars[i] == " " ) {
      if ( s == "" ) {
        continue;
      }
      elements.push(s);
      s = "";
      continue;
    }
    s = s + chars[i];
  }
  if ( s != "" ) {
    elements.push(s);
  }
  console.log(elements);
  return elements;
}

/*****************************************************************************!
 * Function : ProcessCommandElements
 *****************************************************************************/
function
ProcessCommandElements
(InElements)
{
  if ( InElements == undefined || InElements.length == 0 ) {
    return;
  }

  command = InElements[0];

  if ( command == "create" ) {
    ProcessCommandCreate(InElements);
  } else if ( command == "set" ) {
    ProcessCommandSet(InElements);
  } else if ( command == "remove" ) {
    ProcessCommandRemove(InElements);
  } else if ( command == "show" ) {
    ProcessCommandShow(InElements);
  } else if ( command == "help" ) {
    ProcessCommandHelp(InElements);
  }
}

/*****************************************************************************!
 * Function : ProcessCommandShow
 *****************************************************************************/
function
ProcessCommandShow
(InElements)
{
  if ( InElements.length < 2 ) {
    return;
  }
  if ( InElements[1] == "screen" ) {
    ProcessCommandShowScreen(InElements);
  } else if ( InElements[1] == "type" ) {
    ProcessCommandShowType(InElements);
  }
}
    
/*****************************************************************************!
 * Function : ProcessCommandShowScreen
 *****************************************************************************/
function
ProcessCommandShowScreen
(InElements)
{
  var                                   screenname;
  var                                   screen;
  var                                   elements;
  var                                   attribute;
  var                                   i;
  
  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length == 2 ) {
      if ( elements[0] == "name" ) {
        screenname = elements[1];
        screen = FindScreenByName(screenname);
        if ( null == screen ) {
          SetMessageError("\"" + screenname + "\" could not be found");
          return;
        }
        MainShowType(screen.dataAttributes);
        return;
      }
    }
  }
}
  
/*****************************************************************************!
 * Function : ProcessCommandShowElement
 *****************************************************************************/
function
ProcessCommandShowElement
(InElements)
{

}
  
/*****************************************************************************!
 * Function : ProcessCommandShowType
 *****************************************************************************/
function
ProcessCommandShowType
(InElements)
{
  var                                   elements;
  var                                   attribute;
  var                                   i;
  
  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length == 2 ) {
      if ( elements[0] == "name" ) {
        typename = elements[1];
        type = FindTypeByName(typename);
        if ( null == type ) {
          SetMessageError("\"" + typename + "\" could not be found");
          return;
        }
        MainShowType(type);
        return;
      }
    }
  }
}

/*****************************************************************************!
 * Function : MainShowType
 *****************************************************************************/
function
MainShowType
(InType)
{
  var                                   value;
  var                                   name;
  var                                   i;
  var                                   x;
  var                                   keys;
  var                                   d, name;
  
  d = document.getElementById("MainListElementArea");
  d.style.visibility = "visible";

  d = document.getElementById("MainListElementSection");
  while (d.lastChild ) {
	  d.removeChild(d.lastChild);
  }

  keys = Object.keys(InType);
  y = 0;
  for ( i = 0 ; i < keys.length ; i++ ) {
    name = document.createElement("div");
    name.className = "ShowListElement ShowListElementName";
    name.style.top = y + "px";
    name.innerHTML = keys[i];
    d.appendChild(name);

    value = document.createElement("div");
    value.className = "ShowListElement ShowListElementValue";
    value.style.top = y + "px";
    value.innerHTML = InType[keys[i]];
    d.appendChild(value);
    
    y += 20;
  }
}

/*****************************************************************************!
 * Function : ProcessCommandCreate
 *****************************************************************************/
function
ProcessCommandCreate
(InElements)
{
  if ( InElements.length < 2 ) {
    return;
  }
  if ( InElements[1] == "screen" ) {
    ProcessCommandCreateScreen(InElements);
  } else if ( InElements[1] == "element" ) {
    ProcessCommandCreateElement(InElements);
  } else if ( InElements[1] == "type" ) {
    ProcessCommandCreateType(InElements);
  }
}

/*****************************************************************************!
 * Function : ProcessCommandCreateType
 *****************************************************************************/
function
ProcessCommandCreateType
(InElements)
{
  var                                   t;
  var                                   type;
  var                                   i, attribute, elements;

  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length != 2 ) {
      continue;
    }
    if ( elements[0] == "name" ) {
      t = FindTypeByName(elements[1]);
      if ( t != undefined ) {
        SetMessageError("Type \"" + elements[1] + "\" already exists");
        return;
      }
    }
  }  

  type = new Object();
  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length != 2 ) {
      continue;
    }
    type[elements[0]] = elements[1];
  }
  if ( type["name"] == undefined ) {
    type["name"] = GenerateTypeName();
    type.element.id = type["name"];
  }

  Types.push(type);
  AddToTypeList(type)
}

/*****************************************************************************!
 * Function : ProcessCommandCreateElement
 *****************************************************************************/
function
ProcessCommandCreateElement
(InElements)
{
  var                                   element;
  var                                   i, attribute, elements;

  element = new Object();

  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length != 2 ) {
      continue;
    }
    if ( elements[0] != "type" ) {
      continue;
    }
    type = FindTypeByName(elements[1]);

    if ( type != null ) {
      // Initialize the 'element' from the 'type'
      k = Object.keys(type);
      for ( i = 0; i < k.length; i++ ) {
        element[k[i]] = type[k[i]];
      }
    }
  }
  
  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length != 2 ) {
      continue;
    }
    if ( elements[0] == "type" ) {
      continue;
    }
    element[elements[0]] = elements[1];
  }
  CreateElement(element);
  if ( element["name"] == undefined ) {
    element["name"] = GenerateElementName();
    element.element.id = element["name"];
  }
  if ( element["text"] != undefined ) {
    element.element.innerHTML = element["text"];
  }
  if ( currentScreen ) {
    currentScreen.element.appendChild(element.element);
    currentScreen.dataElements.push(element);
  }
}

/*****************************************************************************!
 * Function : ProcessCommandCreateScreen
 *****************************************************************************/
function
ProcessCommandCreateScreen
(InElements)
{
  var                                   screen;
  var                                   i, attribute, elements;
  var                                   screenArea;

  screenArea = document.getElementById("MainScreenSection");
  screen = new Object();
  screen.dataElements = [];
  screen.dataAttributes = new Object();

  currentScreen = screen;
  Screens.push(screen);

  for ( i = 2 ; i < InElements.length; i++ ) {
    attribute = InElements[i];
    elements = SplitAssignment(attribute);
    if ( elements.length != 2 ) {
      continue;
    }
    screen[elements[0]] = elements[1];
    screen.dataAttributes[elements[0]] = elements[1];
  }
  CreateElement(screen);
  if ( screen["name"] == undefined ) {
    screen["name"] = GenerateScreenName();
    screen.element.id = screen["name"];
  }
  screenArea.appendChild(screen.element);
  AddToScreenList(screen);
  SetMainScreenAreaHeader("Current Screen : " + screen["name"]);
}

/*****************************************************************************!
 * Function : CreateElement
 *****************************************************************************/
function
CreateElement
(InScreen)
{
  var                                   d;
  
  d = document.createElement("div");
  d.style.position="absolute";
  SetElementFromObject(d, InScreen);  
  InScreen.element = d;
}

/*****************************************************************************!
 * Function : SetElementFromObject
 *****************************************************************************/
function
SetElementFromObject
(InElement, InObject)
{
  var                                   keys, i;

  keys = Object.keys(InObject);

  for (i = 0; i < keys.length; i++ ){
    key = keys[i];
    if ( key == "name" ) {
      InElement.id = InObject[key];
    } else if ( key == "position") {
      elements = InObject[key].split(",");
      if ( elements.length == 2 ) {
        InElement.style.left = elements[0] + "px";
        InElement.style.top = elements[1] + "px";
      }
    } else if ( key == "size" ) {
      elements = InObject[key].split(",");
      if ( elements.length == 2 ) {
        InElement.style.width = elements[0] + "px";
        InElement.style.height = elements[1] + "px";
      }
    } else if ( key == "align" ) {
      InElement.style.textAlign = InObject[key];
    } else if ( key ==  "left" ) {
      InElement.style.left = InObject[key] + "px";
    } else if ( key == "top" ) {
      InElement.style.top = InObject[key] + "px";
    } else if ( key == "width" ) {
      InElement.style.width = InObject[key] + "px";
    } else if ( key == "height" ) {
      InElement.style.height = InObject[key] + "px";
    } else if ( key == "bottom" ) {
      InElement.style.bottom = InObject[key] + "px";
    } else if ( key == "right" ) {
      InElement.style.right = InObject[key] + "px";
    } else if ( key == "background" ) {
      InElement.style.background = InObject[key];
    } else if ( key == "color" ) {
      InElement.style.color = InObject[key];
    } else if ( key == "radius" ) {
      InElement.style.borderRadius = InObject[key] + "px";
    } else if ( key == "border" ) {
      InElement.style.border = "solid " + InObject[key] + "px black";
    } else if ( key == "borderbottom" ) {
      InElement.style.borderBottom = "solid " + InObject[key] + "px black";
    }
  }
}
  
/*****************************************************************************!
 * Function : ProcessCommandSet
 *****************************************************************************/
function
ProcessCommandSet
(InElements)
{

}

/*****************************************************************************!
 * Function : ProcessCommandRemove
 *****************************************************************************/
function
ProcessCommandRemove
(InElements)
{

}

/*****************************************************************************!
 * Function : ProcessCommandHelp
 *****************************************************************************/
function
ProcessCommandHelp
(InElements)
{

}

/*****************************************************************************!
 * Function : GenerateElementName
 *****************************************************************************/
function
GenerateElementName
()
{
  var                                   name;

  ObjectIndex++;
  name = "Element" + ObjectIndex;
  return name;
}

/*****************************************************************************!
 * Function : GenerateScreenName
 *****************************************************************************/
function
GenerateScreenName
()
{
  var                                   name;

  ObjectIndex++;
  name = "Screen" + ObjectIndex;
  return name;
}

/*****************************************************************************!
 * Function : GenerateTypeName
 *****************************************************************************/
function
GenerateTypeName
()
{
  var                                   name;

  ObjectIndex++;
  name = "Type" + ObjectIndex;
  return name;
}

/*****************************************************************************!
 * Function : AddToScreenList
 *****************************************************************************/
function
AddToScreenList
(InScreen)
{
  var                                   n;
  var                                   c;
  var                                   d;
  var                                   list;
  if ( InScreen == null ) {
    return;
  }

  list = document.getElementById("ScreenList");
  d = document.createElement("div");
  d.className = "ListElement ScreenListElement";
  d.innerHTML = InScreen["name"];
  
  c = list.children;
  n = c.length * 20;
  d.style.top = n + "px";

  list.appendChild(d);
}

/*****************************************************************************!
 * Function : AddToTypeList
 *****************************************************************************/
function
AddToTypeList
(InType)
{
  var                                   n;
  var                                   c;
  var                                   d;
  var                                   list;
  if ( InType == null ) {
    return;
  }

  list = document.getElementById("TypeList");
  d = document.createElement("div");
  d.className = "ListElement TypeListElement";
  d.innerHTML = InType["name"];
  d.dataType = InType;
  
  c = list.children;
  n = c.length * 20;
  d.style.top = n + "px";
  d.addEventListener("click", CBTypeElementClicked, false);

  list.appendChild(d);
}

/*****************************************************************************!
 * Function : CBTypeElementClicked
 *****************************************************************************/
function
CBTypeElementClicked
(InEvent)
{
  var                                   d;

  d = InEvent.srcElement;
  console.log(d.dataType);
  MainShowType(d.dataType);
}

/*****************************************************************************!
 * Function : FindTypeByName
 *****************************************************************************/
function
FindTypeByName
(InName)
{
  var                                   i;

  for ( i = 0; i < Types.length ; i++ ) {
    if ( Types[i].name == InName ) {
      return Types[i];
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : FindScreenByScreen
 *****************************************************************************/
function
FindScreenByName
(InName)
{
  var                                   i;

  for ( i = 0; i < Screens.length ; i++ ) {
    if ( Screens[i].name == InName ) {
      return Screens[i];
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : NumberIsEven
 *****************************************************************************/
function
NumberIsEven
(InNumber)
{
  return InNumber % 2;
}

/*****************************************************************************!
 * Function : DisplayPreviousCommandLine
 *****************************************************************************/
function
DisplayPreviousCommandLine
()
{
  var                                   s;
  var                                   d;
  if ( CommandLinesCache.length == 0 ) {
    return;
  }
  if ( CommandLinesCacheIndex == -1 ) {
    CommandLinesCacheIndex = CommandLinesCache.length - 1;
  } else if ( CommandLinesCacheIndex == 0 ) {
    return;
  } else {
    CommandLinesCacheIndex--;
  }
  d = document.getElementById("InputArea");
  s = CommandLinesCache[CommandLinesCacheIndex];
  d.value = s
  d.selectionStart = s.length;
  d.selectionEnd = s.length;
}

/*****************************************************************************!
 * Function : SplitAssignment
 *****************************************************************************/
function
SplitAssignment
(InString)
{
  var                                   i;
  var                                   e;
  n = InString.length;

  i = InString.indexOf('=');
  if ( i == -1 ) {
    return [InString];
  }
  e = [InString.slice(0, i), InString.slice(i+1, n)];
  return e;
}
  
/*****************************************************************************!
 * Function : ResizeLists
 *****************************************************************************/
function
ResizeLists
()
{
  var                                   screenAreaHeight;
  var                                   screenArea;
  var                                   screenList, typeList;
  
  screenArea = document.getElementById("MainScreenArea");
  screenAreaHeight = screenArea.clientHeight;

  screenList = document.getElementById("ScreenListArea");
  typeList = document.getElementById("TypeListArea");

  screenList.style.height = (screenAreaHeight - 310) + "px";
}

/*****************************************************************************!
 * Function : CBResizeMainScreenArea
 *****************************************************************************/
function
CBResizeMainScreenArea
()
{
  ResizeLists();
}

/*****************************************************************************!
 * Function : CBMainListElementAreaClose
 *****************************************************************************/
function
CBMainListElementAreaClose
()
{
  var                                   d;

  d = document.getElementById("MainListElementArea");
  d.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : SetMainScreenAreaHeader
 *****************************************************************************/
function
SetMainScreenAreaHeader
(InHeaderString)
{
  var                                   d;

  d = document.getElementById("MainScreenAreaHeader");
  d.innerHTML = InHeaderString;
}


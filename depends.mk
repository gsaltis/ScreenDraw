HTTPServer.o: HTTPServer.c GeneralUtilities/ANSIColors.h \
 RPiBaseModules/mongoose.h GeneralUtilities/StringUtils.h \
 GeneralUtilities/MemoryManager.h HTTPServer.h WebSocketServer.h \
 WebSocketHTTPConfig.h
main.o: main.c main.h GeneralUtilities/StringUtils.h mainconfig.h \
 HTTPServer.h UserInterfaceServer.h WebSocketServer.h \
 WebSocketHTTPConfig.h GeneralUtilities/FileUtils.h \
 GeneralUtilities/StringUtils.h GeneralUtilities/json.h Screen.h \
 ScreenElement.h ScreenElementAttribute.h GeneralUtilities/JSONOut.h \
 GeneralUtilities/MemoryManager.h
Screen.o: Screen.c Screen.h GeneralUtilities/StringUtils.h \
 ScreenElement.h ScreenElementAttribute.h GeneralUtilities/json.h \
 GeneralUtilities/JSONOut.h GeneralUtilities/StringUtils.h \
 GeneralUtilities/MemoryManager.h GeneralUtilities/JSONIF.h \
 GeneralUtilities/json.h
ScreenElementAttribute.o: ScreenElementAttribute.c \
 ScreenElementAttribute.h GeneralUtilities/StringUtils.h \
 GeneralUtilities/json.h GeneralUtilities/JSONOut.h \
 GeneralUtilities/StringUtils.h GeneralUtilities/MemoryManager.h
ScreenElement.o: ScreenElement.c ScreenElement.h \
 GeneralUtilities/StringUtils.h ScreenElementAttribute.h \
 GeneralUtilities/json.h GeneralUtilities/JSONOut.h \
 GeneralUtilities/StringUtils.h GeneralUtilities/MemoryManager.h
UserInterfaceServer.o: UserInterfaceServer.c UserInterfaceServer.h \
 GeneralUtilities/StringUtils.h RPiBaseModules/linenoise.h \
 GeneralUtilities/ANSIColors.h
WebConnection.o: WebConnection.c WebConnection.h \
 RPiBaseModules/mongoose.h GeneralUtilities/MemoryManager.h \
 GeneralUtilities/StringUtils.h WebSocketHTTPConfig.h
WebSocketHTTPConfig.o: WebSocketHTTPConfig.c WebSocketHTTPConfig.h \
 GeneralUtilities/StringUtils.h GeneralUtilities/MemoryManager.h
WebSocketServer.o: WebSocketServer.c WebSocketServer.h \
 GeneralUtilities/StringUtils.h WebConnection.h RPiBaseModules/mongoose.h \
 GeneralUtilities/JSONIF.h GeneralUtilities/json.h \
 GeneralUtilities/StringUtils.h GeneralUtilities/JSONOut.h \
 GeneralUtilities/MemoryManager.h GeneralUtilities/ANSIColors.h \
 GeneralUtilities/json.h UserInterfaceServer.h main.h \
 WebSocketHTTPConfig.h

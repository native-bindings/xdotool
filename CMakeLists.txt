cmake_minimum_required(VERSION 3.10)
project(node_xdotool CXX)

set(CMAKE_CXX_STANDARD 20)
set(NODE_XDOTOOL_COMPILE_OPTIONS -Wfatal-errors)
set(NODE_XDOTOOL_INCLUDE_DIRS ${CMAKE_JS_INC} ${NODE_XDOTOOL_INCLUDE_DIRS})
set(NODE_XDOTOOL_LIBRARIES xdo ${CMAKE_JS_LIB})

set(NODE_XDOTOOL_SOURCE_FILES src/main.cpp
    src/xdo_c.h
    src/XdoTool.cpp
    src/ResourceManager.cpp
    src/XdoToolTaskWorker.cpp
    src/XdoToolTask.cpp
    src/TypeConverter.cpp
    src/tasks/SendKeysequence.cpp
    src/Screenshooter.cpp
    src/XTask.cpp
    src/XService.cpp
    src/Keyboard.cpp
    src/tasks/GetWindowProperty.cpp
    src/tasks/QueryKeymap.cpp
    src/tasks/GetWindowPID.cpp
    src/tasks/GetViewportDimensions.cpp
    src/tasks/EnterText.cpp
    src/tasks/MoveMouse.cpp
    src/tasks/GetImage.cpp
    src/tasks/WindowHasProperty.cpp
    src/tasks/GetMouseLocation.cpp
    src/tasks/ActivateWindow.cpp
    src/tasks/SearchWindows.cpp
    #src/tasks/GetActiveKeysToKeycodeList.cpp
    #src/tasks/GetActiveKeysToKeycodeList.h
    src/tasks/Sync.cpp
    src/tasks/Sync.h src/tasks/GetFocusedWindow.cpp src/tasks/GetFocusedWindow.h src/tasks/GetWindowSize.cpp src/tasks/GetWindowSize.h)

add_library(node_xdotool SHARED ${NODE_XDOTOOL_SOURCE_FILES})
set_target_properties(node_xdotool PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(node_xdotool ${NODE_XDOTOOL_LIBRARIES})
target_compile_options(node_xdotool PRIVATE ${NODE_XDOTOOL_COMPILE_OPTIONS})
target_include_directories(node_xdotool PRIVATE ${NODE_XDOTOOL_INCLUDE_DIRS})
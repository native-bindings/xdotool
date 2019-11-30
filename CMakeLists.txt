project(node_xdotool)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(NODE_XDOTOOL_COMPILE_OPTIONS -Wfatal-errors)
set(NODE_XDOTOOL_INCLUDE_DIRS ${CMAKE_JS_INC} ${NODE_PATH}/include/node ${NAN_INCLUDE_DIRECTORY} ${NODE_XDOTOOL_INCLUDE_DIRS})
set(NODE_XDOTOOL_LIBRARIES xdo ${CMAKE_JS_LIB})

option(NAN_INCLUDE_DIRECTORY "NAN bindings directory" "")
option(NODE_PATH "Node.js path" "")

set(NODE_XDOTOOL_SOURCE_FILES src/main.cpp
    src/XdoTool.cpp
    src/ResourceManager.cpp
    src/XdoToolTaskWorker.cpp
    src/XdoToolTask.cpp
    src/XTypeConverter.cpp
    src/Screenshooter.cpp
    src/XTask.cpp
    src/XService.cpp
    src/Keyboard.cpp
    src/tasks/GetWindowProperty.cpp
    src/tasks/QueryKeymap.cpp
    src/tasks/GetWindowPID.cpp
    src/tasks/GetViewportDimensions.cpp
    src/tasks/MoveMouse.cpp
    src/tasks/GetImage.cpp
    src/tasks/WindowHasProperty.cpp
    src/tasks/GetMouseLocation.cpp
    src/tasks/ActivateWindow.cpp
    src/tasks/SearchWindows.cpp
)

add_library(node_xdotool SHARED ${NODE_XDOTOOL_SOURCE_FILES})
set_target_properties(node_xdotool PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(node_xdotool ${NODE_XDOTOOL_LIBRARIES})
target_compile_options(node_xdotool PRIVATE ${NODE_XDOTOOL_COMPILE_OPTIONS})
target_include_directories(node_xdotool PRIVATE ${NODE_XDOTOOL_INCLUDE_DIRS})

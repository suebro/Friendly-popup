cmake_minimum_required(VERSION 3.15)
project(SweetPopups)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/PopupManager.cpp
    src/ui/PopupWindow.cpp
    src/input/KeyboardHook.cpp
    src/config/Messages.cpp
)

# Include directories
include_directories(src)

# Create executable
add_executable(SweetPopups ${SOURCES})

# Link libraries
target_link_libraries(SweetPopups ${CMAKE_THREAD_LIBS_INIT})

if(WIN32)
    target_link_libraries(SweetPopups user32 gdi32)
endif()
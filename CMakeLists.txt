cmake_minimum_required(VERSION 3.12)

# CMAKE configuration
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

## Cross Compiler Settings

# Crosscompiler location
#SET(CMAKE_C_COMPILER /home/lildeltaplus/pi-tool/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /home/lildeltaplus/pi-tool/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
#
## Pi Sysroot
#SET(CMAKE_FIND_ROOT_PATH /home/lildeltaplus/pi-tool/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot)
#
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
##

# Project name.
project(StickyCef)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)


#
# CEF_ROOT setup.
#

# Specify the CEF distribution version.
set(CEF_VERSION "88.2.9+g5c8711a+chromium-88.0.4324.182")

#Ensure the required version of CEF is stored locally
include(cmake/DownloadCEF.cmake)
DownloadCEF("linuxarm" "${CEF_VERSION}" "${CMAKE_SOURCE_DIR}/third_party/cef")

#let CEF add its own submodules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")


#
# Load the CEF configuration.
#

# Execute FindCEF.cmake which must exist in CMAKE_MODULE_PATH.
find_package(CEF REQUIRED)


#
# Define CEF-based targets.
#

# Include the libcef_dll_wrapper target.
# Comes from the libcef_dll/CMakeLists.txt file in the binary distribution
# directory.
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#Include OpenCV
#find_package(OpenCV REQUIRED)
#include_directories( ${OpenCV_INCLUDE_DIRS} )

#add_subdirectory(${CMAKE_SOURCE_DIR}/shared shared)
# Include application targets.
# Comes from the <target>/CMakeLists.txt file in the current directory.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
endif()


# Display configuration settings.
PRINT_CEF_CONFIG()

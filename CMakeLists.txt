cmake_minimum_required(VERSION 3.12)

# CMAKE configuration
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CONFIGURATION_TYPES Debug Release)


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

DownloadCEF("linux64" "${CEF_VERSION}" "${CMAKE_SOURCE_DIR}/third_party/cef")

#set(PROJECT_ARCH "linuxarm")
#set(CEF_PLATFORM "linuxarm")
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
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Python3 COMPONENTS Interpreter Development)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#add_subdirectory(${CMAKE_SOURCE_DIR}/shared shared)
# Include application targets.
# Comes from the <target>/CMakeLists.txt file in the current directory.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
endif()


# Display configuration settings.
PRINT_CEF_CONFIG()

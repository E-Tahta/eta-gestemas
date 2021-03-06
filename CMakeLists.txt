cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(eta-gestemas)

include(CTest)

# Enable c, and c++ language
enable_language(C CXX)

# Application name
set(GESTEMAS_APPLICATION_NAME "eta-gestemas")

# Application description
set(GESTEMAS_APPLICATION_DESCRIPTION
  "System-wide Multitouch Gesture Recognition Application.")

# Default gesture recognizers system path
set(GESTEMAS_CONFIG_SYSTEM_PATH
  "/usr/share/eta/eta-gestemas/recognizers.xml")

# User defined gesture recognizers user path
set(GESTEMAS_CONFIG_USER_PATH
  ".config/eta/eta-gestemas/recognizers.xml")

# Project version
set(GESTEMAS_VERSION_MAJOR 0)
set(GESTEMAS_VERSION_MINOR 1)
set(GESTEMAS_VERSION_PATCH 9)
set(GESTEMAS_VERSION
  "${GESTEMAS_VERSION_MAJOR}.${GESTEMAS_VERSION_MINOR}.${GESTEMAS_VERSION_PATCH}")

# Location of additional cmake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(OutOfSourceConfig)
include(BuildTypeConfig)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS
    "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

option(BUILD_GESTEMAS "Build ultimate target eta-gestemas" ON)
option(BUILD_TOOLS "Build tests that require user interaction" ON)
option(BUILD_TEST "Build automated unit tests" ON)
option(BUILD_MAN "Build manual" OFF)
option(BUILD_DOC "Build documentation" OFF)

add_subdirectory(lib)

if(BUILD_GESTEMAS)
    add_subdirectory(src)
endif()

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if(BUILD_TEST)
    add_subdirectory(test)
endif()

if(BUILD_MAN)
    add_subdirectory(man)
endif()

if(BUILD_DOC)
    add_subdirectory(doc)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/recognizers.xml
    ${CMAKE_CURRENT_BINARY_DIR}/recognizers.xml COPYONLY)
get_filename_component(GESTEMAS_CONFIG_SYSTEM_DIR
    ${GESTEMAS_CONFIG_SYSTEM_PATH} DIRECTORY)
install (FILES recognizers.xml
    DESTINATION ${GESTEMAS_CONFIG_SYSTEM_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eta_gestemas
    ${CMAKE_CURRENT_BINARY_DIR}/eta_gestemas COPYONLY)
install (FILES eta_gestemas
    DESTINATION /usr/bin
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eta-gestemas.desktop
    ${CMAKE_CURRENT_BINARY_DIR}/eta-gestemas.desktop COPYONLY)
install (FILES eta-gestemas.desktop
    DESTINATION /etc/xdg/autostart)

cmake_minimum_required(VERSION 3.2)

PROJECT(dmflags)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

ADD_DEFINITIONS(-DGFLAGS_DLL_DECL=)

IF (WIN32)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)

FILE(GLOB_RECURSE SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/src/gflags.cc
${CMAKE_CURRENT_SOURCE_DIR}/src/gflags_reporting.cc
${CMAKE_CURRENT_SOURCE_DIR}/src/gflags_completions.cc)

IF (WIN32)
    LIST(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/gflags_port.cc)
ENDIF(WIN32)

IF (WIN32)
    ADD_CUSTOM_TARGET(
        dmflags_config
        COMMAND echo "dmflags_config"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
ELSEIF (APPLE)
    ADD_CUSTOM_TARGET(
        dmflags_config
        COMMAND glibtoolize && aclocal && autoheader && autoconf && automake --add-missing && sh configure
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
ELSEIF (UNIX)
    ADD_CUSTOM_TARGET(
        dmflags_config
        COMMAND libtoolize && aclocal && autoheader && autoconf && automake --add-missing && sh configure
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
ENDIF()

ADD_LIBRARY(dmflags STATIC ${SOURCES})
ADD_DEPENDENCIES(dmflags dmflags_config)

FILE(GLOB_RECURSE SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/test/main.cc)

ADD_EXECUTABLE(dmflagstest ${SOURCES})

TARGET_LINK_LIBRARIES(dmflagstest dmflags)


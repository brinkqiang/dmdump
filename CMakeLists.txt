cmake_minimum_required(VERSION 3.21)

project(dmdump)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/ModuleImport.cmake)
include(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

add_definitions(-DUSE_DMDUMP)
add_definitions(-DGFLAGS_DLL_DECL=)

LibImport("libdmdump" "src/libdmdump")

ModuleImport("dmflags" "thirdparty/dmflags")

if (PROJECT_IS_TOP_LEVEL)


        ExeImport("tools" "libdmdump;dmflags")

        if (WIN32)
                add_custom_command(
                TARGET dmdump
                PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/depends/procdump.exe $<TARGET_FILE_DIR:dmdump>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/depends/procdump64.exe $<TARGET_FILE_DIR:dmdump>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/depends/procdump64a.exe $<TARGET_FILE_DIR:dmdump>
                )
        endif()
endif()

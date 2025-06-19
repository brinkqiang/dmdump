cmake_minimum_required(VERSION 3.8)

PROJECT(dmdump)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

ADD_DEFINITIONS(-DUSE_DMDUMP)
ADD_DEFINITIONS(-DGFLAGS_DLL_DECL=)

LibImport("libdmdump" "src/libdmdump")

ModuleImport("dmflags" "thirdparty/dmflags")

ExeImport("test" "libdmdump;dmflags")

ADD_CUSTOM_COMMAND(
TARGET dmdump
PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/tools/procdump.exe $<TARGET_FILE_DIR:dmdump>
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/tools/procdump64.exe $<TARGET_FILE_DIR:dmdump>
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/tools/procdump64a.exe $<TARGET_FILE_DIR:dmdump>
)

#INSTALL(DIRECTORY include/ DESTINATION include)
INSTALL(TARGETS dmdump
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#SET(DMDUMP_LIBS "dmdump")
#MARK_AS_ADVANCED(${DMDUMP_LIBS})
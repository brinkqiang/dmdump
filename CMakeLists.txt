cmake_minimum_required(VERSION 2.8)

PROJECT(dmdump)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

ADD_DEFINITIONS(-DUSE_DMDUMP)
ADD_DEFINITIONS(-DGFLAGS_DLL_DECL=)

LibImport("dmdump_module" "src/dmdump_module")

ModuleImport("dmflags" "thirdparty/dmflags")

ExeImport("test" "dmdump_module")

#ADD_CUSTOM_COMMAND(
#TARGET dmdumptest
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different
#${CMAKE_CURRENT_SOURCE_DIR}/config/test.csv
#$<TARGET_FILE_DIR:dmdumptest>
#)

#INSTALL(DIRECTORY include/ DESTINATION include)
INSTALL(TARGETS dmdump
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#SET(DMDUMP_LIBS "dmdump")
#MARK_AS_ADVANCED(${DMDUMP_LIBS})
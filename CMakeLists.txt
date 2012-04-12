cmake_minimum_required (VERSION 2.6)
project (ParserTools)

enable_testing()

#include_directories(/usr/include/lua5.1)

#set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
#lib/x86_64-linux-gnu
find_package(Lua51 REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
#FIND_LIBRARY(MATH_LIBRARY m)

add_subdirectory(utils)
add_subdirectory(genutils)
add_subdirectory(lexer)
add_subdirectory(lalrparser)
add_subdirectory(parserlog)
add_subdirectory(gramdefparser)
add_subdirectory(luatemplateparser)
add_subdirectory(parserluagen)




add_library(lalrparser
	src/private/gram.cpp
	src/private/lalrgram.cpp
	src/private/lalrparser.cpp
	src/private/lalrparsingtable.cpp)

#target_link_libraries(lalrparser)

add_subdirectory(test)


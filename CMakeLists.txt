cmake_minimum_required (VERSION 3.6)

project (cobra)
add_executable(cobra cobra.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT cobra)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -EHsc")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif (MSVC)


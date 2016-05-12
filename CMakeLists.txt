cmake_minimum_required (VERSION 2.6)
project (gtest)

#Enforce C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -std=c++11 -stdlib=libc++ -L/usr/local/lib -lglfw3 -framework OpenGL -framework Cocoa -framework Carbon -framework IOKit -framework CoreVideo")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -std=c++0x -stdlib=libc++ -L/usr/local/lib -lglfw3 -framework OpenGL -framework Cocoa -framework Carbon -framework IOKit -framework CoreVideo")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_executable(gtest main.cpp)

set(CMAKE_MODULE_PATH /usr/local/lib)
#set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

message( STATUS ${CMAKE_MODULE_PATH} )

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

#find_package (glfw3 REQUIRED)
#include_directories("/usr/include" "/usr/local/include")
include_directories( ${OPENGL_INCLUDE_DIRS} )

target_link_libraries(gtest ${OPENGL_LIBRARIES} )
target_link_libraries(gtest ${GLFW3_STATIC_LIBRARIES})
include(CheckCXXCompilerFlag)


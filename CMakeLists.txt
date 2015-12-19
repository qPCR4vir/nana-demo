# CMake configuration for Nana Demos
# Author: Carlos Nihelton(https://github.com/CarlosNihelton)
# carlosnsoliveira@gmail.com

cmake_minimum_required(VERSION 3.1)

project(Nana_Demos)

# set compile  flags
# if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
# endif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")


#Global MSVC definitions
if(WIN32)
	if(MSVC)
		option(WIN32_USE_MP "Set to ON to build nana with the /MP option (Visual Studio 2005 and above)." ON)
		# ??
		if(WIN32_USE_MP)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
		endif()
		
	endif(MSVC)
endif(WIN32)


#Unicode
option(USE_UNICODE "Use Unicode Character Set")
if(USE_UNICODE)
    add_definitions(-DNANA_UNICODE)
endif()

if(APPLE)
    add_definitions(-DAPPLE)
    include_directories(/opt/X11/include/)
elseif(UNIX)
    add_definitions(-Dlinux)
endif()



if(UNIX)
    find_package(Freetype)
    if (FREETYPE_FOUND)
        include_directories( ${FREETYPE_INCLUDE_DIRS})
    endif()
endif()

if(WIN32)
    add_definitions(-DWIN32)
    if(MINGW)
        add_definitions(-DMINGW)
    endif()
endif()

#Find PNG
option(ENABLE_PNG "Enable the use of PNG")
option(LIBPNG_FROM_OS "Use libpng from operating system.")
if(ENABLE_PNG)
    add_definitions(-DNANA_ENABLE_PNG)
    if(LIBPNG_FROM_OS)
        find_package(PNG)
        if (PNG_FOUND)
            include_directories( ${PNG_INCLUDE_DIRS})
            add_definitions(-DUSE_LIBPNG_FROM_OS)
        endif()
    endif()
endif()

#Find JPEG
option(ENABLE_JPEG "Enable the use of JPEG")
option(LIBJPEG_FROM_OS "Use libjpeg from operating system.")
if(ENABLE_JPEG)
    add_definitions(-DNANA_ENABLE_JPEG)
    if(LIBJPEG_FROM_OS)
        find_package(JPEG)
        if (JPEG_FOUND)
            include_directories( ${JPEG_INCLUDE_DIRS})
            add_definitions(-DUSE_LIBJPEG_FROM_OS)
        endif()
    endif()
endif()

#For my case I have separate dirs for Nana and Demos.
include_directories(${CMAKE_SOURCE_DIR}/../nana/include)
link_directories(${CMAKE_SOURCE_DIR}/../nana/ShadowBuild)

add_executable(hello_world ${CMAKE_SOURCE_DIR}/Examples/HelloWord.cpp)
set_property( TARGET hello_world PROPERTY CXX_STANDARD 14 )
target_link_libraries(hello_world nana X11 pthread rt Xft)

add_executable(file_explorer ${CMAKE_SOURCE_DIR}/file_explorer.cpp)
set_property( TARGET file_explorer PROPERTY CXX_STANDARD 14 )
target_link_libraries(file_explorer nana X11 pthread rt Xft)

add_executable(widget_show ${CMAKE_SOURCE_DIR}/widget_show.cpp)
set_property( TARGET widget_show PROPERTY CXX_STANDARD 14 )
target_link_libraries(widget_show nana X11 pthread rt Xft)

add_executable(widget_show2 ${CMAKE_SOURCE_DIR}/widget_show2.cpp)
set_property( TARGET widget_show2 PROPERTY CXX_STANDARD 14 )
target_link_libraries(widget_show2 nana X11 pthread rt Xft)

# CMake configuration for Nana Demos and examples
# Author: Ariel Vina-Rodriguez (https://github.com/qPCR4vir)
# Prerequisites: 
# Statically compiled nana.lib and the include directory of nana are in directory: ../nana/lib
# That is: your base directory have two directories: nana (with the nana.lib and the include/nana directory), 
# and nana-demo. This cmake run in the nana-demo directory

cmake_minimum_required(VERSION 2.8)

project(nana_demos)
# set(NANA_LINKS)

option(NANA_CMAKE_ENABLE_MINGW_STD_THREADS_WITH_MEGANZ "replaced boost.thread with meganz's mingw-std-threads." OFF)
option(NANA_CMAKE_ENABLE_PNG "Enable the use of PNG" OFF)
option(NANA_CMAKE_LIBPNG_FROM_OS "Use libpng from operating system." ON)
option(NANA_CMAKE_ENABLE_JPEG "Enable the use of JPEG" OFF)
option(NANA_CMAKE_LIBJPEG_FROM_OS "Use libjpeg from operating system." ON)
option(NANA_CMAKE_ENABLE_AUDIO "Enable class audio::play for PCM playback." OFF)
option(NANA_CMAKE_VERBOSE_PREPROCESSOR "Show annoying debug messages during compilation." ON)
option(NANA_CMAKE_STOP_VERBOSE_PREPROCESSOR "Stop compilation after showing the annoying debug messages." ON)
option(NANA_CMAKE_AUTOMATIC_GUI_TESTING "Activate automatic GUI testing?" OFF)

# Nana uses some build systems: MS-VS solution, MAKE, bakefile, codeblock, etc. manually optimized.
# In the future CMake could be the prefered, and maybe will be used to generate the others and the central nana repo
# will distribute all of them. But by now CMake is just one of them and all the other distributed build system
# files/projects are manually write. This current CMakeList.txt reflect this fact and that is why we don't
# generate here configurated *.h files or explicitly enumerate the sources files: anyway this CM-list
# will be "touched" to force a re-run of cmake.


#https://cmake.org/cmake-tutorial/
#https://cmake.org/cmake/help/v3.3/module/CMakeDependentOption.html?highlight=cmakedependentoption
# use CACHE FORCE  or set(ENABLE_MINGW_STD_THREADS_WITH_MEGANZ ON) or delete CMakecache.txt or the entirely build dir
# if your changes don't execute

#  It seems that project() defines essential system variables like CMAKE_FIND_LIBRARY_PREFIXES.
#  https://bbs.archlinux.org/viewtopic.php?id=84967

option(NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS "" ON)
option(NANA_CMAKE_AUTOMATIC_GUI_TESTING "Activate automatic GUI testing?" OFF)
option(NANA_CMAKE_BUILD_FreeMe "Build FreeMe (currently broken)?" OFF)


find_library(nana_lib nana
        "${CMAKE_INSTALL_PREFIX}/../nana_lib/lib"
        "${CMAKE_INSTALL_PREFIX}/../nana/lib"
        "${CMAKE_INSTALL_PREFIX}/../lib"
        "${CMAKE_CURRENT_BINARY_DIR}/../nana_lib/lib"
        "${CMAKE_CURRENT_BINARY_DIR}/../nana/lib"
        "${CMAKE_CURRENT_BINARY_DIR}/../lib"
        "${CMAKE_CURRENT_DIR}/../nana_lib/lib"
        "${CMAKE_CURRENT_DIR}/../nana/lib"
        "${CMAKE_CURRENT_DIR}/../lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nana_lib/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nana/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
        )

message("nana_lib = " ${nana_lib})
# list(APPEND NANA_LINKS  "${NANA_LINKS} ../nana_lib/lib/libnana.a" )
# set(CMAKE_EXE_LINKER_FLAGS "-l${nana_lib}")

find_path(nana_include nana/config.hpp
        "${CMAKE_INSTALL_PREFIX}/../nana_lib/include"
        "${CMAKE_INSTALL_PREFIX}/../nana/include"
        "${CMAKE_INSTALL_PREFIX}/../include"
        "${CMAKE_CURRENT_BINARY_DIR}/../nana_lib/include"
        "${CMAKE_CURRENT_BINARY_DIR}/../nana/include"
        "${CMAKE_CURRENT_BINARY_DIR}/../include"
        "${CMAKE_CURRENT_DIR}/../nana_lib/include"
        "${CMAKE_CURRENT_DIR}/../nana/include"
        "${CMAKE_CURRENT_DIR}/../include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nana_lib/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nana/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
        DOC  "Nana Include directory")

message("nana_include= " ${nana_include})
include_directories(${nana_include})



########### Compatibility with CMake 3.1
if(POLICY CMP0054)
  # http://www.cmake.org/cmake/help/v3.1/policy/CMP0054.html
  cmake_policy(SET CMP0054 OLD)
endif()

if(POLICY CMP0004)    # ignore leading space
  # http://www.cmake.org/cmake/help/v3.0/policy/CMP0004.html
  cmake_policy(SET CMP0004 OLD)
endif()

add_definitions(-DNANA_IGNORE_CONF)


########### OS

if(WIN32)
    add_definitions(-DWIN32)
    #Global MSVC definitions. You may prefer the hand-tuned sln and projects from the nana repository.
    if(MSVC)
        option(WIN32_USE_MP "Set to ON to build nana with the /MP option (Visual Studio 2005 and above)." ON)
        # ??
        if(WIN32_USE_MP)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        endif(WIN32_USE_MP)
    endif(MSVC)

    if(MINGW)
        if(NANA_CMAKE_ENABLE_MINGW_STD_THREADS_WITH_MEGANZ)
            add_definitions(-DSTD_THREAD_NOT_SUPPORTED)
            add_definitions(-DNANA_ENABLE_MINGW_STD_THREADS_WITH_MEGANZ)
        endif(NANA_CMAKE_ENABLE_MINGW_STD_THREADS_WITH_MEGANZ)
    endif(MINGW)
endif(WIN32)

if(APPLE)
    add_definitions(-DAPPLE)
    include_directories(/opt/X11/include/)
    set(NANA_LINKS "${NANA_LINKS} -L/opt/X11/lib/ -liconv")
    set(ENABLE_AUDIO OFF)
elseif(UNIX)
    add_definitions(-Dlinux)
    message("added -D linux")
endif(APPLE)

if(UNIX)
    set(NANA_LINKS "${NANA_LINKS} -lX11")
    find_package(Freetype)
    if (FREETYPE_FOUND)
        include_directories( ${FREETYPE_INCLUDE_DIRS})
        set(NANA_LINKS "${NANA_LINKS} -lXft")
    endif(FREETYPE_FOUND)
endif(UNIX)


########### Compliers
#
# Using gcc: gcc 4.8 don't support C++14 and make_unique. You may want to update at least to 4.9.
# In Windows, the gcc which come with CLion was 4.8 from MinGW. You may want to install MinGW-w64 from the
# TDM-GCC Compiler Suite for Windows which will update you to gcc 5.1.
# gcc 5.3 and 5.4 include filesytem, but you need to add the link flag: -lstdc++fs
#
# see at end of:  https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dynamic_or_shared.html

# set compile  flags
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")    # Clang || GNU

    if  ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -g")       # Clang
	
    else ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14  -Wall -g")       # GNU 
          
    endif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

endif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

# enable static linkage
if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND NOT MINGW)    # GNU || CLang not MinGW
    #set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++  -pthread")
    
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
	                                                                     # IS_GNUCXX < 5.3
    else(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
       set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lstdc++fs")    # IS_GNUCXX 5.3 or more
       set(NANA_LINKS "${NANA_LINKS} -lstdc++fs") 
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
    
endif (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND NOT MINGW)


if (APPLE AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")                    # APPLE Clang
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++")
endif ()


############# Optional libraries


#Find PNG
if(NANA_CMAKE_ENABLE_PNG)
    add_definitions(-DNANA_ENABLE_PNG)
    set(NANA_LINKS "${NANA_LINKS} -lpng")
    if(NANA_CMAKE_LIBPNG_FROM_OS)
        find_package(PNG)
        if (PNG_FOUND)
            include_directories( ${PNG_INCLUDE_DIRS})
            add_definitions(-DUSE_LIBPNG_FROM_OS)
        endif(PNG_FOUND)
    endif(NANA_CMAKE_LIBPNG_FROM_OS)
endif(NANA_CMAKE_ENABLE_PNG)

#Find JPEG
if(NANA_CMAKE_ENABLE_JPEG)
    add_definitions(-DNANA_ENABLE_JPEG)
    set(NANA_LINKS "${NANA_LINKS} -ljpeg")
    if(NANA_CMAKE_LIBJPEG_FROM_OS)
        find_package(JPEG)
        if (JPEG_FOUND)
            include_directories( ${JPEG_INCLUDE_DIRS})
            add_definitions(-DUSE_LIBJPEG_FROM_OS)
        endif(JPEG_FOUND)
    endif(NANA_CMAKE_LIBJPEG_FROM_OS)
endif(NANA_CMAKE_ENABLE_JPEG)

if(NANA_CMAKE_ENABLE_AUDIO)
    add_definitions(-DNANA_ENABLE_AUDIO)
    if(UNIX)
        find_package(ASOUND)
        if (ASOUND_FOUND)
            include_directories( ${ASOUND_INCLUDE_DIRS})
            set(NANA_LINKS "${NANA_LINKS}  -lasound")
        else(ASOUND_FOUND)
            message(FATAL_ERROR "libasound is not found")
        endif(ASOUND_FOUND)
    endif(UNIX)
endif(NANA_CMAKE_ENABLE_AUDIO)


if (NANA_CMAKE_NANA_FILESYSTEM_FORCE)
    add_definitions(-DNANA_FILESYSTEM_FORCE)
	
elseif (NANA_CMAKE_STD_FILESYSTEM_FORCE)
    add_definitions(-DSTD_FILESYSTEM_FORCE)
	
elseif (NANA_CMAKE_FIND_BOOST_FILESYSTEM OR NANA_CMAKE_BOOST_FILESYSTEM_FORCE)

	if (NANA_CMAKE_BOOST_FILESYSTEM_FORCE)
        add_definitions(-DNANA_BOOST_FILESYSTEM_FORCE)
	endif(NANA_CMAKE_BOOST_FILESYSTEM_FORCE)
	
    # https://cmake.org/cmake/help/git-master/module/FindBoost.html
	# Implicit dependencies such as Boost::filesystem requiring Boost::system will be automatically detected and satisfied, 
	# even if system is not specified when using find_package and if Boost::system is not added to target_link_libraries. 
	# If using Boost::thread, then Thread::Thread will also be added automatically.
    find_package(Boost COMPONENTS filesystem)
	if (Boost_FOUND)
      add_definitions(-DNANA_BOOST_FILESYSTEM_AVAILABLE)
	  include_directories(SYSTEM "${Boost_INCLUDE_DIR}")
	  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${Boost_LIBRARIES}")
	  # set(NANA_LINKS "${NANA_LINKS} ${Boost_LIBRARIES}")    ######  FIRST !!!!!!!!!!!!!!!!!    add   is not first
	  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Boost_LIBRARIES}")
	endif (Boost_FOUND)
	
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_STATIC_RUNTIME ON)  # ??
	#set(Boost_USE_MULTITHREADED ON)
	
endif (NANA_CMAKE_NANA_FILESYSTEM_FORCE)



if(NANA_CMAKE_VERBOSE_PREPROCESSOR)
    add_definitions(-DVERBOSE_PREPROCESSOR)
endif(NANA_CMAKE_VERBOSE_PREPROCESSOR)


 #  Headers: use INCLUDE_DIRECTORIES
 #  Libraries: use FIND_LIBRARY and link with the result of it (try to avoid LINK_DIRECTORIES)

if(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
	 add_definitions(-DNANA_AUTOMATIC_GUI_TESTING)
	 enable_testing ()
endif(NANA_CMAKE_AUTOMATIC_GUI_TESTING)

function(set_nana_test dir files)
    foreach ( test ${${files}})
        add_executable(${test} "${${dir}}${test}.cpp")
        set_property( TARGET ${test} PROPERTY CXX_STANDARD 14 )
        target_link_libraries(${test} ${nana_lib} ${NANA_LINKS})
        #if(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
        #add_custom_command( TARGET  ${test} POST_BUILD COMMAND  ${test} )
        #add_custom_target(do_always_${test} ALL COMMAND ${test})
        #add_test(${test} COMMAND ${test})
        #endif(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
        install(TARGETS ${test} RUNTIME DESTINATION bin)
        message("... to build:  ${${dir}}${test}.cpp" )
    endforeach( test ${${files}})
endfunction(set_nana_test)


set (demos_dir )
set (demos    calculator file_explorer helloworld_demo notepad   widget_show  widget_show2   )
# Pending: 

set (examples_dir Examples/)
set (examples a_group_impl       animate-bmp        audio_player       audio_player_simple  
              background-effects categ              clicked            windows-subclassing   
              dock               drag-button        draw               example.button 
              example_combox     example_listbox    example_menu       lambda_event.Cpp11 
              filebox-txt        folder_tree        folder_tree_nana   folder_tree_std 
              framework_design_1 framework_design_2 framework_design_3 label_listener
              group              HelloWord          helloword_quit     listbox_inline_widget
              inputbox           listbox_Resolver   loader_1           loader_2
              main mbox          menu_debug         menu_popuper       modal_form 
              MontiHall          place_login        png                textbox_line_number 
              stretch_image      threading          thread-pool        various_events 
              window-dragger     screen             decore   
			  )
# Pending: 

if (NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS)
	list(APPEND demos  )
    if (NANA_CMAKE_BUILD_FreeMe)
        add_definitions(-DBUILD_FreeMe)
        list(APPEND demos  FreeMe)
    endif (NANA_CMAKE_BUILD_FreeMe)
endif (NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS)


set_nana_test(demos_dir demos)
set_nana_test(examples_dir examples)
install(DIRECTORY ${examples_dir} DESTINATION ${examples_dir} )



message ( "CMAKE_CXX_COMPILER_ID   = "  ${CMAKE_CXX_COMPILER_ID})
message ( "COMPILER_IS_CLANG       = "  ${COMPILER_IS_CLANG})
message ( "CMAKE_COMPILER_IS_GNUCXX= "  ${CMAKE_COMPILER_IS_GNUCXX})
message ( "CMAKE_CXX_FLAGS         = "  ${CMAKE_CXX_FLAGS})
message ( "CMAKE_EXE_LINKER_FLAGS  = "  ${CMAKE_EXE_LINKER_FLAGS})
message ( "CMAKE_STATIC_LINKER_FLAGS= "  ${CMAKE_STATIC_LINKER_FLAGS})
message ( "NANA_LINKS              = "  ${NANA_LINKS})
message ( "DESTDIR                 = "  ${DESTDIR})
message ( "CMAKE_INSTALL_PREFIX    = "  ${CMAKE_INSTALL_PREFIX})
message ( "NANA_CMAKE_ENABLE_AUDIO = "  ${NANA_CMAKE_ENABLE_AUDIO})
message ( "CMAKE_CURRENT_SOURCE_DIR= "  ${CMAKE_CURRENT_SOURCE_DIR})
message ( "CMAKE_CURRENT_DIR       = "  ${CMAKE_CURRENT_DIR})
message ( "CMAKE_CURRENT_BINARY_DIR= "  ${CMAKE_CURRENT_BINARY_DIR})

message ( "NANA_CMAKE_FIND_BOOST_FILESYSTEM         = "  ${NANA_CMAKE_FIND_BOOST_FILESYSTEM})
message ( "NANA_CMAKE_BOOST_FILESYSTEM_FORCE        = "  ${NANA_CMAKE_BOOST_FILESYSTEM_FORCE})
message ( "NANA_CMAKE_BOOST_FILESYSTEM_INCLUDE_ROOT = "  ${NANA_CMAKE_BOOST_FILESYSTEM_INCLUDE_ROOT})
message ( "NANA_CMAKE_BOOST_FILESYSTEM_LIB          = "  ${NANA_CMAKE_BOOST_FILESYSTEM_LIB})
message ( "NANA_CMAKE_AUTOMATIC_GUI_TESTING         = "  ${NANA_CMAKE_AUTOMATIC_GUI_TESTING})
message ( "NANA_CMAKE_ADD_DEF_AUTOMATIC_GUI_TESTING = "  ${NANA_CMAKE_ADD_DEF_AUTOMATIC_GUI_TESTING})




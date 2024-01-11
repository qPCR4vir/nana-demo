# CMake configuration for Nana Demos and examples
# Author: Ariel Vina-Rodriguez (qPCR4vir)
# Prerequisites:
# The source code of nana is in directory: ../nana
# That is: your base directory have the two directories: nana (with source code of nana),
# and nana-demo (containing this CMakeList). This cmake run in the nana-demo directory.
# The used 'resources' and self the soucers will be copied into the directory "nana-test/Examples"
# and the executables in "nana-test/bin"

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(nana_demo VERSION      1.6.2
        DESCRIPTION "nana C++ GUI library demos and tests"
        HOMEPAGE_URL https://github.com/cnjinhao/nana
        LANGUAGES CXX         )

enable_testing()
add_subdirectory(../nana ../cmake-nana-build-${CMAKE_BUILD_TYPE} )
# this previus line will take the source code of nana and build it "out of tree"
# into dir '../cmake-nana-build-release/' or whatever the build type is.

# In nana-demo each example is an small executable build from just only one .cpp file.
# We will now keep the containing directory and the list of those files in two cmake variables
# that will be send to a coustom cmake function to build each test and run it. 

set (demos_dir ${CMAKE_CURRENT_LIST_DIR})
set (demos    calculator file_explorer helloworld_demo notepad   widget_show  widget_show2   )
# Pending: n.a.

set (examples_dir ${CMAKE_CURRENT_LIST_DIR}/Examples)
set (examples
        a_group_impl       animate-bmp        audio_player       audio_player_simple
        background-effects categ              clicked            
        dock               drag-button        draw               example.button
        example_combox     example_listbox    example_menu       lambda_event.Cpp11
        filebox-txt                           folder_tree_nana   folder_tree_std
        framework_design_1 framework_design_2 framework_design_3 label_listener
        group              HelloWord          helloword_quit     listbox_inline_widget
        inputbox           listbox_Resolver   loader_1           loader_2
        main mbox          menu_debug         menu_popuper       modal_form
        MontiHall          place_login        png                textbox_line_number
        stretch_image      threading          thread-pool        various_events
        window-dragger     screen             decore
        )
# Pending: n.a. windows-subclassing

# add some special and optional cases
option(NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS "" ON)
option(NANA_CMAKE_BUILD_FreeMe "Build FreeMe (currently broken)?" OFF)

if (NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS)
    list(APPEND demos  )  # uppss, currently no experiemnts...
    if (NANA_CMAKE_BUILD_FreeMe)
        add_definitions(-DBUILD_FreeMe)
        list(APPEND demos  FreeMe)
    endif (NANA_CMAKE_BUILD_FreeMe)
endif (NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS)

# copy this dir to reuse the resources (images)
install(DIRECTORY ${examples_dir} DESTINATION "nana-test")

# coustom cmake function to build each 'app' test and run it. 
# set_nana_test( dir files)  - take two arguments: the directory and the list off .cpp sources, one for each executable
function(set_nana_test dir files)
    foreach ( test ${${files}})  
        # for each filename in the list of files we keep the current name in 'test'
        # for example the 'calculator' demo is build from 'nana-demo/calculator.cpp'
        add_executable(${test} "${${dir}}/${test}.cpp")  # the source is 'dir/test.cpp'
        target_link_libraries(${test} PUBLIC nana)       # link it with nana
        if(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
            add_test(NAME ${test} COMMAND ${test} WORKING_DIRECTORY ${examples_dir})
            message("... added test:  ${test}" )
        endif(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
        install(TARGETS ${test} RUNTIME DESTINATION "nana-test/bin")
        message("... to build:  ${${dir}}/${test}.cpp" )
    endforeach( test ${${files}})
endfunction(set_nana_test)

set_nana_test(demos_dir demos)
set_nana_test(examples_dir examples)

if (NANA_CMAKE_VERBOSE_PREPROCESSOR)
    # Just for information:
    include(CMakePrintHelpers)

    cmake_print_properties(TARGETS file_explorer  PROPERTIES
            COMPILE_DEFINITIONS   COMPILE_OPTIONS    COMPILE_FLAGS  LINK_LIBRARIES
            INCLUDE_DIRECTORIES   INSTALL_NAME_DIR   LINK_FLAGS     VERSION
            ARCHIVE_OUTPUT_DIRECTORY ARCHIVE_OUTPUT_NAME INSTALL_RPATH
            )
    cmake_print_variables(CONFIG)

    message ( "CMAKE_CXX_COMPILER_ID   = "  ${CMAKE_CXX_COMPILER_ID})
    message ( "COMPILER_IS_CLANG       = "  ${COMPILER_IS_CLANG})
    message ( "CMAKE_COMPILER_IS_GNUCXX= "  ${CMAKE_COMPILER_IS_GNUCXX})
    message ( "CMAKE_CXX_COMPILER_VERSION= "  ${CMAKE_CXX_COMPILER_VERSION})
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
    message ( "CMAKE_DEBUG_POSTFIX     = "  ${CMAKE_DEBUG_POSTFIX})

    message ( "NANA_CMAKE_FIND_BOOST_FILESYSTEM         = "  ${NANA_CMAKE_FIND_BOOST_FILESYSTEM})
    message ( "NANA_CMAKE_BOOST_FILESYSTEM_FORCE        = "  ${NANA_CMAKE_BOOST_FILESYSTEM_FORCE})
    message ( "NANA_CMAKE_BOOST_FILESYSTEM_INCLUDE_ROOT = "  ${NANA_CMAKE_BOOST_FILESYSTEM_INCLUDE_ROOT})
    message ( "NANA_CMAKE_BOOST_FILESYSTEM_LIB          = "  ${NANA_CMAKE_BOOST_FILESYSTEM_LIB})
    message ( "NANA_CMAKE_AUTOMATIC_GUI_TESTING         = "  ${NANA_CMAKE_AUTOMATIC_GUI_TESTING})
    message ( "NANA_CMAKE_ADD_DEF_AUTOMATIC_GUI_TESTING = "  ${NANA_CMAKE_ADD_DEF_AUTOMATIC_GUI_TESTING})

endif()


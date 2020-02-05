# CMake configuration for Nana Demos and examples
# Author: Ariel Vina-Rodriguez (qPCR4vir)
# Prerequisites:
# The source code of nana is in directory: ../nana
# That is: your base directory have two directories: nana (with source code of nana),
# and nana-demo. This cmake run in the nana-demo directory
# deprecated:
# Statically compiled nana.lib and the include directory of nana are in directory: ../nana_lib
# That is: your base directory have two directories: nana_lib (with the nana.lib and the include/nana directory),
# and nana-demo. This cmake run in the nana-demo directory
#  |
#  +  nana-demo         (directory with this file and all the others from nana-demo)
#  +  nana_lib
#         |
#         +  lib        (directory with the staticaly complied nana library)
#         +  include    (directory with all the nana include files)

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(nana_demo VERSION      1.6.2
        DESCRIPTION "nana C++ GUI library demos and tests"
        HOMEPAGE_URL http://nanapro.org
        LANGUAGES CXX         )

enable_testing()
add_subdirectory(../nana ../cmake-nana-build-${CMAKE_BUILD_TYPE} )

option(NANA_CMAKE_INCLUDE_EXPERIMENTAL_DEMOS "" OFF)
option(NANA_CMAKE_BUILD_FreeMe "Build FreeMe (currently broken)?" OFF)

function(set_nana_test dir files)
    foreach ( test ${${files}})
        add_executable(${test} "${${dir}}/${test}.cpp")
        target_link_libraries(${test} PUBLIC nana)
        if(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
            #add_custom_command( TARGET  ${test} POST_BUILD COMMAND  ${test} )
            #add_custom_target(do_always_${test} ALL COMMAND ${test})
            add_test(NAME ${test} COMMAND ${test} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Examples)
            message("... added test:  ${test}" )
        endif(NANA_CMAKE_AUTOMATIC_GUI_TESTING)
        install(TARGETS ${test} RUNTIME DESTINATION "nana-test/bin")
        message("... to build:  ${${dir}}/${test}.cpp" )
    endforeach( test ${${files}})
endfunction(set_nana_test)


set (demos_dir ${CMAKE_CURRENT_LIST_DIR})
set (demos    calculator file_explorer helloworld_demo notepad   widget_show  widget_show2   )
# Pending:


set (examples_dir ${CMAKE_CURRENT_LIST_DIR}/Examples)
set (examples
        a_group_impl       animate-bmp        audio_player       audio_player_simple
        background-effects categ              clicked            windows-subclassing
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
# copy this dir to reuse e resources (images)
install(DIRECTORY ${examples_dir} DESTINATION "nana-test")

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


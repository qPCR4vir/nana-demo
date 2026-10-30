# Interaction tests: the widgets are driven by real mouse and keyboard input (see README.md).
# Included by the main CMakeLists.txt only with NANA_CMAKE_AUTOMATIC_GUI_TESTING.
# The helper library nana_test_input is linked only by these tests, never by nana.

if(WIN32)
    add_library(nana_test_input STATIC test_input_common.cpp test_input_win32.cpp)
    target_link_libraries(nana_test_input PUBLIC nana)		# SendInput: user32, already used by nana
else()
    find_package(X11 COMPONENTS Xtst xcb)
    if(X11_Xtst_FOUND AND X11_xcb_FOUND)
        add_library(nana_test_input STATIC test_input_common.cpp test_input_x11.cpp)
        target_link_libraries(nana_test_input PUBLIC nana ${X11_Xtst_LIB} ${X11_xcb_LIB} ${X11_X11_LIB})
        target_include_directories(nana_test_input PRIVATE ${X11_Xtst_INCLUDE_PATH} ${X11_xcb_INCLUDE_PATH})
    else()
        message(WARNING "InteractionTests: XTest (libXtst) or XCB (libxcb) not found, the interaction tests are not built")
        return()
    endif()
endif()

set(interaction_tests test_button test_menu test_tooltip test_filebox)

foreach(test ${interaction_tests})
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} PRIVATE nana_test_input)
    add_test(NAME ${test} COMMAND ${test})
    set_tests_properties(${test} PROPERTIES SKIP_RETURN_CODE 77)	# nana_test::skip_code: the input could not be sent
    message("... added interaction test:  ${test}")
endforeach()

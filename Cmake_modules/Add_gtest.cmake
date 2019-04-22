# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Created on: Apr 21, 2019
#      Author: Jakob Almqvist
#
#  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# function ADD_GTEST
# Will generate a google test target. It requires the external project google_test which should set
# GTEST_LIBS_DIR: Google test library directory
# GMOCK_LIBS_DIR: Google mock library directory
# GTEST_INCLUDE_DIRS: Google test include directory
# GMOCK_INCLUDE_DIRS: Google mock include directory
#
# Input arguments:
# NAME - String with a name for the test
# SRC  - List of source files to be linked with target
# ARGN - List of libraries to be linked with target
function(ADD_GTEST NAME SRC)

    # Generate test executable from SRC with name NAME
    add_executable(${NAME} ${SRC})
    target_compile_definitions(${NAME} PRIVATE TEST_SOURCE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")

    # Link with Google test/Google mock libraries and libraries listed in ARGN 
    target_link_libraries(${NAME} ${ARGN}
                                  ${GTEST_LIBS_DIR}/libgtest.a
                                  ${GTEST_LIBS_DIR}/libgtest_main.a
                                  ${GMOCK_LIBS_DIR}/libgmock.a
                                  ${GMOCK_LIBS_DIR}/libgmock_main.a)

    # Set Google test/Google mock include directories for test
    target_include_directories(${NAME} PRIVATE ${GTEST_INCLUDE_DIRS}
                                               ${GMOCK_INCLUDE_DIRS})

    # Make it dependent on the external project google_test
    add_dependencies(${NAME} google_test)

    # Add the test
    add_test(${NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME})

endfunction()

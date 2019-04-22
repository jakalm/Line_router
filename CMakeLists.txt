# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Created on: Apr 21, 2019
#      Author: Jakob Almqvist
#
#  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required(VERSION 3.10.2)
project (line_router)


################################### COMPILER SETTINGS ##################################################################

# Executable output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/Bin)

# Use C++11 (required)
set(CMAKE_CXX_STANDARD 11)

# Set compiler flags
# Use debug symbols
# Optimize level 3
# Enable a couple of warnings
# Disable no-psabi to not spam the compiler with warnings that doesn't matter for this project
set(COMPILER_FLAGS "-g \
                    -O3 \
                    -Werror \
                    -Wall -Wextra -Wcast-align \
                    -Wno-psabi")

# Compiler flags
# C++
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
endif()
# C
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILER_FLAGS}")
endif()


################################### THREAD #############################################################################

# Add thread library (like pthread)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)


################################### GOOGLE TEST ########################################################################

# Defines make target named test.
enable_testing()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Cmake_modules)
include(Add_gtest)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Ext/Google_test)


################################### QT #################################################################################

find_package(Qt5 5.9.5 COMPONENTS Core REQUIRED)


################################### INCLUDE DIRECTORIES ################################################################

set(LINE_ROUTER_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/Grid
                                    ${CMAKE_CURRENT_LIST_DIR}/Path_planner
                                    ${CMAKE_CURRENT_LIST_DIR}/Path_planner/A_star
                                    ${CMAKE_CURRENT_LIST_DIR}/UI/Line_router)

include_directories(${LINE_ROUTER_INCLUDE_DIRECTORIES})


################################### ADD DIRECTORIES ####################################################################

add_subdirectory(Grid)
add_subdirectory(Path_planner)
add_subdirectory(UI)


################################### EXECUTABLES ########################################################################

add_executable(line_router Line_router_main.cpp)
target_link_libraries(line_router line_router_window
                                  Qt5::Core)

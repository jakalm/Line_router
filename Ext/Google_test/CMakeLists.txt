# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Created on: Apr 21, 2019
#      Author: Jakob Almqvist
#
#  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

include(ExternalProject)

# Add the external project google_test from googletest-release-1.8.0.zip
ExternalProject_Add(google_test
                    URL ${CMAKE_CURRENT_LIST_DIR}/googletest-release-1.8.0.zip
                    URL_MD5 adfafc8512ab65fd3cf7955ef0100ff5
                    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
                    # Disable install step
                    INSTALL_COMMAND "")

# Specify include dir
ExternalProject_Get_Property(google_test source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include PARENT_SCOPE)
set(GMOCK_INCLUDE_DIRS ${source_dir}/googlemock/include PARENT_SCOPE)

# Specify link libraries
ExternalProject_Get_Property(google_test binary_dir)
set(GTEST_LIBS_DIR ${binary_dir}/googlemock/gtest PARENT_SCOPE)
set(GMOCK_LIBS_DIR ${binary_dir}/googlemock PARENT_SCOPE)

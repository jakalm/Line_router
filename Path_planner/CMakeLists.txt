# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Created on: Apr 21, 2019
#      Author: Jakob Almqvist
#
#  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

add_subdirectory(A_star)

add_library(availability_grid Availability_grid.cpp)
target_link_libraries(availability_grid grid)

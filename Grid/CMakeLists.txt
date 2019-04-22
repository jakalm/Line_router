# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Created on: Apr 21, 2019
#      Author: Jakob Almqvist
#
#  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

add_library(grid Coord_point_2D.cpp
                 Flat_point_2D.cpp)
add_subdirectory(Unit_tests)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  Created on: Apr 21, 2019
#      Author: Jakob Almqvist
#
#  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# QT 5
# Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 5.9.5 COMPONENTS Widgets REQUIRED)

# Line router paint widget
add_library(line_router_paint_widget Line_router_paint_widget.cpp)
target_link_libraries(line_router_paint_widget a_star
                                               grid
                                               Qt5::Widgets)

# Line router window UI
# This will generate UI header files from the XML-file Line_router_window.ui
QT5_WRAP_UI(LINE_ROUTER_WINDOW_UI_HEADERS Line_router_window.ui)

# Line router window
add_library(line_router_window Line_router_window.cpp ${LINE_ROUTER_WINDOW_UI_HEADERS})
target_link_libraries(line_router_window line_router_paint_widget
                                         Qt5::Widgets)

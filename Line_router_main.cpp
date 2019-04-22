/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Line_router_window.h>
#include <A_star_planner.h>
#include <Path_planner.h>

// QT headers
#include <QApplication>

// Standard library headers
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <memory>

int main(int argc, char** argv)
{
    std::cout << std::setprecision(3) << std::fixed;

    // QApplication needed for running the QT 5 GUI
    QApplication application(argc, argv);

    // Create a 600x600 path planner
    const size_t grid_width = 600;
    const size_t grid_height = 600;
    std::shared_ptr<Path_planner> path_planner = std::make_shared<A_star_planner>(grid_width, grid_height);

    // Create the Line router window
    Line_router_window line_router_window(path_planner);

    // Display the window
    line_router_window.show();

    // Execute the QApplication
    return application.exec();
}

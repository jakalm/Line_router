/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_PATH_PLANNER_PATH_PLANNER_H_
#define LINE_ROUTER_PATH_PLANNER_PATH_PLANNER_H_

#include <Availability_grid.h>
#include <Coord_point_2D.h>

// Standard library headers
#include <cstddef>
#include <memory>
#include <vector>

// Path planner interface
class Path_planner
{
public:
    // No constructor since this is a interface class

    virtual ~Path_planner()
    {
    }

    // Get a path from start point to end point
    virtual bool get_path(const Coord_point_2D& start,
                          const Coord_point_2D& end,
                          std::vector<Coord_point_2D>& path) = 0;

    // Get path planner grid width
    virtual size_t get_width() const = 0;
    // Get path planner grid height
    virtual size_t get_height() const = 0;

    // Set a new grid size (could be costly if the grid is large)
    virtual void set_grid_size(const size_t width, const size_t height) = 0;

    // Get a pointer to the currently used Availability grid. The availability grid could in this way be altered before
    // running get_path.
    virtual std::shared_ptr<Availability_grid> get_availability_grid() const = 0;

    // Set a new availability grid that will replace the currently used one
    virtual void set_availability_grid(const std::shared_ptr<Availability_grid> availability_grid) = 0;

    // Set point to available, i.e. a path could pass through this point
    virtual void set_available(const size_t x, const size_t y) = 0;
    virtual void set_available(const Coord_point_2D& point) = 0;
    // Set point to blocked, i.e. a path cannot pass through this point
    virtual void set_blocked(const size_t x, size_t y) = 0;
    virtual void set_blocked(const Coord_point_2D& point) = 0;
};

#endif // LINE_ROUTER_PATH_PLANNER_PATH_PLANNER_H_

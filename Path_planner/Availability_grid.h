/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_PATH_PLANNER_AVAILABILITY_GRID_H_
#define LINE_ROUTER_PATH_PLANNER_AVAILABILITY_GRID_H_

#include <Flat_grid_2D.h>
#include <Flat_point_2D.h>
#include <Coord_point_2D.h>

// Standard library headers
#include <cstddef>

// This bool grid is used to set available/blocked grid points. It is initialized with all true values, i.e. available.
// See Flat_grid_2D for more information about the flattened grid.
class Availability_grid : public Flat_grid_2D<bool>
{
public:
    Availability_grid(const size_t width, const size_t height);
    virtual ~Availability_grid();

    bool is_available(const size_t flat_index) const;
    bool is_available(const Flat_point_2D& point) const;

    void set_available(const size_t flat_index);
    void set_available(const Flat_point_2D& point);

    void set_blocked(const size_t flat_index);
    void set_blocked(const Flat_point_2D& point);

    // Use below function with care since they will convert the coordinates to a flat index which requires a
    // multiplication and an addition
    bool is_available(const size_t x, const size_t y) const;
    bool is_available(const Coord_point_2D& point) const;

    void set_available(const size_t x, const size_t y);
    void set_available(const Coord_point_2D& point);

    void set_blocked(const size_t x, size_t y);
    void set_blocked(const Coord_point_2D& point);
};

#endif // LINE_ROUTER_PATH_PLANNER_AVAILABILITY_GRID_H_

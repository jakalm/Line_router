/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_GRID_COORD_POINT_2D_H_
#define LINE_ROUTER_GRID_COORD_POINT_2D_H_

#include <Flat_point_2D.h>

// Standard library headers
#include <cstddef>
#include <ostream>

// Need to forward declare this since Flat_point_2D is also dependent on Coord_point_2D
class Flat_point_2D;

// A 2D point coordinate class with coordinates x and y
class Coord_point_2D
{
public:
    Coord_point_2D();
    Coord_point_2D(const size_t x, const size_t y);
    // This constructor requires a division and a modulus operation
    Coord_point_2D(const Flat_point_2D& point, const size_t width);

    virtual ~Coord_point_2D();

    size_t get_x() const;
    size_t get_y() const;

    // In a width x height grid that is build from a one dimensional (1D) array the flat index with the width coordinate
    // x and height coordinate y is defined as:
    // index = x + y * width
    size_t get_flat_index(const size_t width) const;

    // Compare operators
    bool operator==(const Coord_point_2D& other_point) const;
    bool operator!=(const Coord_point_2D& other_point) const;

protected:
    size_t x;
    size_t y;
};

// Prints the x and y coordinates to an std::ostream
std::ostream& operator<<(std::ostream& os, const Coord_point_2D& point);

#endif // LINE_ROUTER_GRID_COORD_POINT_2D_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_GRID_FLAT_POINT_2D_H_
#define LINE_ROUTER_GRID_FLAT_POINT_2D_H_

#include <Coord_point_2D.h>
#include <Flat_point_2D.h>

// Standard library headers
#include <cstddef>
#include <ostream>

// Need to forward declare this since Coord_point_2D is also dependent on Flat_point_2D
class Coord_point_2D;

// A two dimensional (2D) point that is built up from a one dimensional (1D) index. In a width x height grid with width
// coordinate x and height coordinate y the index is defined as
// flat_index = x + y*width
class Flat_point_2D
{
public:
    Flat_point_2D();
    Flat_point_2D(const size_t flat_index);
    // These two constructors requires a multiplication and a addition operation
    Flat_point_2D(const size_t x, const size_t y, const size_t width);
    Flat_point_2D(const Coord_point_2D& point, const size_t width);

    virtual ~Flat_point_2D();

    // See class description
    size_t get_flat_index() const;

    // The x and y-coordinates iare calculated from the flat index as
    // x = flat_index % width
    // y = flat_index / width
    // where the width is the width of a grid where the point lives
    size_t get_x(const size_t width) const;
    size_t get_y(const size_t width) const;

    bool operator==(const Flat_point_2D& other_point) const;
    // This operator is mainly used for sorting
    bool operator<(const Flat_point_2D& other_point) const;

protected:
    // See class description
    size_t flat_index;
};

// Prints the flat index to an std::ostream
std::ostream& operator<<(std::ostream& os, const Flat_point_2D& point);

#endif // LINE_ROUTER_GRID_FLAT_POINT_2D_H_

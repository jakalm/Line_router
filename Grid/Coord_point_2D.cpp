/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Coord_point_2D.h>
#include <Flat_point_2D.h>

// Standard library headers
#include <cstddef>

Coord_point_2D::Coord_point_2D() : x(0), y(0)
{
}

Coord_point_2D::Coord_point_2D(const size_t x, const size_t y) : x(x), y(y)
{
}

Coord_point_2D::Coord_point_2D(const Flat_point_2D& point, const size_t width) : x(point.get_x(width)),
                                                                                 y(point.get_y(width))
{
}

Coord_point_2D::~Coord_point_2D()
{
}

size_t Coord_point_2D::get_x() const
{
    return x;
}

size_t Coord_point_2D::get_y() const
{
    return y;
}

size_t Coord_point_2D::get_flat_index(const size_t width) const
{
    return x + y * width;
}

bool Coord_point_2D::operator==(const Coord_point_2D& other_point) const
{
    return (x == other_point.get_x() && y == other_point.get_y());
}

bool Coord_point_2D::operator!=(const Coord_point_2D& other_point) const
{
    return not (*this == other_point);
}

std::ostream& operator<<(std::ostream& os, const Coord_point_2D& point)
{
    os << "(x, y): (" << point.get_x() << ", " << point.get_y() << ")";
    return os;
}

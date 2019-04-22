/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Flat_point_2D.h>
#include <Coord_point_2D.h>

// Standard library headers
#include <cstddef>
#include <ostream>

Flat_point_2D::Flat_point_2D() : flat_index(0)
{
}

Flat_point_2D::Flat_point_2D(const size_t flat_index) : flat_index(flat_index)
{
}

Flat_point_2D::Flat_point_2D(const size_t x, const size_t y, const size_t width) : flat_index(x + y*width)
{
}

Flat_point_2D::Flat_point_2D(const Coord_point_2D& point, const size_t width) : Flat_point_2D(point.get_x(),
                                                                                              point.get_y(),
                                                                                              width)
{
}

Flat_point_2D::~Flat_point_2D()
{
}

size_t Flat_point_2D::get_flat_index() const
{
    return flat_index;
}

size_t Flat_point_2D::get_x(const size_t width) const
{
    return flat_index % width;
}

size_t Flat_point_2D::get_y(const size_t width) const
{
    return flat_index / width;
}

bool Flat_point_2D::operator==(const Flat_point_2D& other_point) const
{
    return flat_index == other_point.get_flat_index();
}

bool Flat_point_2D::operator<(const Flat_point_2D& other_point) const
{
    return flat_index < other_point.get_flat_index();
}

std::ostream& operator<<(std::ostream& os, const Flat_point_2D& point)
{
    os << point.get_flat_index();
    return os;
}

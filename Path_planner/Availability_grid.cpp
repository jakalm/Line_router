/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Availability_grid.h>
#include <Flat_grid_2D.h>
#include <Coord_point_2D.h>

// Standard library headers
#include <cstddef>

Availability_grid::Availability_grid(const size_t width, const size_t height) : Flat_grid_2D(width, height)
{
    std::fill(vec.begin(), vec.end(), true);
}

Availability_grid::~Availability_grid()
{
}

bool Availability_grid::is_available(const size_t flat_index) const
{
    return get(flat_index);
}

bool Availability_grid::is_available(const Flat_point_2D& point) const
{
    return is_available(point);
}

void Availability_grid::set_available(const size_t flat_index)
{
    set(flat_index, true);
}

void Availability_grid::set_available(const Flat_point_2D& point)
{
    set(point, true);
}

void Availability_grid::set_blocked(const size_t flat_index)
{
    set(flat_index, false);
}

void Availability_grid::set_blocked(const Flat_point_2D& point)
{
    set(point, false);
}

bool Availability_grid::is_available(const size_t x, const size_t y) const
{
    return get(x, y);
}
bool Availability_grid::is_available(const Coord_point_2D& point) const
{
    return get(point);
}

void Availability_grid::set_available(const size_t x, const size_t y)
{
    set(x, y, true);
}
void Availability_grid::set_available(const Coord_point_2D& point)
{
    set(point, true);
}

void Availability_grid::set_blocked(const size_t x, size_t y)
{
    set(x, y, false);
}
void Availability_grid::set_blocked(const Coord_point_2D& point)
{
    set(point, false);
}

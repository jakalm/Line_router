/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_GRID_FLAT_GRID_2D_H_
#define LINE_ROUTER_GRID_FLAT_GRID_2D_H_

#include <Coord_point_2D.h>
#include <Flat_point_2D.h>

// Standard library headers
#include <cstddef>
#include <vector>

// A two dimensional (2D) grid with dimensions height x width that contains values of type T. It is built from a one
// dimensional (1D) fixed size std::vector containing elements of type T. The elements are stored in a way such that in
// a width x height grid with width coordinate x and height coordinate y the element in index i represent the
// coordinates
// x = i % width
// y = i / width
// All get and set functions will throw an out of range exception from the std::vector if trying to set or get elements
// that is out of bounds.
template<typename T>
class Flat_grid_2D
{
public:
    // Creates a width x height grid with an initial value set to all points
    Flat_grid_2D(const size_t width, const size_t height, const T& initial_value = T()) :
                                                                                       vec(width*height, initial_value),
                                                                                       width(width),
                                                                                       height(height)
    {
    }

    virtual ~Flat_grid_2D()
    {
    }

    size_t get_width() const
    {
        return width;
    }

    size_t get_height() const
    {
        return height;
    }

    // Fill all grid points with value
    void fill(const T& value)
    {
        std::fill(vec.begin(), vec.end(), value);
    }

    // Resize and fill grid points with value
    void resize(const size_t width, const size_t height, const T& value = T())
    {
        this->width = width;
        this->height = height;
        vec.resize(width*height, value);
    }

    // Get value of element at flat_index
    T get(const size_t flat_index) const
    {
        return vec.at(flat_index);
    }
    // Get value of element at point
    T get(const Flat_point_2D& point) const
    {
        return vec.at(point.get_flat_index());
    }
    // Set value of element at flat_index
    void set(const size_t flat_index, const T& value)
    {
        vec.at(flat_index) = value;
    }
    // Set value of element at point
    void set(const Flat_point_2D& point, const T& value)
    {
        vec.at(point.get_flat_index()) = value;
    }

    // Note that the functions below requires a division and a modulus operation
    // Get value of element at coordinate x and y. The coordinates need to be converted such that
    // x = index % width
    // y = index / width
    T get(const size_t x, const size_t y) const
    {
        return vec.at(get_flat_index(x, y));
    }
    // Get value of element at coordinate point. The coordinates need to be converted as described above.
    T get(const Coord_point_2D& point) const
    {
        return vec.at(point.get_flat_index(width));
    }
    // Set value of element at coordinate x and y. The coordinates need to be converted as described above.
    void set(const size_t x, const size_t y, const T& value)
    {
        vec.at(get_flat_index(x, y)) = value;
    }
    // Set value of element at coordinate point. The coordinates need to be converted as described above.
    void set(const Coord_point_2D& point, const T& value)
    {
        vec.at(point.get_flat_index(width)) = value;
    }

protected:
    std::vector<T> vec;

private:
    size_t width;
    size_t height;

    // Convert x and y coordinates to flat_point. See class description.
    size_t get_flat_index(const size_t x, const size_t y) const
    {
        return x + y * width;
    }
};

#endif // LINE_ROUTER_GRID_FLAT_GRID_2D_H_

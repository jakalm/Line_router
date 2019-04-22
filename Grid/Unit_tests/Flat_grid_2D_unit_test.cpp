/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Flat_grid_2D.h>

// Google test header
#include <gtest/gtest.h>

// Standard library headers
#include <cstddef>

// Setup a float grid and check that the width, height, initial values are set correctly
// Also check that a value is set correctly
TEST(Flat_grid_2D, Float_grid)
{
    const size_t grid_width  = 2000;
    const size_t grid_height = 2500;
    const size_t grid_size = grid_width * grid_height;

    const float init_value = 500;
    Flat_grid_2D<float> flat_grid(grid_width, grid_height, init_value);

    EXPECT_EQ(flat_grid.get_width(),  grid_width);
    EXPECT_EQ(flat_grid.get_height(), grid_height);

    for (size_t i = 0; i < grid_size; i++)
    {
        EXPECT_FLOAT_EQ(flat_grid.get(i), init_value);
    }

    const float value = 34324.45;
    const size_t x = 35;
    const size_t y = 432;
    const Coord_point_2D coord_point(x, y);
    flat_grid.set(coord_point, value);
    EXPECT_FLOAT_EQ(flat_grid.get(x, y), value);
}

// Create a large grid (1 gigabyte) and see that all initial values are set correctly
TEST(Flat_grid_2D, Large_int32_grid)
{
    // One gig grid size
    const size_t grid_width  = 16384;
    const size_t grid_height = 16384;
    const size_t grid_size = grid_width * grid_height;

    const int32_t init_value = 10000;
    const Flat_grid_2D<int32_t> flat_grid(grid_width, grid_height, init_value);

    EXPECT_EQ(flat_grid.get_width(),  grid_width);
    EXPECT_EQ(flat_grid.get_height(), grid_height);

    for (size_t i = 0; i < grid_size; i++)
    {
        EXPECT_FLOAT_EQ(flat_grid.get(i), init_value);
    }
}

// Test that a grid throws an exception when trying to get a coordinate that is out of range
TEST(Flat_grid_2D, Get_out_of_range)
{
    const size_t grid_width  = 2000;
    const size_t grid_height = 2500;
    const size_t grid_size = grid_width * grid_height;

    const float init_value = 123.32;
    Flat_grid_2D<float> flat_grid(grid_width, grid_height, init_value);

    EXPECT_EQ(flat_grid.get_width(),  grid_width);
    EXPECT_EQ(flat_grid.get_height(), grid_height);

    for (size_t i = 0; i < grid_size; i++)
    {
        EXPECT_FLOAT_EQ(flat_grid.get(i), init_value);
    }

    const size_t x = 0;
    const size_t y = 2500;
    const Coord_point_2D coord_point(x, y);
    EXPECT_ANY_THROW(flat_grid.get(coord_point));
}

// Test that a grid throws an exception when trying to set a coordinate that is out of range
TEST(Flat_grid_2D, Set_out_of_range)
{
    const size_t grid_width  = 100;
    const size_t grid_height = 100;
    const size_t grid_size = grid_width * grid_height;

    const float init_value = 2348023.23;
    Flat_grid_2D<float> flat_grid(grid_width, grid_height, init_value);

    EXPECT_EQ(flat_grid.get_width(),  grid_width);
    EXPECT_EQ(flat_grid.get_height(), grid_height);

    for (size_t i = 0; i < grid_size; i++)
    {
        EXPECT_FLOAT_EQ(flat_grid.get(i), init_value);
    }

    const float value = 0xDEAD;
    const size_t x = 0;
    const size_t y = 100;
    const Coord_point_2D coord_point(x, y);
    EXPECT_ANY_THROW(flat_grid.set(coord_point, value));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <A_star_planner.h>
#include <Availability_grid.h>
#include <Coord_point_2D.h>

// Google test header
#include <gtest/gtest.h>

// Standard library headers
#include <cstddef>

TEST(A_star_planner, Simple_open_area)
{
    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(2, 2);

    A_star_planner a_star_planner(availability_grid);

    std::vector<Coord_point_2D> path;
    ASSERT_TRUE(a_star_planner.get_path(Coord_point_2D(0, 0), Coord_point_2D(1, 1), path));

    ASSERT_EQ(path.size(),  size_t(2));
    EXPECT_EQ(path.front(), Coord_point_2D(0, 0));
    EXPECT_EQ(path.back(),  Coord_point_2D(1, 1));
}

TEST(A_star_planner, Normal_sized_open_area)
{
    const size_t grid_width  = 600;
    const size_t grid_height = grid_width;

    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(grid_width,
                                                                                                     grid_height);
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(grid_width-1, grid_height-1);

    A_star_planner a_star_planner(availability_grid);

    std::vector<Coord_point_2D> path;
    ASSERT_TRUE(a_star_planner.get_path(start_point, end_point, path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }
}

TEST(A_star_planner, Normal_sized_area_with_diagonal_block)
{
    const size_t grid_width  = 600;
    const size_t grid_height = grid_width;

    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(grid_width,
                                                                                                     grid_height);
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(grid_width-1, grid_height-1);

    // Create a diagonal with only one possible point to pass through like this (scaled down)
    // S A A X
    // A A X A   S = Start, E = End, X = Block, A = Available
    // A X A A
    // A A A E
    for (size_t x = 1; x < grid_width; x++)
    {
        const size_t y = grid_width-x-1;
        availability_grid->set_blocked(x, y);
    }

    A_star_planner a_star_planner(availability_grid);

    std::vector<Coord_point_2D> path;
    ASSERT_TRUE(a_star_planner.get_path(start_point, end_point, path));

    // Best path would be to go straight down to point 0,598 (599 points used) and 1 step diagonal to 1,599 and then
    // right to 599,599 (598 points used) which in total gives 1198 points used including start and end point
    ASSERT_EQ(path.size(), size_t(grid_height + grid_width - 2));
    for (size_t i = 0; i < path.size(); i++)
    {
        if (i < grid_height - 1)
        {
            // Check that it moves straight down to 0,998
            EXPECT_EQ(path.at(i), Coord_point_2D(0, i));
        }
        else if (i == grid_height-1)
        {
            // Check that it moves diagonal to 1,999
            EXPECT_EQ(path.at(i), Coord_point_2D(1, grid_height-1));
        }
        else
        {
            // Check that it moves straight right to 999,999
            EXPECT_EQ(path.at(i), Coord_point_2D(i-grid_height+2, grid_height-1));
        }
    }
}

TEST(A_star_planner, Large_open_area)
{
    const size_t grid_width  = 2000;
    const size_t grid_height = grid_width;

    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(grid_width,
                                                                                                     grid_height);
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(grid_width-1, grid_height-1);

    A_star_planner a_star_planner(availability_grid);

    std::vector<Coord_point_2D> path;
    ASSERT_TRUE(a_star_planner.get_path(start_point, end_point, path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }
}

TEST(A_star_planner, Large_area_with_diagonal_block)
{
    const size_t grid_width  = 2000;
    const size_t grid_height = grid_width;

    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(grid_width,
                                                                                                     grid_height);
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(grid_width-1, grid_height-1);

    // Create a diagonal with only one possible point to pass through like this (scaled down)
    // S A A X
    // A A X A   S = Start, E = End, X = Block, A = Available
    // A X A A
    // A A A E
    for (size_t x = 1; x < grid_width; x++)
    {
        const size_t y = grid_width-x-1;
        availability_grid->set_blocked(x, y);
    }

    A_star_planner a_star_planner(availability_grid);

    std::vector<Coord_point_2D> path;
    ASSERT_TRUE(a_star_planner.get_path(start_point, end_point, path));

    // Best path would be to go straight down to point 0,1998 (1999 points used) and 1 step diagonal to 1,1999 and then
    // right to 1999,1999 (1998 points used) which in total gives 3998 points used including start and end point
    ASSERT_EQ(path.size(), size_t(grid_height + grid_width - 2));
    for (size_t i = 0; i < path.size(); i++)
    {
        if (i < grid_height - 1)
        {
            // Check that it moves straight down to 0,998
            EXPECT_EQ(path.at(i), Coord_point_2D(0, i));
        }
        else if (i == grid_height-1)
        {
            // Check that it moves diagonal to 1,999
            EXPECT_EQ(path.at(i), Coord_point_2D(1, grid_height-1));
        }
        else
        {
            // Check that it moves straight right to 999,999
            EXPECT_EQ(path.at(i), Coord_point_2D(i-grid_height+2, grid_height-1));
        }
    }
}

TEST(A_star_planner, Impossible_to_reach_end_point)
{
    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(100, 100);
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(99, 99);

    A_star_planner a_star_planner(availability_grid);
    std::vector<Coord_point_2D> path;

    // Availability map should be updated inside A_star_planner whenever the shared_ptr is changed
    availability_grid->set_blocked(98,98);
    availability_grid->set_blocked(99,98);
    availability_grid->set_blocked(98,99);

    EXPECT_FALSE(a_star_planner.get_path(start_point, end_point, path));
}

TEST(A_star_planner, End_point_outside_border)
{
    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(1000, 1000);

    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(1000, 0); // End point is just outside border

    A_star_planner a_star_planner(availability_grid);
    std::vector<Coord_point_2D> path;

    EXPECT_FALSE(a_star_planner.get_path(start_point, end_point, path));
}


TEST(A_star_planner, End_point_trapped_in_the_middle)
{
    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(100, 100);

    // Setup end point to be trapped like this in the middle of the grid:
    // X X X
    // X E X   E = End, X = Block
    // X X X
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(50, 50);
    availability_grid->set_blocked(49, 49);
    availability_grid->set_blocked(49, 50);
    availability_grid->set_blocked(49, 51);
    availability_grid->set_blocked(50, 51);
    availability_grid->set_blocked(51, 51);
    availability_grid->set_blocked(51, 50);
    availability_grid->set_blocked(51, 49);
    availability_grid->set_blocked(50, 49);

    A_star_planner a_star_planner(availability_grid);
    std::vector<Coord_point_2D> path;

    EXPECT_FALSE(a_star_planner.get_path(start_point, end_point, path));
}

TEST(A_star_planner, End_point_trapped_in_top_right_corner)
{
    const std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(100, 100);

    // Setup end point to be trapped like this:
    // B B B
    // X E B   E = End, X = Block, B = Border
    // X X B
    const Coord_point_2D start_point(0, 0);
    const Coord_point_2D end_point(99, 99);
    availability_grid->set_blocked(98, 99);
    availability_grid->set_blocked(98,98);
    availability_grid->set_blocked(99,98);

    A_star_planner a_star_planner(availability_grid);
    std::vector<Coord_point_2D> path;

    EXPECT_FALSE(a_star_planner.get_path(start_point, end_point, path));
}

TEST(A_star_planner, Change_of_grid_size)
{
    // Start with a 100 x 100 grid
    size_t grid_width = 100;
    size_t grid_height = 100;
    A_star_planner a_star_planner(grid_width, grid_height);

    std::vector<Coord_point_2D> path;

    ASSERT_TRUE(a_star_planner.get_path(Coord_point_2D(0,0), Coord_point_2D(grid_width-1, grid_width-1), path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }

    // Increase width and height by 1
    grid_width++;
    grid_height++;
    std::shared_ptr<Availability_grid> availability_grid = std::make_shared<Availability_grid>(grid_width,
                                                                                                     grid_height);
    a_star_planner.set_availability_grid(availability_grid);

    ASSERT_TRUE(a_star_planner.get_path(Coord_point_2D(0,0), Coord_point_2D(grid_width-1, grid_width-1), path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }

    // Decrease width and height by 2
    grid_width -= 2;
    grid_height -= 2;
    ASSERT_TRUE(a_star_planner.get_path(Coord_point_2D(0,0), Coord_point_2D(grid_width-1, grid_width-1), path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }

    // Decrease availability grid by 1 outside of a_star_planner
    grid_width -= 1;
    grid_height -= 1;
    availability_grid->resize(grid_width, grid_height, true);
    ASSERT_TRUE(a_star_planner.get_path(Coord_point_2D(0,0), Coord_point_2D(grid_width-1, grid_width-1), path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }

    // Decrease width and height by 5 by using the a_star_planner set_grid_size
    grid_width -= 5;
    grid_height -= 5;
    a_star_planner.set_grid_size(grid_width, grid_height);
    ASSERT_TRUE(a_star_planner.get_path(Coord_point_2D(0,0), Coord_point_2D(grid_width-1, grid_width-1), path));

    // Best path would be to go diagonal to end point
    ASSERT_EQ(path.size(), size_t(grid_width));
    for (size_t i = 0; i < path.size()-1; i++)
    {
        EXPECT_EQ(path.at(i), Coord_point_2D(i, i));
    }
}

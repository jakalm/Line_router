/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_PATH_PLANNER_A_STAR_A_STAR_PLANNER_H_
#define LINE_ROUTER_PATH_PLANNER_A_STAR_A_STAR_PLANNER_H_

#include <Availability_grid.h>
#include <Path_planner.h>
#include <Coord_point_2D.h>
#include <Flat_point_2D.h>
#include <Flat_grid_2D.h>

// Standard library headers
#include <memory>

// This class tries to find the route from start to end based on the search algorithm A*. It is designed to find the
// path from the start point to the given end point which has the smallest cost. In this implementation the cost is the
// coordinate distance traveled. For more information about the algorithm, see README.md in Line router application.
// The Availability grid is a bool grid used to set available/blocked grid points and also defines the width and height
// of the grid.
// This class is intended to be accessed by one thread since it is not thread safe.
class A_star_planner : public Path_planner
{
public:
    // Create an A_star_planner with an already existing availability grid. The availability grid must have been
    // initialized before calling this. The grid size will be fetched from the availability grid.
    A_star_planner(std::shared_ptr<Availability_grid> availability_grid);
    // Create an A_star_planner with a grid size of width x height. It will also initialize an all available
    // width x height availability grid.
    A_star_planner(const size_t width, const size_t height);

    virtual ~A_star_planner();

    // Get a path from start point to end point
    // Returns a vector with path where first element is the start point and last is the end point
    bool get_path(const Coord_point_2D& start, const Coord_point_2D& end, std::vector<Coord_point_2D>& path) override;

    // Get path planner grid width
    size_t get_width() const override;
    // Get path planner grid height
    size_t get_height() const override;

    // Set a new grid size (could be costly if the grid is large)
    void set_grid_size(const size_t width, const size_t height) override;

    // Get a pointer to the currently used Availability grid. The availability grid could in this way be altered before
    // running get_path.
    std::shared_ptr<Availability_grid> get_availability_grid() const override;

    // Set a new availability grid that will replace the currently used one
    void set_availability_grid(const std::shared_ptr<Availability_grid> availability_grid) override;

    // Set point to available, i.e. a path could pass through this point
    void set_available(const size_t x, const size_t y) override;
    void set_available(const Coord_point_2D& point) override;
    // Set point to blocked, i.e. a path cannot pass through this point
    void set_blocked(const size_t x, size_t y) override;
    void set_blocked(const Coord_point_2D& point) override;

private:
    std::shared_ptr<Availability_grid> availability_grid;

    size_t width;
    size_t height;

    // This grid is used to keep track of the cost from start point to the the point in the grid. Start point has
    // a cost of zero.
    Flat_grid_2D<float> path_cost_grid;

    // This grid consists of flattened grid indexes that points to a previous neighbor point visited. It is
    // updated by the currently visited point that sets all its available neighbors to the currently visited flattened
    // grid index. When the end point has been reached this grid can be used to backtrack the path to the start point.
    Flat_grid_2D<size_t> path_grid;

    // Reconstruct the path from start point to end point by using the path_grid. This should only be called once the
    // end point is set in the path_grid.
    bool reconstruct_path(const Coord_point_2D& start,
                          const Flat_point_2D& end,
                          std::vector<Coord_point_2D>& path_vector) const;

    // Get the horizontal, vertical and diagonal available neighbors of a given point. Diagonal neighbors are considered
    // to be available if at least one of the nearest neighbors to the diagonal neighbor is available. Examples:
    // P = point, X = blocked, A = Available
    //       Up, Right and Upper right available | Up and Upper right available | All blocked
    //              X A A                        |             X A A            |   A X A
    //              X P A                        |             X P X            |   X P X
    //              X X X                        |             X X X            |   A X A
    // The Neighbors typedef is an 8 sized pair of a flat point and a bool. The bool states if the neighbor is a
    // diagonal point or not. This could be useful if the cost for a diagonal step is different than a horizontal and
    // vertical step.
    // Return value is the number of available neighbors and thus the number of elements filled to the Neighbor array.
    typedef std::array<std::pair<Flat_point_2D, bool>, 8> Neighbors;
    size_t get_neighbors(const Flat_point_2D& point, Neighbors& neighbors) const;

    // Cheapest cost to target point is the cheapest cost of the path from point to the target point. It is often
    // denoted by h. In this implementation the cost is set to the line-of-sight distance from point to the target point
    // by the equation sqrt(dx^2 + dy^2), where dx is the difference in x-coordinate and dy is the difference in
    // y-coordinate. In a grid-space this is not the true distance since a move is only possible in horizontal, vertical
    // or diagonal. But it works as a good approximation.
    float calculate_cheapest_cost_to_target(const Coord_point_2D& point, const Coord_point_2D& target) const;
};

#endif // LINE_ROUTER_PATH_PLANNER_A_STAR_A_STAR_PLANNER_H_

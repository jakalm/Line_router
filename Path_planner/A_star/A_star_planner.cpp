/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <A_star_planner.h>
#include <Cost_point_2D.h>
#include <Flat_point_2D.h>

// Standard library headers
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

A_star_planner::A_star_planner(std::shared_ptr<Availability_grid> availability_grid) :
                                                  availability_grid(availability_grid),
                                                  width(availability_grid->get_width()),
                                                  height(availability_grid->get_height()),
                                                  path_cost_grid(width, height, std::numeric_limits<float>::infinity()),
                                                  path_grid(width, height, 0)
{
}

A_star_planner::A_star_planner(const size_t width, const size_t height) :
                                                      A_star_planner(std::make_shared<Availability_grid>(width, height))
{
}

A_star_planner::~A_star_planner()
{
}

bool A_star_planner::get_path(const Coord_point_2D& start, const Coord_point_2D& end, std::vector<Coord_point_2D>& path)
{
    if (not availability_grid)
    {
        throw "A_star_planner::get_path: Availability grid not set";
    }

    if (availability_grid->get_width() != width || availability_grid->get_height() != height)
    {
        // Availability grid has been altered outside of this class. Grid need to be resized
        set_grid_size(availability_grid->get_width(), availability_grid->get_height());
    }

    if (start.get_x() >= width || start.get_y() >= height)
    {
        std::cout << "WARNING: A_star_planner: Start point out of bounds" << std::endl;
        return false;
    }

    if (end.get_x() >= width || end.get_y() >= height)
    {
        std::cout << "WARNING: A_star_planner: End point out of bounds" << std::endl;
        return false;
    }

    if (start == end)
    {
        // Already at end point from the beginning
        path.push_back(start);

        return true;
    }

    // Fill cost grid with infinite numbers, except for start point which should have zero cost.
    // Path grid does not need to be reset since it will not be used until a path is found and it has been rebuild
    path_cost_grid.fill(std::numeric_limits<float>::infinity());
    path_cost_grid.set(start, 0);

    // Clear the output path vector
    path.clear();

    // Set start point total cost and add it to the points to visit queue
    const float cheapest_cost_to_end_point = calculate_cheapest_cost_to_target(start, end);
    const Cost_point_2D start_point(start, width, cheapest_cost_to_end_point);
    std::priority_queue<Cost_point_2D> points_to_visit;
    points_to_visit.push(start_point);

    // Initialize the neighbors array
    Neighbors neighbors;

    while (points_to_visit.empty() == false)
    {
        // Pop the point which have the lowest total cost
        const Flat_point_2D current_point = points_to_visit.top();
        points_to_visit.pop();

        if (Coord_point_2D(current_point, width) == end)
        {
            // End point reached, reconstruct the path
            return reconstruct_path(start, current_point, path);
        }

        // Path cost is the cost from start point to current point. It is often denoted by g. In this implementation
        // the cost is set to the distance traveled in number of points. A diagonal move is counted as a distance of
        // sqrt(1^2+1^2)~=1.4142136 points.
        float path_cost_current_cell = path_cost_grid.get(current_point);

        // Get the neighbors of the current cell
        const size_t number_of_neighbors = get_neighbors(current_point, neighbors);
        for (size_t neighbor_index = 0; neighbor_index < number_of_neighbors; neighbor_index++)
        {
            const Flat_point_2D& neighbor_point = neighbors.at(neighbor_index).first;
            const bool is_diagonal = neighbors.at(neighbor_index).second;

            float path_cost = path_cost_current_cell;
            if (is_diagonal)
            {
                // A diagonal move cost is set to sqrt(1^2 + 1^2) ~= 1.4142136 points
                path_cost += 1.4142136;
            }
            else
            {
                // A horizontal or vertical move is set to one
                path_cost += 1;
            }

            if (path_cost < path_cost_grid.get(neighbor_point))
            {
                // If path cost is less than the current path cost for that point, update the path cost grid and path
                // grid add this point to point_to_visit queue.

                // Estimate the cheapest cost to end point
                const float cheapest_cost_to_end_point = calculate_cheapest_cost_to_target(
                                                                                          Coord_point_2D(neighbor_point,
                                                                                                         width),
                                                                                                         end);

                // The total cost is the cheapest possible cost from start point to neighbor point plus the estimated
                // cheapest cost to end point. A* function f = g + h.
                const float total_cost = path_cost + cheapest_cost_to_end_point;

                // Add point to points_to_visit queue
                const Cost_point_2D neighbor_cost_point(neighbor_point, total_cost);
                points_to_visit.push(neighbor_cost_point);

                // Update path cost grid with path cost
                path_cost_grid.set(neighbor_point, path_cost);

                // Set path grid point to current point
                path_grid.set(neighbor_point, current_point.get_flat_index());
            }
        }
    }

    std::cout << "Failed to plan path from: " << start << " to " << end << std::endl;

    return false;
}

size_t A_star_planner::get_width() const
{
    return width;
}

size_t A_star_planner::get_height() const
{
    return height;
}

void A_star_planner::set_grid_size(const size_t width, const size_t height)
{
    if (not availability_grid || (availability_grid->get_width() != width || availability_grid->get_height()))
    {
        availability_grid->resize(width, height, true);
    }

    if (path_cost_grid.get_width() != width || path_cost_grid.get_height() != height)
    {
        path_cost_grid.resize(width, height, std::numeric_limits<float>::infinity());
    }

    if (path_grid.get_width() != width || path_grid.get_height() != height)
    {
        path_grid.resize(width, height, 0);
    }

    this->width = width;
    this->height = height;
}

std::shared_ptr<Availability_grid> A_star_planner::get_availability_grid() const
{
    return availability_grid;
}

void A_star_planner::set_availability_grid(std::shared_ptr<Availability_grid> availability_grid)
{
    this->availability_grid =  availability_grid;

    set_grid_size(availability_grid->get_width(), availability_grid->get_height());
}

void A_star_planner::set_available(const size_t x, const size_t y)
{
    if (availability_grid)
    {
        availability_grid->set_available(x, y);
    }
    else
    {
        throw "A_star_planner::set_available: Availability grid not set when trying to set point to available";
    }
}

void A_star_planner::set_available(const Coord_point_2D& point)
{
    if (availability_grid)
    {
        availability_grid->set_available(point);
    }
    else
    {
        throw "A_star_planner::set_available: Availability grid not set when trying to set point to available";
    }
}

void A_star_planner::set_blocked(const size_t x, size_t y)
{
    if (availability_grid)
    {
        availability_grid->set_blocked(x, y);
    }
    else
    {
        throw "A_star_planner::set_available: Availability grid not set when trying to set point to available";
    }
}

void A_star_planner::set_blocked(const Coord_point_2D& point)
{
    if (availability_grid)
    {
        availability_grid->set_blocked(point);
    }
    else
    {
        throw "A_star_planner::set_available: Availability grid not set when trying to set point to available";
    }
}

bool A_star_planner::reconstruct_path(const Coord_point_2D& start,
                                      const Flat_point_2D& end,
                                      std::vector<Coord_point_2D>& path_vector) const
{
    // This function will do a reverse search from end point to start point from the path grid

    // Clear the path vector
    path_vector.clear();

    // Add end point to path
    path_vector.push_back(Coord_point_2D(end, width));

    // Get the next index to travel to
    size_t next_index = path_grid.get(end);

    // Continue to get the next index until we have reached the start cell. It shouldn't take more than width x height
    // iterations until the start point have been reached
    const size_t max_iterations = width * height;
    size_t number_of_iterations = 0;
    while (next_index != start.get_flat_index(width) && number_of_iterations < max_iterations)
    {
        number_of_iterations++;
        Coord_point_2D next_point(Flat_point_2D(next_index), width);
        next_index = path_grid.get(next_index);
        path_vector.push_back(next_point);
    }

    if (number_of_iterations >= max_iterations)
    {
        std::cout << "ERROR: Maximum iterations to reconstruct the path from start to end reached" << std::endl;
        return false;
    }

    // Check that the last point is the start point
    Coord_point_2D last_point(Flat_point_2D(next_index), width);
    if (last_point != start)
    {
        std::cout << "ERROR: Last point in reconstructed path is not the start point" << std::endl;
        return false;
    }

    // Add start point to path
    path_vector.push_back(last_point);

    // Reverse the order so that the start point is first and end point is last
    std::reverse(path_vector.begin(), path_vector.end());

    return true;
}

size_t A_star_planner::get_neighbors(const Flat_point_2D& point, Neighbors& neighbors) const
{
    size_t number_of_neighbors = 0;

    // Get x and y coordinates
    const size_t x = point.get_x(width);
    const size_t y = point.get_y(width);

    // Check if horizontal and vertical coordinates are within limits
    const bool left_within_limits  = x > 0;
    const bool right_within_limits = x < width - 1;
    const bool up_within_limits    = y > 0;
    const bool down_within_limits  = y < height - 1;

    bool left_available = false;
    bool up_available = false;
    bool right_available = false;
    bool down_available = false;

    // This is the flat index of the point and will be in the center of its neighbors
    const size_t center_index = point.get_flat_index();

    // Code below checks if neighbor is available and if so adds it to the neighbors array

    // Left neighbor
    Flat_point_2D left;
    if (left_within_limits)
    {
        left_available = availability_grid->is_available(center_index-1);
        if (left_available)
        {
            left = Flat_point_2D(center_index-1);
            neighbors.at(number_of_neighbors).first = left;
            neighbors.at(number_of_neighbors).second = false;
            number_of_neighbors++;
        }
    }

    // Up neighbor
    Flat_point_2D up;
    if (up_within_limits)
    {
        up_available = availability_grid->is_available(center_index-width);
        if (up_available)
        {
            up = Flat_point_2D(center_index-width);
            neighbors.at(number_of_neighbors).first = up;
            neighbors.at(number_of_neighbors).second = false;
            number_of_neighbors++;
        }
    }

    // Right neighbor
    Flat_point_2D right;
    if (right_within_limits)
    {
        right_available = availability_grid->is_available(center_index+1);
        if (right_available)
        {
            right = Flat_point_2D(center_index+1);
            neighbors.at(number_of_neighbors).first = right;
            neighbors.at(number_of_neighbors).second = false;
            number_of_neighbors++;
        }
    }

    // Down neighbor
    Flat_point_2D down;
    if (down_within_limits)
    {
        down_available = availability_grid->is_available(center_index+width);
        if (down_available)
        {
            down = Flat_point_2D(center_index+width);
            neighbors.at(number_of_neighbors).first = down;
            neighbors.at(number_of_neighbors).second = false;
            number_of_neighbors++;
        }
    }

    // Diagonal neighbors

    // Upper left neighbor
    if (up_within_limits && left_within_limits)
    {
        if (availability_grid->is_available(center_index-width-1) && (up_available || left_available))
        {
            const Flat_point_2D upper_left(center_index-width-1);
            neighbors.at(number_of_neighbors).first = upper_left;
            neighbors.at(number_of_neighbors).second = true;
            number_of_neighbors++;
        }
    }

    // Upper right neighbor
    if (up_within_limits && right_within_limits)
    {
        if (availability_grid->is_available(center_index-width+1) && (up_available || right_available))
        {
            const Flat_point_2D upper_right(center_index-width+1);
            neighbors.at(number_of_neighbors).first = upper_right;
            neighbors.at(number_of_neighbors).second = true;
            number_of_neighbors++;
        }
    }

    // Down right neighbor
    if (down_within_limits && right_within_limits)
    {
        if (availability_grid->is_available(center_index+width+1) && (down_available || right_available))
        {
            const Flat_point_2D lower_right(center_index+width+1);
            neighbors.at(number_of_neighbors).first = lower_right;
            neighbors.at(number_of_neighbors).second = true;
            number_of_neighbors++;
        }
    }

    // Down left neighbor
    if (down_within_limits && left_within_limits)
    {
        if (availability_grid->is_available(center_index+width-1) && (down_available || left_available))
        {
            const Flat_point_2D lower_left(center_index+width-1);
            neighbors.at(number_of_neighbors).first = lower_left;
            neighbors.at(number_of_neighbors).second = true;
            number_of_neighbors++;
        }
    }

    return number_of_neighbors;
}

float A_star_planner::calculate_cheapest_cost_to_target(const Coord_point_2D& point, const Coord_point_2D& target) const
{
    // Check that the coordinates are within range of an ssize_t.
    if (target.get_x() > std::numeric_limits<ssize_t>::max() || target.get_y() > std::numeric_limits<ssize_t>::max())
    {
        throw "A_star_planner::calculate_cheapest_cost_to_target: Out of range coordinates";
    }

    const float dx = static_cast<ssize_t>(target.get_x()) - static_cast<ssize_t>(point.get_x());
    const float dy = static_cast<ssize_t>(target.get_y()) - static_cast<ssize_t>(point.get_y());

    return std::sqrt(dx*dx + dy*dy);
}

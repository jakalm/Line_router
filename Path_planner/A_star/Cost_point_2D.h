/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_PATH_PLANNER_A_STAR_COST_POINT_2D_H_
#define LINE_ROUTER_PATH_PLANNER_A_STAR_COST_POINT_2D_H_

#include <Flat_point_2D.h>
#include <Coord_point_2D.h>

// Standard library headers
#include <cstddef>

// This class is mainly used together with a sorted container for sorting points with a cost value. But could also be
// used to keep track of cost for a given point.
class Cost_point_2D : public Flat_point_2D
{
public:
    Cost_point_2D(const size_t flat_index, const float cost);
    Cost_point_2D(const Flat_point_2D& point, const float cost);
    // The constructors below need to convert the x and y coordinate to a flat grid index. It requires a multiplication
    // and a addition operation.
    Cost_point_2D(const size_t x, const size_t y, const size_t width, const float cost);
    Cost_point_2D(const Coord_point_2D& point, const size_t width, const float cost);

    virtual ~Cost_point_2D();

    // Get cost for the point
    float get_cost() const;

    // This is for sorting purposes. It will sort by cost in an ascending order.
    bool operator<(const Cost_point_2D& other_point) const;

private:
    float cost;
};

#endif // LINE_ROUTER_PATH_PLANNER_A_STAR_COST_POINT_2D_H_

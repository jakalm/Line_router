/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Cost_point_2D.h>
#include <Coord_point_2D.h>

// Standard library headers
#include <cstddef>

Cost_point_2D::Cost_point_2D(const size_t flat_index, const float cost) : Flat_point_2D(flat_index), cost(cost)
{

}

Cost_point_2D::Cost_point_2D(const Flat_point_2D& point, const float cost) : Flat_point_2D(point), cost(cost)
{

}

Cost_point_2D::Cost_point_2D(const size_t x, const size_t y, const size_t width, const float cost) :
                                                                                             Flat_point_2D(x, y, width),
                                                                                             cost(cost)
{
}

Cost_point_2D::Cost_point_2D(const Coord_point_2D& point, const size_t width, const float cost) :
                                                                                            Flat_point_2D(point, width),
                                                                                            cost(cost)
{
}

Cost_point_2D::~Cost_point_2D()
{
}

float Cost_point_2D::get_cost() const
{
    return cost;
}

bool Cost_point_2D::operator<(const Cost_point_2D& other_point) const
{
     return get_cost() > other_point.get_cost();
}

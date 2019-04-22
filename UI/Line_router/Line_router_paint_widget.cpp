/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Line_router_paint_widget.h>
#include <Path_planner.h>
#include <Coord_point_2D.h>

// QT headers
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>

// Standard library headers
#include <cstddef>
#include <memory>
#include <vector>
#include <iostream>

Line_router_paint_widget::Line_router_paint_widget(const std::shared_ptr<Path_planner> path_planner,
                                                   QWidget* parent) : QWidget(parent),
                                                                      // zero indexed coordinates
                                                                      x_lower_bound(0),
                                                                      y_lower_bound(0),
                                                                      x_upper_bound(path_planner->get_width() - 1),
                                                                      y_upper_bound(path_planner->get_height() - 1),
                                                                      start_point_set(false),
                                                                      end_point_set(false),
                                                                      start_point_marked(false),
                                                                      path_planner(path_planner),
                                                                      pixmap(path_planner->get_width(),
                                                                             path_planner->get_height())
{
    // Fill pixmap background
    pixmap.fill(Qt::black);

    if (not this->path_planner)
    {
        throw "Line_router_paint_widget::Line_router_paint_widget: Path planner is not set";
    }
}

Line_router_paint_widget::~Line_router_paint_widget()
{
}

void Line_router_paint_widget::mousePressEvent(QMouseEvent* mouse_event)
{
    // Maximum coordinate is one less than width and height
    if (mouse_event->pos().x() < 0 || mouse_event->pos().x() > pixmap.width() - 1 ||
        mouse_event->pos().y() < 0 || mouse_event->pos().y() > pixmap.height() - 1)
    {
        std::cout << "WARNING: Position out of bounds" << std::endl;
        return;
    }

    if (not start_point_set)
    {
        // If start point is not set, do it now and run a paintEvent
        line_start = mouse_event->pos();
        start_point_set = true;
        update(); // This runs a paintEvent
    }
    else
    {
        // If start point is set, set the end point and run a paintEvent
        line_end = mouse_event->pos();
        end_point_set = true;
        update(); // This runs a paintEvent
    }
}

void Line_router_paint_widget::paintEvent(QPaintEvent*)
{
    if (not path_planner)
    {
        std::cout << "ERROR: Path planner not set" << std::endl;
        return;
    }

    QPainter painter(this);

    if (start_point_set && not start_point_marked)
    {
        // Draw current pixmap
        painter.drawPixmap(0, 0, pixmap);

        // Mark first start point, this will not be stored in the pixmap
        mark_point(painter, line_start);

        start_point_marked = true;
    }
    else if (end_point_set)
    {
        // End point set, draw the line if possible to find a clear path

        QPainter pixmap_painter(&pixmap);

        // Rotate the colors
        static Qt::GlobalColor color = Qt::lightGray;
        color = Qt::GlobalColor(3 + ((color-2) % 16));
        QPen pen(color);
        pixmap_painter.setPen(pen);

        // Set the start and end coordinates
        const Coord_point_2D start(line_start.x(), line_start.y());
        const Coord_point_2D end(line_end.x(), line_end.y());

        // Run path planning to find the path
        std::vector<Coord_point_2D> path;
        if (path_planner->get_path(start, end, path))
        {
            // If successful, draw all points in path
            for (const Coord_point_2D& point : path)
            {
                // Set point to blocked
                path_planner->set_blocked(point.get_x(), point.get_y());
                // Also set neighbor points to blocked to make it clear that the lines are not intersecting
                block_neighbors(point);

                // Draw point, this will update the pixmap
                pixmap_painter.drawPoint(point.get_x(), point.get_y());
            }
        }

        // Draw the pixmap here to reset the marked point if the path planning were unsuccessful
        painter.drawPixmap(0, 0, pixmap);

        // Reset the states to wait for the first point to be set again
        start_point_set = false;
        end_point_set = false;
        start_point_marked = false;
    }
    else
    {
        // Keep the pixmap even if there is a mouse click outside of the window
        painter.drawPixmap(0, 0, pixmap);

        if (start_point_set)
        {
            // Mark the start point if start point is set
            mark_point(painter, line_start);
        }
    }
}

void Line_router_paint_widget::mark_point(QPainter& painter, const QPoint& point)
{
    // Create a red round pen to mark the point
    QPen pen;
    pen.setBrush(Qt::red);
    pen.setWidth(10);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    // Set the pen for painter
    painter.setPen(pen);

    // Draw the point
    painter.drawPoint(point);
}

void Line_router_paint_widget::block_neighbors(const Coord_point_2D& point)
{
    if (point.get_x() > x_lower_bound)
    {
        // x is within lower bound, block left neighbor
        path_planner->set_blocked(point.get_x()-1, point.get_y());

        if (point.get_y() > y_lower_bound)
        {
            // y is also within lower bound, block upper left neighbor
            path_planner->set_blocked(point.get_x()-1, point.get_y()-1);
        }
        if (point.get_y() < y_upper_bound)
        {
            // y is within upper bound, block lower left neighbor
            path_planner->set_blocked(point.get_x()-1, point.get_y()+1);
        }
    }

    if (point.get_y() > y_lower_bound)
    {
        // y is within lower bound, block upper neighbor
        path_planner->set_blocked(point.get_x(), point.get_y()-1);

        if (point.get_x() < x_upper_bound)
        {
            // x is within upper bound, block upper right neighbor
            path_planner->set_blocked(point.get_x()+1, point.get_y()-1);
        }
    }

    if (point.get_x() < x_upper_bound)
    {
        // x is within upper bound, block right neighbor
        path_planner->set_blocked(point.get_x()+1, point.get_y());

        if (point.get_y() < y_upper_bound)
        {
            // y is also within upper bound, block lower right neighbor
            path_planner->set_blocked(point.get_x()+1, point.get_y()+1);
        }
    }

    if (point.get_y() < y_upper_bound)
    {
        // y is within upper bound, block lower neighbor
        path_planner->set_blocked(point.get_x(), point.get_y()+1);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_UI_LINE_ROUTER_LINE_ROUTER_PAINT_WIDGET_H_
#define LINE_ROUTER_UI_LINE_ROUTER_LINE_ROUTER_PAINT_WIDGET_H_

#include <Path_planner.h>
#include <Coord_point_2D.h>

// QT headers
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QPoint>
#include <QPainter>

// Standard library headers
#include <cstddef>
#include <memory>

// The Line_router_paint_widget sets up the pixel map environment where it is possible to draw lines. It will wait for a
// mouse click on a point that is inside of the pixel map and set that point to be the start point. Then the start point
// is marked with a red dot on the pixel map. The second mouse click inside the pixel map will set the end point and
// will pass the start and end point to the Path_planner. If the Path_planner is successful in finding a path from start
// to end a line will be drawn on the given path and all the points passed will be set to blocked in the Path_planner.
// If the Path_planner is unsuccessful in finding a path it will just unmark the first point and wait for the first
// mouse click again.
class Line_router_paint_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Line_router_paint_widget(const std::shared_ptr<Path_planner> path_planner,
                                      QWidget* parent = 0);
    ~Line_router_paint_widget();

protected:
    // Enter this function when a mouse click happens
    void mousePressEvent(QMouseEvent* mouse_event) override;

    // Trigger paint event based on mouse clicks
    void paintEvent(QPaintEvent* paint_event) override;

private:
    // Lower and upper bounds of the pixmap
    size_t x_lower_bound;
    size_t y_lower_bound;
    size_t x_upper_bound;
    size_t y_upper_bound;

    // Bools to keep track on what state the widget is in
    bool start_point_set;
    bool end_point_set;
    bool start_point_marked;

    std::shared_ptr<Path_planner> path_planner;

    // Pixmap will contain the drawn lines
    QPixmap pixmap;

    // Start and end points that will be set by mouse clicks
    QPoint line_start;
    QPoint line_end;

    // This will mark a point on the Widget. NOTE: It will not be on the pixmap.
    void mark_point(QPainter& painter, const QPoint& point);

    // Block horizontal, vertical and diagonal neighbors for a given point. This is used to make it more clear that the
    // lines does not intersect. The side-effect is that the path planner will not find a path in a one pixel width
    // corridor.
    void block_neighbors(const Coord_point_2D& point);
};

#endif // LINE_ROUTER_UI_LINE_ROUTER_LINE_ROUTER_PAINT_WIDGET_H_

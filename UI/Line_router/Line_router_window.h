/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LINE_ROUTER_UI_LINE_ROUTER_LINE_ROUTER_WINDOW_H_
#define LINE_ROUTER_UI_LINE_ROUTER_LINE_ROUTER_WINDOW_H_

#include <Path_planner.h>

// QT headers
#include <QMainWindow>
#include <QWidget>

// Standard library headers
#include <cstddef>
#include <memory>

// Need to forward declare Line_router_window since it should be generated and included as source for the
// line_router_window library
namespace Ui
{
    class Line_router_window;
}

class Line_router_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Line_router_window(const std::shared_ptr<Path_planner> path_planner,
                                QWidget *parent = 0);
    ~Line_router_window();

private:
    Ui::Line_router_window *ui;
};

#endif // LINE_ROUTER_UI_LINE_ROUTER_LINE_ROUTER_WINDOW_H_

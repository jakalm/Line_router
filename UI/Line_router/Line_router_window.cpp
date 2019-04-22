/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Created on: Apr 21, 2019
 *      Author: Jakob Almqvist
 *
 *  Copyright (C) 2019 Jakob Almqvist. All rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <Line_router_window.h>
#include <Path_planner.h>
#include <Line_router_paint_widget.h>

// Generated QT UI from XML file Main_window.ui
#include <ui_Line_router_window.h>

// QT headers
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>

// Standard library headers
#include <cstddef>
#include <memory>

Line_router_window::Line_router_window(std::shared_ptr<Path_planner> path_planner,
                                       QWidget *parent) : QMainWindow(parent),
                                                          ui(new Ui::Line_router_window)
{
    ui->setupUi(this);

    QHBoxLayout* hbox = new QHBoxLayout();
    ui->centralWidget->setLayout(hbox);

    // Make the window a bit bigger than the grid
    const size_t window_width = path_planner->get_width() + 20;
    const size_t window_height = path_planner->get_height() + 20;
    setGeometry(0, 0, window_width, window_height);

    hbox->addWidget(new Line_router_paint_widget(path_planner));
}

Line_router_window::~Line_router_window()
{
    delete ui;
}


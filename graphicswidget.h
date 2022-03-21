#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>
#include "appmanager.h"


class GraphicsWidget : public QGraphicsView
{
public:
    GraphicsWidget();
    QString get_fuction();


private:
    QGraphicsScene scene;
    QLineEdit* ledit;
    Painter* cvs;

    void init_elements();
};

#endif // GRAPHICSWIDGET_H

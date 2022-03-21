#ifndef PAINTER_H
#define PAINTER_H

#pragma once

#include <QVector2D>
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <vector>
#include <QKeyEvent>
#include "function.h"
#include "funcvector.h"
#include "settings.h"

class Painter : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Painter();
    Painter(int width, int height);

    double scale_x = 30, scale_y = 30;

    //Painting
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //Events
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;


signals:
    void broadcastSquereQueiry(const std::vector<FuncVector>&);
    void broadcastSquereQueiryM(const QVector2D& v);

public slots:
    void listenFunction(const Function& f) {
        funcs_.push_back(f);
    }
    void listenCrossPoint(const FuncVector& p) {
        cross_points_.push_back(p);
    }
    void listenSquereInfo(long double S) {
        S_ = S;
    }

private:
    int width_ = 700, height_ = 700;
    QPixmap pixmap_;
    std::vector<Function> funcs_;
    std::vector<FuncVector> cross_points_;

    std::vector<FuncVector> active_points_;
    QVector2D anchor_;

    long double S_ = 0;

    void fillPixmap();
};


#endif // PAINTER_H

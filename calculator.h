#ifndef CALCULATOR_H
#define CALCULATOR_H

#pragma once

#include <QObject>
#include <QPointF>
#include "function.h"
#include <vector>
#include "funcvector.h"
#include "settings.h"
#include <QDebug>


class Calculator : public QObject
{
    Q_OBJECT
public:
    Calculator();

    std::vector<long double> find_root(const Function& f, long double a, long double b);
    long double find_integral(const Function& f, long double a, long double b);
    long double find_integral_square(std::vector<FuncVector> funvecs);


signals:
    void broadcastFunction(const Function&);
    void broadcastCrossPoints(const FuncVector&);
    void broadcastSqureInfo(long double S);

public slots:
    void listenFunction(const Function& func) {
        emit broadcastFunction(func);
        for (Function f: funcs_) {
            std::vector<long double> roots = find_root(f - func, Settings::instance().LHS, Settings::instance().RHS);
            for (long double x: roots) {
                Function tmp[2];
                tmp[0] = f;
                tmp[1] = func;
                try {
                    cross_points_.push_back(FuncVector(tmp, QVector2D(x, f(x))));
                    emit broadcastCrossPoints(FuncVector(tmp, QVector2D(x, f(x))));
                }  catch (...) {}
            }
        }
        funcs_.push_back(func);
    }
    void listenSquereQueiry(const std::vector<FuncVector>& v) {
        emit broadcastSqureInfo(find_integral_square(v));
    }
    void listenSquereQueiryM(const QVector2D& v) {
        emit broadcastSqureInfo(find_area_squre(v.toPointF(), Settings::instance().LHS, Settings::instance().RHS));
    }

private:
    std::vector<Function> funcs_;
    std::vector<FuncVector> cross_points_;

    long double calc_root(const Function& f, long double a, long double b);
    bool is_same_area(QPointF dot1, QPointF dot2);
    long double find_area_squre(QPointF anchor, long double lbnd, long double rbnd);
};

#endif // CALCULATOR_H

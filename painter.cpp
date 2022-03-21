#include "painter.h"

Painter::Painter()
{


}


void Painter::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QVector2D point = QVector2D(event->pos()) - QVector2D(width_ / 2, height_ / 2);
    point *= QVector2D(1 / scale_x, -1 / scale_y);

    if (Settings::instance().SQUERE_METOD == Settings::MONTECARLO) {
        anchor_ = point;
        update();
        emit broadcastSquereQueiryM(anchor_);
        return;
    }

    if (!active_points_.empty() && (active_points_[active_points_.size() - 1].p - point).length() * scale_x <= 10) {
        active_points_.pop_back();
        update();
        return;
    }

    if (!active_points_.empty() && (active_points_[0].p - point).length() * scale_x <= 10) {
        if (active_points_.size() < 3) return;
        emit broadcastSquereQueiry(active_points_);
        update();
        return;
    }

    for (FuncVector v: cross_points_) {
        auto p = v.p;
        if ((p - point).length() * scale_x <= 10) {
            active_points_.push_back(v);
        }
    }
    update();
}


Painter::Painter(int width, int height) : width_(width), height_(height) {

}


QRectF Painter::boundingRect() const {
    return QRectF(0, 0, width_, height_);
}


void Painter::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    fillPixmap();
    painter->drawPixmap(0, 0, width_, height_, pixmap_);

    painter->setPen(Qt::white);
    painter->drawText(0, 15, "Squere: " + QString::fromStdString(std::to_string(S_)));
}


void Painter::fillPixmap() {
    pixmap_ = QPixmap(width_, height_);

    QPainter painter(&pixmap_);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, pixmap_.width(), pixmap_.height());


    QMatrix m;
    m.translate(pixmap_.width() / 2, pixmap_.height() / 2);
    m.scale(1, -1);

    painter.setTransform(QTransform(m));

    long double lbnd_x = -pixmap_.width() / 2 / scale_x, rbnd_x = pixmap_.width() / 2 / scale_x;

    // NET
    painter.setPen(QColor(100, 100, 100, 100));
    for (double x = -width_; x < width_; x += 1) {
        painter.drawLine(x * scale_x, -height_ * scale_y, x * scale_x, height_ * scale_y);
    }
    for (double y = -height_; y < height_; y += 1) {
        painter.drawLine(-width_ * scale_x, y * scale_y, width_ * scale_x, y * scale_y);
    }

    // OX OY
    painter.setPen(QPen(Qt::white));
    painter.drawLine(0, -pixmap_.height() / 2, 0, pixmap_.height() / 2);
    painter.drawLine(-pixmap_.width() / 2, 0, pixmap_.width() / 2, 0);


    // GRAPHICS
    painter.setPen(QPen(Qt::red));
    for (Function f: funcs_) {
        for (double x = lbnd_x, dx = 1 / scale_x; x < rbnd_x; x += dx) {
            try {
                painter.drawLine((x - dx) * scale_x, f(x - dx) * scale_y, x * scale_x, f(x) * scale_y);
            }  catch (...) {

            }
        }
    }

    painter.setPen(QPen(Qt::blue, 5));
    for (FuncVector v: cross_points_) {
        auto p = v.p;
        painter.drawPoint(p.x() * scale_x, p.y() * scale_y);
    }

    painter.setPen(QPen(Qt::yellow, 5));
    if (Settings::instance().SQUERE_METOD == Settings::INTEGRAL) {
        for (FuncVector v: active_points_) {
            auto p = v.p;
            painter.drawPoint(p.x() * scale_x, p.y() * scale_y);
        }
    } else {
        painter.drawPoint((anchor_ * QVector2D(scale_x, scale_y)).toPointF());
    }


}

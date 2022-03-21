#include "graphicswidget.h"
#include <QtDebug>


GraphicsWidget::GraphicsWidget()
{
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    setFrameStyle(0);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(700, 700);

    setSceneRect(QRect(0, 0, 700, 700));
    setScene(&scene);
    init_elements();
}


void GraphicsWidget::init_elements() {
    cvs = &AppManager::instance().painter;
    cvs->setPos(0, 0);
    scene.addItem(cvs);
}

//void GraphicsWidget::onBtnClicked(Qt::MouseButton btn) {
//    qWarning() << "connected";
//}

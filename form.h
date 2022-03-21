#ifndef FORM_H
#define FORM_H

#pragma once

#include <QWidget>
#include "graphicswidget.h"
#include "appmanager.h"
#include <QLineEdit>


namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == '+') {
            AppManager::instance().painter.scale_x += 1;
            AppManager::instance().painter.scale_y += 1;
        } else if (event->key() == '-') {
            AppManager::instance().painter.scale_x -= 1;
            AppManager::instance().painter.scale_y -= 1;
        } else if (event->key() == 'M') {
            Settings::instance().SQUERE_METOD = Settings::MONTECARLO;
        } else if (event->key() == 'I') {
            Settings::instance().SQUERE_METOD = Settings::INTEGRAL;
        }
        AppManager::instance().painter.update();
    }

//signals:
//    void update_func(const QString& str);

//private slots:
//    void returnPressed();
//    void listItemDoubleClicked(QListWidgetItem *item);

private:
    Ui::Form *ui;
    QLineEdit* ledit;
    QListWidget* list;
    GraphicsWidget* gw;
};


#endif // FORM_H

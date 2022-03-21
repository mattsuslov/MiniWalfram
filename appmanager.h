#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include "painter.h"
#include "reader.h"
#include "calculator.h"
#include "funcvector.h"


class AppManager : public QObject
{
    Q_OBJECT
public:
    AppManager();

    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;

    static AppManager& instance() {
        static AppManager res;
        return res;
    };

    Reader reader;
    Painter painter;
    Calculator calculator;
};

#endif // APPMANAGER_H

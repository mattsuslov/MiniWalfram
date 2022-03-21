#include <graphicswidget.h>
#include <QApplication>
#include <form.h>
#include "appmanager.h"

int main(int argc, char *argv[])
{
    srand(0);
    QApplication a(argc, argv);

    AppManager::instance();

    Form w;
    w.show();

    return a.exec();
}

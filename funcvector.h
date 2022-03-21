#ifndef FUNCVECTOR_H
#define FUNCVECTOR_H


#include <QVector2D>
#include "function.h"

class FuncVector
{
public:
    FuncVector();
    FuncVector(Function f[2], QVector2D v) : p(v) {
        func[0] = f[0];
        func[1] = f[1];
    };

    Function func[2] = {};
    QVector2D p;
};

#endif // FUNCVECTOR_H

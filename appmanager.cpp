#include "appmanager.h"

AppManager::AppManager()
{
    connect(&calculator, &Calculator::broadcastSqureInfo, &painter, &Painter::listenSquereInfo);
    connect(&painter, &Painter::broadcastSquereQueiry, &calculator, &Calculator::listenSquereQueiry);
    connect(&painter, &Painter::broadcastSquereQueiryM, &calculator, &Calculator::listenSquereQueiryM);
    connect(&calculator, &Calculator::broadcastCrossPoints, &painter, &Painter::listenCrossPoint);
    connect(&calculator, &Calculator::broadcastFunction, &painter, &Painter::listenFunction);


    std::vector<Function> funcs;
//    funcs.push_back(Function() + Monomial(0.6) * Term(1) + Monomial(3));
//    funcs.push_back(Function() + Monomial(1) * Term(3) - Monomial(6) * Term(2) + Monomial(12) * Term(1) - Monomial(9));
//    funcs.push_back(Function() + Monomial(3) * Term(-1, new Function(Function() + Monomial(1) * Term(1))));
    funcs.push_back(Function() + Monomial(1) * Term(1) - Monomial(1));
    funcs.push_back(Function() - Monomial(1) * Term(1) - Monomial(1));
    funcs.push_back(Function() + Monomial(1) * Term(1) + Monomial(1));
    funcs.push_back(Function() - Monomial(1) * Term(1) + Monomial(1));
    funcs.push_back(Function(Monomial(1) * Term(2)));


    for (const Function& f: funcs) {
        calculator.listenFunction(f);
    }
}

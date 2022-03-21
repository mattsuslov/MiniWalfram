#ifndef SETTINGS_H
#define SETTINGS_H

#include <random>
#include <time.h>

class Settings
{
public:
    Settings();

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    static Settings& instance() {
        static Settings res;
        return res;
    };


    enum SquereMetod {
        INTEGRAL,
        MONTECARLO
    };


    long double LHS = -10, RHS = 10;
    long double EPSM = 0.01;
    long double EPSI = 0.00001l;
    long double EPS = 0.000001l;
    SquereMetod SQUERE_METOD;
    std::mt19937 rnd;
};

#endif // SETTINGS_H

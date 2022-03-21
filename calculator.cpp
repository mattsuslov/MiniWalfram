#include "calculator.h"
#include <QDebug>

Calculator::Calculator()
{

}


bool Calculator::is_same_area(QPointF dot1, QPointF dot2) {
    for (Function& f: funcs_) {
        if ((dot1.y() - f(dot1.x())) * (dot2.y() - f(dot2.x())) < 0) return false;
        std::vector<long double> asimp = f.potential_asimptot();
        for (long double x: asimp) {
            if ((x - dot1.x()) * (x - dot2.x()) < 0) return false;
        }
    }
    return true;
}


long double Calculator::find_area_squre(QPointF anchor, long double lbnd, long double rbnd) {
    std::vector<long double> xs;
    std::vector<long double> ys;


    for (int i = 0; i < funcs_.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            std::vector<long double> roots = find_root(funcs_[i] - funcs_[j], lbnd, rbnd);
            for (auto x: roots) {
                xs.push_back(x);
                ys.push_back(funcs_[i](x));
            }
        }
    }


    qWarning("0 lev");
    for (int i = 0; i < xs.size(); ++i) {
        std::cout << xs[i] << std::endl;
    }

    for (Function f: funcs_) {
        auto der_roots = find_root(+f, lbnd, rbnd);
        for (auto x: der_roots) {
            xs.push_back(x);
            ys.push_back(f(x));
        }
    }

    if (xs.empty()) return 0;

    qWarning("1 lev");
    long double max_x = xs[0], min_x = xs[0], max_y = ys[0], min_y = ys[0];
    for (int i = 1; i < xs.size(); ++i) {
        max_x = std::max(xs[i], max_x);
        min_x = std::min(xs[i], min_x);
    }
    for (int i = 1; i < ys.size(); ++i) {
        max_y = std::max(ys[i], max_y);
        min_y = std::min(ys[i], min_y);
    }
    if ((anchor.x() < min_x || anchor.x() > max_x) || (anchor.y() < min_y || anchor.y() > max_y)) return 0;

    qWarning("2 lev");

    double S = (max_x - min_x) * (max_y - min_y);
    int n = 1000;
    int k = 0;
    for (int i = 0; i < n; ++i) {
        float random = std::abs(((float) Settings::instance().rnd()) / (float) Settings::instance().rnd._Max);
        double x = random * (max_x - min_x) + min_x;
        random = std::abs(((float) Settings::instance().rnd()) / (float) Settings::instance().rnd._Max);
        double y = random * (max_x - min_x) + min_x;
        k += is_same_area(QPointF(x, y), anchor);
    }
    double S_old = S;
    for (int its = 0; std::abs(S_old - S * k / n) >= Settings::instance().EPSM; ++its) {
        S_old = S * k / n;
        for (int i = 0; i < n; ++i) {
            float random = std::abs(((float) Settings::instance().rnd()) / (float) Settings::instance().rnd._Max);
            double x = random * (max_x - min_x) + min_x;
            random = std::abs(((float) Settings::instance().rnd()) / (float) Settings::instance().rnd._Max);
            double y = random * (max_x - min_x) + min_x;
            k += is_same_area(QPointF(x, y), anchor);
        }
        n *= 2;
    }
    return S_old;
}


long double Calculator::find_integral_square(std::vector<FuncVector> funvecs) {
    long double ans = 0;
    int n = funvecs.size();
    for (int i = 0; i < n; ++i) {
        bool f = false;
        for (int j = 0; j < 2 && !f; ++j) {
            for (int k = 0; k < 2 && !f; ++k) {
                {
                    bool flag = false;
                    std::vector<long double> tmp = funvecs[(i+1) % n].func[j].potential_asimptot();
                    for (long double x: tmp) {
                        if (abs(abs(x - funvecs[(i+1) % n].p.x()) + abs(x - funvecs[i].p.x()) - abs(funvecs[(i+1) % n].p.x() - funvecs[i].p.x())) <= Settings::instance().EPS) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag) continue;
                }

                if (funvecs[(i+1) % n].func[j] == funvecs[i].func[k]) {
                    std::cout << funvecs[(i+1) % n].func[j] << " " << funvecs[i].func[k] << std::endl;
                    if (funvecs[i].p.x() <= funvecs[(i+1) % n].p.x()) {
                        ans += find_integral(funvecs[(i+1) % n].func[j], funvecs[i].p.x(), funvecs[(i+1) % n].p.x());
                    } else {
                        ans -= find_integral(funvecs[(i+1) % n].func[j], funvecs[(i+1) % n].p.x(), funvecs[i].p.x());
                    }
                    f = true;
                }
            }
        }
        if (!f) throw std::runtime_error("Bad input");
    }
    return abs(ans);
}


std::vector<long double>& operator+=(std::vector<long double>& a, const std::vector<long double>& b) {
    for (long double el: b) {
        a.push_back(el);
    }
    return a;
}

std::vector<long double> Calculator::find_root(const Function& f, long double a, long double b) {
    if (f.isSimple()) return {};
    std::vector<long double> bounds = {a};
    if (!f.isMono()) {
        bounds += f.potential_asimptot();
        bounds += Calculator::find_root(+f, a, b);
        bounds += Calculator::find_root(+(+f), a, b);
    }
    bounds += {b};
    sort(bounds.begin(), bounds.end());
    std::vector<long double> roots;
    for (int i = 1; i < bounds.size(); ++i) {
        try {
            if (abs(f(bounds[i-1])) <= Settings::instance().EPSM) {
                roots.push_back(bounds[i-1]);
                continue;
            }
        }  catch (...) {}

        try {
            if (abs(f(bounds[i])) <= Settings::instance().EPSM) continue;
        }  catch (...) {}

        try {
            if (f(bounds[i-1] + Settings::instance().EPSI) * f(bounds[i] - Settings::instance().EPSI) > 0) continue;

        } catch (...) {}

        roots.push_back(Calculator::calc_root(f, bounds[i-1] + Settings::instance().EPSI, bounds[i] - Settings::instance().EPSI));
    }
    return roots;
}

long double Calculator::calc_root(const Function&f, long double a, long double b) {
    Function der1, der2;
    der1 = +f;
    der2 = +der1;
    while (b - a > Settings::instance().EPS) {
        long double k = (f(b) - f(a)) / (b - a);
        long double sh = f(a) - k * a;
        long double c = -sh / k;
        if (der1(c) * der2(c) > 0) {
            a = c;
            b = b - f(b) / der1(b);
        } else {
            a = a - f(a) / der1(a);
            b = c;
        }
    }
    return a;
}

long double Calculator::find_integral(const Function& f, long double a, long double b) {
    long double S_old = 0;
    long long n = 128;
    while (n <= 1e9+7) {
        long double S = 0;
        long double dx = (b - a) / n;
        for (long double x = a; x <= b; x += dx) {
            S += f(x + dx / 2) * dx;
        }
        if (abs(S_old - S) / 3.0 <= Settings::instance().EPSI) return S;
        S_old = S;
        n *= 2;
    }
}

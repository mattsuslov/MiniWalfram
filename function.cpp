#include "function.h"
#include <QDebug>


Monomial operator*(const Monomial& a, const Term& b) {
    Monomial res(a);
    res.terms_.push_back(b);
    return res;
}


Monomial operator*(const Term& a, const Monomial& b) {
    return b * a;
}


Monomial operator*(const Monomial& a, const Monomial& b) {
    Monomial res(a);
    res.scalar_ *= b.scalar_;
    for (const Term& el: b.terms_) {
        res *= el;
    }
    return res;
}


bool operator^(const Monomial& a, const Monomial& b) {
    if (a.terms_.size() != b.terms_.size()) return false;
    for (int i = 0; i < a.terms_.size(); ++i) {
        if (a.terms_[i] != b.terms_[i]) {
            return false;
        }
    }
    return true;
}

long double Term::operator()(long double x) const {
    switch (type_) {
    case VAR:
        if (power_ < 0) {
            for (long double as: asimptots) {
                if(abs(x - as) < Settings::instance().EPS) {
                    throw std::runtime_error("Function not exist");
                }
            }
        }
        return std::pow(x, power_);
    case FUNCTION:
        if (power_ < 0) {
            for (long double as: asimptots) {
                if(abs(x - as) < Settings::instance().EPS) {
                    throw std::runtime_error("Function not exist");
                }
            }
        }
        return std::pow(func_->operator()(x), power_);
    default:
        break;
    }
    return 1;
}

std::vector<long double>& operator+=(std::vector<long double>& a, const std::vector<long double>& b);


long double calc_root(const Function&f, long double a, long double b) {
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
    std::cout << a << std::endl;
    return a;
}

std::vector<long double> find_root(const Function& f, long double a, long double b) {
    if (f.isSimple()) return {};
    std::vector<long double> bounds = {a};
    if (!f.isMono()) {
        bounds += f.potential_asimptot();
        bounds += find_root(+f, a, b);
        bounds += find_root(+(+f), a, b);
    }
    bounds += {b};
    sort(bounds.begin(), bounds.end());
    std::vector<long double> roots;
    for (long double el: bounds) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
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
            if (f(bounds[i-1] + Settings::instance().EPS) * f(bounds[i] - Settings::instance().EPS) > 0) continue;

        } catch (...) {}


        roots.push_back(calc_root(f, bounds[i-1] + Settings::instance().EPS, bounds[i] - Settings::instance().EPS));
    }
    return roots;
}



Term::Term(long double power, Function* func) : power_(power), func_(func) {
    type_ = (func_ ? FUNCTION : VAR);
    if (type_ == FUNCTION) {
        asimptots = find_root(*func_, Settings::instance().LHS, Settings::instance().RHS);
    }
}

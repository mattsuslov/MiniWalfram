#ifndef FUNCTION_H
#define FUNCTION_H


#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <stack>
#include <iostream>
#include "settings.h"


struct Term;
class Monomial;
class Function;


struct Term {
    Term(long double power, Function* func = nullptr);


    long double operator()(long double x) const;

    friend bool operator<(const Term& a, const Term& b) {
        if (a.type_ == b.type_) {
            return a.power_ > b.power_;
        }
        return a.type_ < b.type_;
    }
    friend bool operator>=(const Term& a, const Term& b) {
        return !(a < b);
    }
    friend bool operator>(const Term& a, const Term& b) {
        return b.type_ < a.type_;
    }
    friend bool operator<=(const Term& a, const Term& b) {
        return !(a > b);
    }
    friend bool operator==(const Term& a, const Term& b) {
        return a.type_ == b.type_ && a.power_ == b.power_;
    }
    friend bool operator!=(const Term& a, const Term& b) {
        return !(a == b);
    }

    std::string get_text() const {
        std::string text_format = "x";
        if (power_ != 1) {
            text_format += "^" + std::to_string(power_);
        }
        return text_format;
    }
    long double power_;

    enum Types {
        VAR,
        FUNCTION,
        LOG
    };
    Types type_;
    Function* func_;
    std::vector<long double> asimptots;
};

class Monomial
{
public:
    Monomial() = default;
    Monomial(long double scalar) : scalar_(scalar) {};
    Monomial(const Monomial& mon) {
        terms_ = mon.terms_;
        scalar_ = mon.scalar_;
    }

    std::vector<long double> potential_asimptot() const {
        if (terms_.size() == 0) return {};
        return terms_[0].asimptots;
    }

    bool isSimple() const {
        return terms_.size() == 0;
    }

    bool isMono() const {
        return terms_.size() == 0 || terms_[0].power_ < 0 || abs(terms_[0].power_ - (int)(terms_[0].power_)) > Settings::instance().EPS;
    }



    Monomial& operator=(Monomial other) {
        std::swap(other.terms_, terms_);
        std::swap(scalar_, other.scalar_);
        return *this;
    }
    Monomial& operator*=(const Term& other) {
        *this = *this * other;
        return *this;
    }
    Monomial& operator*=(const Monomial& other) {
        *this = *this * other;
        return *this;
    }

    friend Monomial operator+(const Monomial& a, const Monomial& b) {
        Monomial res = a;
        res.scalar_ += b.scalar_;
        return res;
    }
    friend Monomial operator*(const Monomial&, const Monomial&);
    friend Monomial operator*(const Monomial&, const Term&);
    friend Monomial operator*(const Term&, const Monomial&);
    friend bool operator^(const Monomial&, const Monomial&);

    Monomial operator+() const {
        if (terms_.empty()) return Monomial(0);
        Monomial res = *this;
        res.scalar_ *= res.terms_[0].power_;
        res.terms_[0].power_ -= 1;
        if (res.terms_[0].power_ == 0) {
            res.terms_.erase(res.terms_.begin());
        }
        return res;
    }
    Monomial operator-() const {
        Monomial res = *this;
        res.scalar_ = -res.scalar_;
        return res;
    }
    long double operator()(long double x) const {
        long double ans = scalar_;
        for (const Term& t: terms_) {
            ans *= t(x);
        }
        return ans;
    }


    friend bool operator==(const Monomial& a, long double b) {
        return abs(a.scalar_ - b) <= Settings::instance().EPSI;
    }


    friend std::ostream& operator<<(std::ostream& ofs, const Monomial& m) {
        if (m.scalar_ >= 0) ofs << "+";
        ofs << m.scalar_;
        for (const Term& el: m.terms_) {
            ofs << el.get_text();
        }
        return ofs;
    }

private:
    long double scalar_ = 0;
    std::vector<Term> terms_;
};


class Function {
public:
    Function() = default;
    Function(const Function&) = default;
    Function& operator=(Function other) {
        std::swap(polin_, other.polin_);
        return *this;
    }
    Function(Function&& f) {
        polin_ = std::move(f.polin_);
    }

    Function(const Monomial& m) { polin_.push_back(m); }

    std::vector<long double> potential_asimptot() const {
        std::vector<long double> res;
        for (const Monomial& el: polin_) {
            std::vector<long double> tmp = el.potential_asimptot();
            for (long double x: tmp) {
                res.push_back(x);
            }
        }
        return res;
    }

    bool isSimple() const {
        if (polin_.size() == 0) return true;
        if (polin_.size() == 1 && polin_[0].isSimple()) return true;
        return false;
    }

    bool isMono() const {
        if (polin_.size() == 0) return true;
        if (polin_.size() == 1 && polin_[0].isMono()) return true;
        return false;
    }

    long double operator()(long double x) const {
        long double ans = 0;
        for (const Monomial& el: polin_) {
            ans += el(x);
        }
        return ans;
    }
    friend Function operator+(const Function& a, const Monomial& b) {
        Function res = a;
        res.polin_.push_back(b);
        res.compress();
        return res;
    }
    friend Function operator+(const Monomial& a, const Function& b) {
        return b + a;
    }
    Function& operator+=(const Monomial& mon) {
        *this = *this + mon;
        return *this;
    }
    friend Function operator+(const Function& a, const Function& b) {
        Function res = a;
        for (const Monomial& el: b.polin_) {
            res += el;
        }
        res.compress();
        return res;
    }
    Function& operator+=(const Function& f) {
        *this = *this + f;
        return *this;
    }
    friend Function operator-(const Function& a, const Monomial& b) {
        return a + -b;
    }
    friend Function operator-(const Monomial& a, const Function& b) {
        return a + -b;
    }
    Function& operator-=(const Monomial& mon) {
        *this = *this - mon;
        return *this;
    }
    friend Function operator-(const Function& a, const Function& b) {
        Function res = a;
        for (const Monomial& el: b.polin_) {
            res -= el;
        }
        res.compress();
        return res;
    }
    Function& operator-=(const Function& f) {
        *this = *this - f;
        return *this;
    }
    Function operator+() const {
        Function res;
        for (int i = 0; i < polin_.size(); ++i) {
            res += +polin_[i];
        }
        return res;
    }
    Function operator-() const {
        Function res = *this;
        for (Monomial& el: res.polin_) {
            el = -el;
        }
        return res;
    }
    friend std::ostream& operator<<(std::ostream& ofs, const Function& func) {
        for (const Monomial& mon: func.polin_) {
            ofs << mon;
        }
        return ofs;
    }

    friend bool operator==(const Function& a, const Function& b) {
        Function res = a - b;
        res.compress();
        return res.polin_.empty();
    }

private:
    std::vector<Monomial> polin_;
    void compress() {
        for (int i = 0; i < polin_.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (polin_[i] ^ polin_[j]) { // equals signature
                    polin_.push_back(polin_[i] + polin_[j]);
                    polin_.erase(polin_.begin() + i);
                    polin_.erase(polin_.begin() + j);
                    --j;
                    --i;
                }
            }
        }
        for (int i = 0; i < polin_.size(); ++i) {
            if (polin_[i] == 0) {
                polin_.erase(polin_.begin() + i);
                --i;
            }
        }
    }
};

#endif

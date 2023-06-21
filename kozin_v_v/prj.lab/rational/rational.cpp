//
// Created by kozin on 21.02.2023.
//
#include "rational/rational.hpp"
#include <sstream>
#include <cstdint>
#include <stdexcept>

//// Greatest common division
//int32_t GCD(int32_t a, int32_t b) {
//    a = a > 0 ? a : -a;
//    b = b > 0 ? b : -b;
//    while (a && b) {
//        if (a > b) {
//            a %= b;
//        } else {
//            b %= a;
//        }
//    }
//    return a + b;
//}
//Rational::Rational(const int32_t p, const int32_t q) {
//    if (q == 0) throw std::invalid_argument("division by zero");
//    int32_t d = GCD(p, q);
//    denum_ = q/d;
//    num_ = p/d;
//    if (denum_ < 0) {
//        denum_ = -denum_;
//        num_ = -num_;
//    }
//}
//Rational::Rational(const int32_t p) noexcept {
//    denum_ = 1;
//    num_ = p;
//}
//// Input/output
//std::ostream& operator<<(std::ostream& ostrm, const Rational& rhs) {
//    return rhs.writeTo(ostrm);
//}
//std::ostream& Rational::writeTo(std::ostream& ostrm) const noexcept {
//    ostrm << num_ << separator << denum_;
//    return ostrm;
//}
//std::istream& operator>>(std::istream& istrm, Rational& rhs) {
//    return rhs.readFrom(istrm);
//}
//std::istream& Rational::readFrom(std::istream& istrm) noexcept {
//    int32_t p(0);
//    int32_t q(1);
//    char divide;
//    char test;
//    istrm >> p;
//    test = istrm.peek();
//    if (test != '/') istrm.setstate(std::ios_base::failbit);
//    istrm >> divide;
//    test = istrm.peek();
//    if (test == ' ') istrm.setstate(std::ios_base::failbit);
//    istrm >> q;
//    if (!istrm.bad() && !istrm.fail() && (q >= 0)) {
//        *this = Rational(p, q);
//    } else {
//        istrm.setstate(std::ios_base::failbit);
//    }
//    return istrm;
//}
//// Algebra
//Rational operator+(const Rational &lhs, const Rational &rhs) {
//    Rational x = lhs;
//    return Rational(x += rhs);
//}
//Rational operator-(const Rational &lhs, const Rational &rhs) {
//    Rational x = lhs;
//    return Rational(x -= rhs);
//}
//Rational operator/(const Rational &lhs, const Rational &rhs) {
//    Rational x = lhs;
//    return Rational(x /= rhs);
//}
//
//Rational operator*(const Rational &lhs, const Rational &rhs) {
//    Rational x = lhs;
//    return Rational(x *= rhs);
//}
//
//Rational& Rational::operator+=(const Rational &rhs) {
//    *this = Rational(num_*rhs.denum_ + rhs.num_ * denum_, denum_ * rhs.denum_);
//    return *this;
//}
//
//Rational& Rational::operator-=(const Rational &rhs) {
//    *this = Rational(num_*rhs.denum_ - rhs.num_ * denum_, denum_ * rhs.denum_);
//    return *this;
//}
//
//Rational& Rational::operator/=(const Rational &rhs) {
//    *this = Rational(num_ * rhs.denum_, denum_*rhs.num_);
//    return *this;
//}
//
//Rational& Rational::operator*=(const Rational &rhs) {
//    *this = Rational(num_*rhs.num_, denum_ * rhs.denum_);
//    return *this;
//}
//
//Rational& Rational::operator+=(const int32_t &rhs) {
//    *this += Rational(rhs);
//    return *this;
//}
//
//Rational& Rational::operator-=(const int32_t &rhs) {
//    *this -= Rational(rhs);
//    return *this;
//}
//
//Rational& Rational::operator/=(const int32_t &rhs) {
//    *this /= Rational(rhs);
//    return *this;
//}
//
//Rational& Rational::operator*=(const int32_t &rhs) {
//    *this *= Rational(rhs);
//    return *this;
//}
//
//Rational& Rational::operator++() {
//    *this += 1;
//    return *this;
//}
//
//Rational& Rational::operator--() {
//    *this -= 1;
//    return *this;
//}
//Rational Rational::operator++(int) {
//    Rational old = *this;
//    *this += 1;
//    return old;
//}
//
//Rational Rational::operator--(int) {
//    Rational old = *this;
//    *this -= 1;
//    return old;
//}
//
//Rational Rational::operator-() {
//    num_ = -num_;
//    return *this;
//}
//// Compare
//bool Rational::operator==(const Rational &rhs) const noexcept {
//    return ((num_ == rhs.num_) && (denum_ == rhs.denum_));
//}
//bool Rational::operator!=(const Rational &rhs) const noexcept {
//    return !operator==(rhs);
//}
//bool Rational::operator>(const Rational &rhs) const noexcept {
//    return ((num_ * rhs.denum_ - rhs.num_ * denum_) > 0);
//}
//bool Rational::operator<(const Rational &rhs) const noexcept {
//    return rhs.operator>(*this);
//}
//bool Rational::operator<=(const Rational &rhs) const noexcept {
//    return !(*this).operator>(rhs);
//}
//bool Rational::operator>=(const Rational &rhs) const  noexcept {
//    return !rhs.operator>(*this);
//}
//bool operator==(const Rational &lhs, const int32_t &rhs) {
//    return lhs == Rational(rhs);
//}
//bool operator!=(const Rational &lhs, const int32_t &rhs) {
//    return lhs != Rational(rhs);
//}
//bool operator>(const Rational &lhs, const int32_t &rhs) {
//    return lhs > Rational(rhs);
//}
//bool operator<(const Rational &lhs, const int32_t &rhs) {
//    return lhs < Rational(rhs);
//}
//bool operator<=(const Rational &lhs, const int32_t &rhs) {
//    return lhs <= Rational(rhs);
//}
//bool operator>=(const Rational &lhs, const int32_t &rhs) {
//    return lhs >= Rational(rhs);
//}


Rational::Rational(const int32_t num) noexcept {
    num_ = num;
    denom_ = 1;
}

Rational::Rational(const int32_t num, const int32_t denom) {
    if(denom == 0){throw std::invalid_argument("Division by zero");}
    num_ = num;
    denom_ = denom;
    reduce();
}


void Rational::reduce() {
    int gcd = gcd_(num_, denom_);
    num_ /= gcd;
    denom_ /= gcd;
    if (denom_ < 0) {
        num_ = -num_;
        denom_ = -denom_;
    }
}

// метод для вычисления наибольшего общего делителя
int Rational::gcd_(int a, int b) const noexcept {
    while (b != 0) {
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}


Rational Rational::operator+=(const Rational &rhs) {
    num_ = num_ * rhs.getDenominator() + rhs.getNumerator() * denom_;
    denom_ *= rhs.getDenominator();
    reduce();
    return *this;
}

Rational Rational::operator-=(const Rational &rhs) {
    num_ = num_ * rhs.getDenominator() - rhs.getNumerator() * denom_;
    denom_ *= rhs.getDenominator();
    reduce();
    return *this;
}

Rational Rational::operator*=(const Rational &rhs) {
    num_ *= rhs.getNumerator();
    denom_ *= rhs.getDenominator();
    reduce();
    return *this;
}

Rational Rational::operator/=(const Rational &rhs) {
    num_ *= rhs.getDenominator();
    denom_ *= rhs.getNumerator();
    reduce();
    return *this;
}

Rational Rational::operator+=(const int32_t &rhs) {

    return *this += Rational(rhs);
}

Rational Rational::operator-=(const int32_t &rhs) {
    return *this -= Rational(rhs);
}

Rational Rational::operator/=(const int32_t &rhs) {
    return *this /= Rational(rhs);
}

Rational Rational::operator*=(const int32_t &rhs) {
    return *this *= Rational(rhs);
}


Rational Rational::operator++() {
    num_ += denom_;
    return *this;
};

Rational Rational::operator--() {
    num_ -= denom_;
    return *this;
};


Rational operator+(const Rational &lhs, const Rational &rhs) {
    return Rational(
            lhs.getNumerator() * rhs.getDenominator() + lhs.getDenominator() * rhs.getNumerator(),
            lhs.getDenominator() * rhs.getDenominator()
    );
}

Rational operator-(const Rational &lhs, const Rational &rhs) {
    return Rational(
            lhs.getNumerator() * rhs.getDenominator() - lhs.getDenominator() * rhs.getNumerator(),
            lhs.getDenominator() * rhs.getDenominator()
    );
}

Rational operator*(const Rational &lhs, const Rational &rhs) {
    return Rational(
            lhs.getNumerator() * rhs.getNumerator(),
            lhs.getDenominator() * rhs.getDenominator()
    );
}

Rational operator/(const Rational &lhs, const Rational &rhs) {
    return Rational(
            lhs.getNumerator() * rhs.getDenominator(),
            lhs.getDenominator() * rhs.getNumerator()
    );
}

Rational operator+(const Rational &lhs, const int32_t &rhs) {
    return Rational(lhs + Rational(rhs));
}

Rational operator-(const Rational &lhs, const int32_t &rhs) {
    return Rational(lhs - Rational(rhs));
}
Rational operator-(const Rational &lhs){
    return lhs * (-1);
}

Rational operator*(const Rational &lhs, const int32_t &rhs) {
    return Rational(lhs * Rational(rhs));
}

Rational operator/(const Rational &lhs, const int32_t &rhs) {
    return Rational(lhs / Rational(rhs));
}

Rational Rational::operator++(int) {
    Rational old = *this;
    *this += 1;
    return old;
}

Rational Rational::operator--(int) {
    Rational old = *this;
    *this -= 1;
    return old;
}



std::ostream &Rational::WriteTo(std::ostream &ostrm) const noexcept {
    ostrm << num_ << separator << denom_;
    return ostrm;
}

std::istream &Rational::ReadFrom(std::istream &istrm) noexcept {
    int numerator = 0;
    int denominator = 1;
    char slash = '#';
    istrm >> numerator;
    if (istrm.peek() != '/') {
        istrm.setstate(std::ios_base::failbit);
    }
    istrm >> slash;
    if (istrm.peek() == ' '){
        istrm.setstate(std::ios_base::failbit);
    }
    istrm >> denominator;
    if (!istrm.bad() and !istrm.fail() and (denominator >= 0)) {
        *this = Rational(numerator, denominator);
    } else {
        istrm.setstate(std::ios_base::failbit);
    }
    return istrm;
}


std::ostream &operator<<(std::ostream &ostrm, const Rational &rhs) {
    return rhs.WriteTo(ostrm);
}

std::istream &operator>>(std::istream &istrm, Rational &rhs) {
    return rhs.ReadFrom(istrm);
}

bool operator==(const Rational &lhs, const Rational &rhs) {
    return (lhs.getNumerator() == rhs.getNumerator()) and (rhs.getDenominator() == lhs.getDenominator());
}

bool operator!=(const Rational &lhs, const Rational &rhs) {
    return !(lhs == rhs);
}

bool operator<(const Rational &lhs, const Rational &rhs) {
    return (lhs.getNumerator() * rhs.getDenominator()) < (lhs.getDenominator() * rhs.getNumerator());
}

bool operator>(const Rational &lhs, const Rational &rhs) {
    return rhs < lhs;
}

bool operator<=(const Rational &lhs, const Rational &rhs) {
    return !(rhs < lhs);
}

bool operator>=(const Rational &lhs, const Rational &rhs) {
    return !(lhs < rhs);
}
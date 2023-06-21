//
// Created by kozin on 21.02.2023.
//

#ifndef C___CODEFORSES_RATIONAL_H
#define C___CODEFORSES_RATIONAL_H

#include <iostream>
//int32_t GCD(int32_t a, int32_t b);
//class Rational {
//public:
//    // Constructs
//    Rational() = default;
//    Rational(const Rational&) = default;
//    Rational(const int32_t p, const int32_t q);
//    explicit Rational(const int32_t p) noexcept;
//    ~Rational() = default;
//    // Algebra declaration
//    Rational operator-();
//    Rational& operator+=(const Rational& rhs);
//    Rational& operator-=(const Rational& rhs);
//    Rational& operator/=(const Rational& rhs);
//    Rational& operator*=(const Rational& rhs);
//    Rational& operator+=(const int32_t & rhs);
//    Rational& operator-=(const int32_t & rhs);
//    Rational& operator/=(const int32_t & rhs);
//    Rational& operator*=(const int32_t & rhs);
//    Rational& operator++();
//    Rational operator++(int);
//    Rational& operator--();
//    Rational operator--(int);
//    // Compare
//    bool operator==(const Rational& rhs) const noexcept;
//    bool operator>(const Rational& rhs) const noexcept;
//    bool operator!=(const Rational& rhs) const noexcept;
//    bool operator<(const Rational &rhs) const noexcept;
//    bool operator<=(const Rational &rhs) const noexcept;
//    bool operator>=(const Rational &rhs) const noexcept;
//    // Input/ouput
//    std::ostream& writeTo(std::ostream& ostrm) const noexcept;
//    std::istream& readFrom(std::istream& istrm) noexcept;
//
//private:
//    int32_t num_{0};
//    int32_t denum_{1};
//    static const char separator{'/'};
//};
//
//// Input/output
//std::ostream& operator<<(std::ostream& ostrm, const Rational& rhs);
//std::istream& operator>>(std::istream& istrm, Rational& rhs);
//// Extra algebra
//Rational operator+(const Rational& lhs, const Rational& rhs);
//Rational operator-(const Rational& lhs, const Rational& rhs);
//Rational operator/(const Rational& lhs, const Rational& rhs);
//Rational operator*(const Rational& lhs, const Rational& rhs);
//// Compare with int
//bool operator==(const Rational &lhs, const int32_t &rhs);
//bool operator!=(const Rational &lhs, const int32_t &rhs);
//bool operator>(const Rational &lhs, const int32_t &rhs);
//bool operator<(const Rational &lhs, const int32_t &rhs);
//bool operator<=(const Rational &lhs, const int32_t &rhs);
//bool operator>=(const Rational &lhs, const int32_t &rhs);

class Rational {
public:

    Rational() = default;

    explicit Rational(const int32_t num) noexcept;

    Rational(const int32_t num, const int32_t denom);

    ~Rational() = default;

    int32_t getNumerator() const {
        return num_;
    }

    int32_t getDenominator() const {
        return denom_;
    }

    Rational operator+=(const Rational &rhs);

    Rational operator-=(const Rational &rhs);

    Rational operator*=(const Rational &rhs);

    Rational operator/=(const Rational &rhs);

    Rational operator+=(const int32_t &rhs);

    Rational operator-=(const int32_t &rhs);

    Rational operator*=(const int32_t &rhs);

    Rational operator/=(const int32_t &rhs);

    Rational operator++();

    Rational operator--();

    Rational operator++(int);
    Rational operator--(int);

    std::ostream &WriteTo(std::ostream &ostrm) const noexcept;

    std::istream &ReadFrom(std::istream &istrm) noexcept;


private:
    int32_t num_{0};
    int32_t denom_{1};
    static const char separator{'/'};

    // метод для сокращения дроби
    void reduce();

    int gcd_(int a, int b) const noexcept;
};

Rational operator+(const Rational &lhs, const Rational &rhs);

Rational operator-(const Rational &lhs, const Rational &rhs);

Rational operator*(const Rational &lhs, const Rational &rhs);

Rational operator/(const Rational &lhs, const Rational &rhs);

Rational operator+(const Rational &lhs, const int32_t &rhs);

Rational operator-(const Rational &lhs, const int32_t &rhs);

Rational operator-(const Rational &lhs);

Rational operator*(const Rational &lhs, const int32_t &rhs);

Rational operator/(const Rational &lhs, const int32_t &rhs);

std::ostream &operator<<(std::ostream &ostrm, const Rational &rhs);

std::istream &operator>>(std::istream &istrm, Rational &rhs);

bool operator==(const Rational &lhs, const Rational &rhs);

bool operator!=(const Rational &lhs, const Rational &rhs);

bool operator<(const Rational &lhs, const Rational &rhs);

bool operator>(const Rational &lhs, const Rational &rhs);

bool operator<=(const Rational &lhs, const Rational &rhs);

bool operator>=(const Rational &lhs, const Rational &rhs);

#endif //C___CODEFORSES_RATIONAL_H

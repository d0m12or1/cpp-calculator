#pragma once

#include <iostream>
#include <numeric>
#include <compare>
#include <cstdlib>
#include <sstream>

class Rational {
public:
    Rational() : numerator_(0), denominator_(1) {}
    Rational(int numerator) : numerator_(numerator), denominator_(1) {}
    Rational(int numerator, int denominator)
        : numerator_(numerator), denominator_(denominator)
    {
        if (denominator_ == 0)
        {
            std::abort();
        }
        Reduction();
    }
    Rational(const Rational& other) = default;

    int GetNumerator() const { return numerator_; }
    int GetDenominator() const { return denominator_; }

    Rational Inv() const
    {
        if (numerator_ == 0)
        {
            std::abort();
        }
        return Rational(denominator_, numerator_);
    }

    Rational& operator+=(const Rational& rhs)
    {
        numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator-=(const Rational& rhs)
    {
        numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator*=(const Rational& rhs)
    {
        numerator_ *= rhs.numerator_;
        denominator_ *= rhs.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator/=(const Rational& rhs)
    {
        if (rhs.numerator_ == 0) std::abort();
        numerator_ *= rhs.denominator_;
        denominator_ *= rhs.numerator_;
        Reduction();
        return *this;
    }

    Rational operator+() const
    {
        return *this;
    }
    Rational operator-() const
    {
        return Rational(-numerator_, denominator_);
    }

    Rational operator+(const Rational& rhs) const
    {
        Rational r(*this); r += rhs; return r;
    }

    Rational operator-(const Rational& rhs) const
    {
        Rational r(*this); r -= rhs; return r;
    }

    Rational operator*(const Rational& rhs) const
    {
        Rational r(*this); r *= rhs; return r;
    }

    Rational operator/(const Rational& rhs) const
    {
        Rational r(*this); r /= rhs; return r;
    }

    auto operator<=>(const Rational& rhs) const
    {
        return (numerator_ * rhs.denominator_) <=> (rhs.numerator_ * denominator_);
    }
    bool operator==(const Rational& rhs) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Rational& r)
    {
        if (r.denominator_ == 1)
        {
            os << r.numerator_;
        }
        else
        {
            os << r.numerator_ << " / " << r.denominator_;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Rational& r)
    {
        int num = 0, denom = 1;
        char sep = '\0';
        is >> num;
        if (is.peek() == ' ' || is.peek() == '/')
        {
            is >> sep;
            if (sep == '/')
            {
                is >> denom;
            }
        }
        if (denom == 0)
        {
            is.setstate(std::ios::failbit);
            return is;
        }
        r = Rational(num, denom);
        return is;
    }

private:
    int numerator_;
    int denominator_;

    void Reduction()
    {
        if (denominator_ < 0)
        {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }
};

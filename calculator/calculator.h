#pragma once
#include <optional>
#include <string>
#include <cmath>
#include <type_traits>
#include "rational.h"
#include "pow.h"

using Error = std::string;

template<typename Number>
class Calculator
{
public:
    Calculator() = default;

    std::optional<Error> Add(const Number& a, const Number& b, Number& result)
    {
        result = a + b;
        return std::nullopt;
    }

    std::optional<Error> Sub(const Number& a, const Number& b, Number& result)
    {
        result = a - b;
        return std::nullopt;
    }

    std::optional<Error> Mul(const Number& a, const Number& b, Number& result)
    {
        result = a * b;
        return std::nullopt;
    }

    std::optional<Error> Div(const Number& a, const Number& b, Number& result)
    {
        if constexpr (std::is_integral_v<Number> || std::is_same_v<Number, Rational>)
        {
            if (b == Number(0))
                return "Division by zero";
        }
        result = a / b;
        return std::nullopt;
    }

    std::optional<Error> Pow(const Number& base, const Number& exponent, Number& result)
    {
        if constexpr (std::is_floating_point_v<Number>)
        {
            result = std::pow(base, exponent);
            return std::nullopt;
        }
        else if constexpr (std::is_integral_v<Number>)
        {
            if (base == Number(0) && exponent == Number(0))
                return "Zero power to zero";
            if (exponent < 0)
                return "Integer negative power";
            result = IntegerPow(base, exponent);
            return std::nullopt;
        }
        else if constexpr (std::is_same_v<Number, Rational>)
        {
            if (base == Rational(0,1) && exponent == Rational(0,1))
                return "Zero power to zero";
            if (exponent.GetDenominator() != 1)
                return "Fractional power is not supported";

            result = ::Pow(base, exponent);
            return std::nullopt;
        }
        else
        {
            static_assert(always_false<Number>::value, "Unsupported number type");
        }
    }

    std::optional<Error> Add(const Number& rhs)
    {
        return Add(current_, rhs, current_);
    }

    std::optional<Error> Sub(const Number& rhs)
    {
        return Sub(current_, rhs, current_);
    }

    std::optional<Error> Mul(const Number& rhs)
    {
        return Mul(current_, rhs, current_);
    }

    std::optional<Error> Div(const Number& rhs)
    {
        return Div(current_, rhs, current_);
    }

    std::optional<Error> Pow(const Number& rhs)
    {
        return Pow(current_, rhs, current_);
    }

    void SetMem(const Number& value)
    {
        mem_ = value;
    }

    std::optional<Number> GetMem() const
    {
        return mem_;
    }

    void ClearMem()
    {
        mem_.reset();
    }

    bool GetHasMem() const {
        return mem_.has_value();
    }

    void Save() {
        SetMem(current_);
    }

    void Load() {
        if (mem_) {
            current_ = *mem_;
        }
    }


    void Set(const Number& value)
    {
        current_ = value;
    }

    Number GetNumber() const
    {
        return current_;
    }

private:
    std::optional<Number> mem_;
    Number current_{};

    template<class T> struct always_false : std::false_type {};
};

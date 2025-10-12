#include "calculator.h"
#include <cmath>

Calculator::Calculator() : current_(0) {}

void Calculator::Reset() {
    current_ = 0;
}

void Calculator::SetValue(Number value) {
    current_ = value;
}

Number Calculator::GetValue() const {
    return current_;
}

bool Calculator::ApplyOperation(const QString& operation, Number operand) {
    if (operation == "+") current_ += operand;
    else if (operation == "-") current_ -= operand;
    else if (operation == "*") current_ *= operand;
    else if (operation == "/") current_ /= operand;
    else if (operation == "**") current_ = std::pow(current_, operand);
    else return false;
    return true;
}

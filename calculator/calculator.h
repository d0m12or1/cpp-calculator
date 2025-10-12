#pragma once
#include <QString>

using Number = double;

class Calculator {
public:
    Calculator();

    void Reset();
    void SetValue(Number value);
    Number GetValue() const;

    bool ApplyOperation(const QString& operation, Number operand);

private:
    Number current_ = 0;
};

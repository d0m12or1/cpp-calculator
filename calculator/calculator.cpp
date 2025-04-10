#include "calculator.h"
#include <iostream>
#include <cmath>

namespace {
    Number memory = 0;
    bool memory_init = false;
}

bool ReadNumber (Number& result) {
    if (!(std::cin >> result)) {
        std::cerr << "Error: Numeric operand expected" << std::endl;
        return false;
    }
    return true;
}

bool RunCalculatorCycle() {
    Number current;
    if (!ReadNumber(current)) {
        return false;
    }

    std::string operation;
    while (std::cin >> operation) {
        if (operation == "=") {
            std::cout << current << std::endl;
            continue;
        } else if (operation == "q") {
            return true;
        } else if (operation == "c") {
            current = 0;
            continue;
        } else if (operation == "s") {
            memory = current;
            memory_init = true;
            continue;
        }else if (operation == "l") {
            if (!memory_init) {
                std::cerr << "Error: Memory is empty" << std::endl;
                return false;
            }
            current = memory;
            continue;
        }
        Number operand;
        if (!ReadNumber(operand)) {
            return false;
        }

        if (operation == "+") {
            current += operand;
        } else if (operation == "*") {
            current *= operand;
        } else if (operation == "-") {
            current -= operand;
        } else if (operation == "/") {
            current /= operand;
        } else if (operation == "**") {
            current = std::pow(current, operand);
        } else if (operation == ":") {
            current = operand;
        } else {
            std::cerr << "Error: Unknown token " << operation << std::endl;
            return false;
        }
    }
    return false;
}
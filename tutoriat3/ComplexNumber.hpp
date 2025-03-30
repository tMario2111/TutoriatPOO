#pragma once

#include <cmath>
#include <iostream>

class ComplexNumber
{
public:
    ComplexNumber();

    ComplexNumber(float a, float b);

    ComplexNumber(const ComplexNumber &other);

    ~ComplexNumber();

    ComplexNumber &operator=(const ComplexNumber &other);

    friend std::ostream &operator<<(std::ostream &os, const ComplexNumber &num);

    friend std::istream &operator>>(std::istream &is, ComplexNumber &num);

    float getA() const;

    float getB() const;

    void setA(float a);

    void setB(float b);

    ComplexNumber operator+(const ComplexNumber &other) const;

    ComplexNumber &operator+=(const ComplexNumber &other);

    ComplexNumber operator+(const float other) const;

    friend ComplexNumber operator+(const float a, const ComplexNumber &other);

    static float abs(const ComplexNumber &num);

private:
    float a, b;
};

#include "ComplexNumber.hpp"

// Constructor parametrizat
ComplexNumber::ComplexNumber() : a(0.f), b(0.f)
{
}

// Constructor neparametrizat
ComplexNumber::ComplexNumber(float a, float b) : a(a), b(b) // parametrizat
{
}

// Constructor de copiere
ComplexNumber::ComplexNumber(const ComplexNumber &other) : a(other.a), b(other.b)
{
}

ComplexNumber::~ComplexNumber()
{
}

ComplexNumber &ComplexNumber::operator=(const ComplexNumber &other)
{
    if (this != &other)
    {
        this->a = other.a;
        this->b = other.b;
    }
    return *this;
}

float ComplexNumber::getA() const
{
    return this->a;
}

float ComplexNumber::getB() const
{
    return this->b;
}

void ComplexNumber::setA(float a)
{
    this->a = a;
}

void ComplexNumber::setB(float b)
{
    this->b = b;
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber &other) const
{
    return ComplexNumber(this->a + other.a, this->b + other.b);
}

ComplexNumber &ComplexNumber::operator+=(const ComplexNumber &other)
{
    this->a += other.a;
    this->b += other.b;
    return *this;
}

ComplexNumber ComplexNumber::operator+(const float other) const
{
    return ComplexNumber(this->a + other, this->b);
}

float ComplexNumber::abs(const ComplexNumber &num)
{
    return sqrtf(num.a * num.a + num.b * num.b);
}

std::ostream &operator<<(std::ostream &os, const ComplexNumber &num)
{
    os << num.a << " + " << num.b << "i";
    return os;
}

std::istream &operator>>(std::istream &is, ComplexNumber &num)
{
    is >> num.a >> num.b;
    return is;
}

ComplexNumber operator+(const float a, const ComplexNumber &other)
{
    return ComplexNumber(a + other.a, other.b);
}

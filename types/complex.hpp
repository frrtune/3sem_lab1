#pragma once
#include <iostream>

class Complex {
    private:
        double real_;
        double imaginary_;
    public:
        Complex(double real = 0, double imaginary = 0) : real_(real), imaginary_(imaginary) {}
        double GetReal() const {
            return real_;
        }
        double GetImaginary() const {
            return imaginary_;
        }
        double GetSquaredModule() const {
            return real_ * real_ + imaginary_ * imaginary_;
        }
        Complex Conjugate() const {
            return Complex(real_, -imaginary_);
        }
        Complex operator+(const Complex& other) const {
            return Complex(real_ + other.real_, imaginary_ + other.imaginary_);
        }
        Complex operator-(const Complex& other) const {
            return Complex(real_ - other.real_, imaginary_ - other.imaginary_);
        }
        Complex operator*(const Complex& other) const {
            return Complex(real_ * other.real_ - imaginary_ * other.imaginary_, real_ * other.imaginary_ + imaginary_ * other.real_);
        }
        bool operator==(const Complex& other) const {
            return real_ == other.real_ && imaginary_ == other.imaginary_;
        }
        bool operator!=(const Complex& other) const {
            return !(*this == other);
        }
        bool operator<(const Complex& other) const {
            double squared_module = GetSquaredModule();
            double other_squared_module = other.GetSquaredModule();
            if (squared_module != other_squared_module) return squared_module < other_squared_module;
            if (real_ != other.real_) return real_ < other.real_;
            return imaginary_ < other.imaginary_;
        }
};

inline std::ostream& operator<<(std::ostream& out, const Complex& number) {
    out << number.GetReal();
    if (number.GetImaginary() >= 0) out << '+';
    return out << number.GetImaginary() << 'i';
}
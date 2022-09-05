#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <iostream>

class Complex
{
public:
    Complex() : real(0), imag(0){};
    Complex(int real) : real(real), imag(0){};
    Complex(int real, int imag) : real(real), imag(imag){};
    friend std::ostream &operator<<(std::ostream &os, const Complex &u) { return os << "(" << u.real << ", " << u.imag << ")"; }
    friend Complex operator+(const Complex &u, const Complex &v) { return Complex(u.real + v.real, u.imag + v.imag); }
    friend Complex operator-(const Complex &u, const Complex &v) { return Complex(u.real - v.real, u.imag - v.imag); }
    friend Complex operator*(const Complex &u, const Complex &v) { return Complex(u.real * v.real - u.imag * v.imag, u.real * v.imag + u.imag + v.imag); }

private:
    int real, imag;
};

#endif

/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

// TODO: implement this class for use with the mandlebrot medium

#include <feign/math/vector.h>

struct Complex2f
{
public:
    Complex2f() : real(0.0), imag(0.0) { }

    Complex2f(Float real)
        : real(real), imag(0.0) { }

    Complex2f(Float real, Float imag)
        : real(real), imag(imag) { }

    Complex2f(const Complex2f& other)
        : real(other.real),
          imag(other.imag) { }

    Complex2f operator+(Float r) const
    {
        return Complex2f(real + r,
                         imag);
    }

    Complex2f operator-(Float r) const
    {
        return Complex2f(real - r,
                         imag);
    }

    Complex2f operator+(const Complex2f& other) const
    {
        return Complex2f(real + other.real,
                         imag + other.imag);
    }

    Complex2f operator-(const Complex2f& other) const
    {
        return Complex2f(real - other.real,
                         imag - other.imag);
    }

    void operator+=(const Complex2f& other)
    {
        real += other.real;
        imag += other.imag;
    }

    void operator-=(const Complex2f& other)
    {
        real += other.real;
        imag += other.imag;
    }

    Complex2f operator*(const Complex2f& other) const
    {
        double r = real * other.real - imag * other.imag;
        double i = real * other.imag + imag * other.real;

        return Complex2f(r, i);
    }

    Float norm() const
    {
        return real * real + imag * imag;
    }

    Float real;
    Float imag;
};

struct Complex3f
{
    Complex3f() : real(0.0), imag(0.0), imag_2(0.0) { }

    Complex3f(Float real)
        : real(real), imag(0.0), imag_2(0.0) { }

    Complex3f(Float real, Float imag, Float imag_2)
        : real(real), imag(imag), imag_2(imag_2) { }

    Complex3f(const Complex3f& other)
        : real(other.real),
          imag(other.imag),
          imag_2(other.imag_2) { }

    Complex3f operator+(Float r) const
    {
        return Complex3f(real + r,
                         imag,
                         imag_2);
    }

    Complex3f operator-(Float r) const
    {
        return Complex3f(real - r,
                         imag,
                         imag_2);
    }

    Complex3f operator+(const Complex3f& other) const
    {
        return Complex3f(real + other.real,
                         imag + other.imag,
                         imag_2 + other.imag_2);
    }

    Complex3f operator-(const Complex3f& other) const
    {
        return Complex3f(real - other.real,
                         imag - other.imag,
                         imag_2 - other.imag_2);
    }

    void operator+=(const Complex3f& other)
    {
        real += other.real;
        imag += other.imag;
        imag_2 += other.imag_2;
    }

    void operator-=(const Complex3f& other)
    {
        real -= other.real;
        imag -= other.imag;
        imag_2 -= other.imag_2;
    }

    Complex3f operator*(const Complex3f& other) const
    {
        double r = real * other.real - imag * other.imag_2 - imag_2 * other.imag;
        double i = real * other.imag + imag * other.real - imag_2 * other.imag_2;
        double i2 = real * other.imag_2 + imag * other.imag + imag_2 * other.real;

        return Complex3f(r, i, i2);
    }

    Float norm() const
    {
        return real * real + imag * imag + imag_2 * imag_2;
    }

    Float real;
    Float imag;
    Float imag_2;
};

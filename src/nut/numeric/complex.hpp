/**
 * @file -
 * @author jingqi
 * @date 2015-01-19
 * @brief
 */

#ifndef ___HEADFILE_E2064CA1_204C_4CC2_A176_76A8931D6EE1_
#define ___HEADFILE_E2064CA1_204C_4CC2_A176_76A8931D6EE1_

namespace nut
{

/**
 * 复数
 */
class Complex
{
public:
    // 实数部分，虚数部分
    double real, image;

    Complex(double r = 0, double i = 0)
        : real(r), image(i)
    {}

    Complex(const Complex& x)
        : real(x.real), image(x.image)
    {}

    Complex& operator=(const Complex& x)
    {
        real = x.real;
        image = x.image;
        return *this;
    }

    bool operator==(const Complex& x) const
    {
        return real == x.real && image == x.image;
    }

    bool operator!=(const Complex& x) const
    {
        return !(*this == x);
    }

    Complex operator+(const Complex& x) const
    {
        return Complex(real + x.real, image + x.image);
    }

    Complex operator-(const Complex& x) const
    {
        return Complex(real - x.real, image - x.image);
    }

    Complex operator-() const
    {
        return Complex(-real, -image);
    }

    Complex operator*(const Complex& x) const
    {
        return Complex(real * x.real - image * x.image,
            real * x.image + image * x.real);
    }

    Complex& operator+=(const Complex& x)
    {
        real += x.real;
        image += x.image;
        return *this;
    }

    Complex& operator-=(const Complex& x)
    {
        real -= x.real;
        image -= x.image;
        return *this;
    }

    Complex& operator*=(const Complex& x)
    {
        const double r = real * x.real - image * x.image;
        const double i = real * x.image + image * x.real;
        real = r;
        image = i;
        return *this;
    }
};

}

#endif

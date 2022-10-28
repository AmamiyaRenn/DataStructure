#pragma once

#include <cmath>
#define Hypotenuse(x, y) sqrt(x* x + y * y)

struct Vector2d
{
    Vector2d() : x(0), y(0) {};
    explicit Vector2d(float x) : x(x), y(x) {};
    Vector2d(float x, float y) : x(x), y(y) {};
    // 括号的重载
    void operator()(float x)
    {
        this->x = this->y = x;
        update();
    };
    // 括号的重载
    void operator()(float x, float y)
    {
        this->x = x, this->y = y;
        update();
    };
    // 向量赋值
    Vector2d& operator=(const Vector2d& v)
    {
        this->x = v.x, this->y = v.y;
        update();
        return *this;
    }
    //  重载+号，定义为向量相加
    friend Vector2d operator+(const Vector2d& u, const Vector2d& v) { return Vector2d(u.x + v.x, u.y + v.y); }
    // 重载-号，定义为向量减法
    friend Vector2d operator-(const Vector2d& u, const Vector2d& v) { return Vector2d(u.x - v.x, u.y - v.y); }
    //    //  重载+号，定义为正
    //    friend Vector2d operator+(const Vector2d& u) { return *this; }
    // 重载-号，定义为负
    friend Vector2d operator-(const Vector2d& u) { return Vector2d(-u.x, -u.y); }
    // 重载*号
    Vector2d operator*(const float v) const { return Vector2d(x * v, y * v); }
    Vector2d operator*(const Vector2d v) const { return Vector2d(x * v.x, y * v.y); }
    // 自加
    Vector2d& operator+=(const Vector2d& v)
    {
        this->x += v.x, this->y += v.y;
        update();
        return *this;
    }
    // 自乘
    void operator*=(const Vector2d& v)
    {
        x *= v.x, y *= v.y;
        update();
    }
    // 二范数
    float norm() const { return (Hypotenuse(this->x, this->y)); }
    // 点积
    float dot(Vector2d& v) const { return x * v.x + y * v.y; }
    // 投影
    float projection(Vector2d& v) const { return this->dot(v) / this->norm(); }
    // 输出向量标准化情况
    Vector2d normlize() const { return Vector2d(x / this->norm(), y / this->norm()); }
    // 将向量标准化
    void normlized()
    {
        float n = norm();
        x /= n, y /= n;
        update();
    }
    bool operator==(const Vector2d& v) const { return (x == v.x) && (y == v.y); };
    bool operator!=(const Vector2d& v) const { return (x != v.x) && (y != v.y); };
    bool operator<(const Vector2d& v) const { return (x < v.x) && (y < v.y); };
    bool operator>(const Vector2d& v) const { return (x > v.x) && (y > v.y); }

    virtual void update() {};
    float        x;
    float        y;
};

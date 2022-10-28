#pragma once

#include "app/Vector2d.h"

struct Velocity2d : public Vector2d
{
    Velocity2d() : v(0), angle(0) {};
    explicit Velocity2d(const Vector2d& v) : Vector2d(v.x, v.y) { update(); };
    Velocity2d(float x, float y) : Vector2d(x, y) { update(); };
    void update() override
    {
        v = norm();
        angleUpdate();
    };
    void quadrantJudge();
    void angleUpdate() { angle = 0; };
    float angle; // 同坐标信息不同，angle=0的时候实际上指v的方向是y的正方向,且转动时顺负逆正
    float v;     // 速度
};

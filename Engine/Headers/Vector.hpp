#pragma once
#include "iostream"
#include <cmath>

#include "./game.h"

struct position{
    float x;
    float y;
};

class Vector2D{
public:
    float x;
    float y;

    bool inMotion;
    float moveX;
    float moveY;
    float time;
    float elapsed;
    float startX, startY;

    position speed = {0,0};

    Vector2D();
    Vector2D(float x, float y);

    Vector2D& Add(const Vector2D& vec);
    Vector2D& Subtract(const Vector2D& vec);
    Vector2D& Multiply(const Vector2D& vec);
    Vector2D& Divide(const Vector2D& vec);

    Vector2D& Update();

    void Bounce(const Vector2D &descVec, float strength, float time=1.0f);

    friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

    Vector2D& operator*(const int& i);
    Vector2D& Zero();

    Vector2D& Normalize();
    float Length() const;


    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};
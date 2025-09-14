#include "./Headers/Vector.hpp"

Vector2D::Vector2D(): inMotion(false) {
    x = 0.0f;
    y = 0.0f;
}


Vector2D::Vector2D(float x,float y): inMotion(false) {
    this->x = x;
    this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vec){
    this->x += vec.x;
    this->y += vec.y;

    return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D &vec) {
    this->x -= vec.x;
    this->y -= vec.y;

    return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D &vec) {
    this->x *= vec.x;
    this->y *= vec.y;

    return *this;
}

Vector2D& Vector2D::Divide(const Vector2D &vec) {
    this->x /= vec.x;
    this->y /= vec.y;

    return *this;
}

void Vector2D::Bounce(const Vector2D &descVec, float strength, float time) {
    this->inMotion = true;
    this->time = time;
    this->moveX = descVec.x*strength;
    this->moveY = descVec.y*strength;
    this->speed = {this->moveX, this->moveY};
    this->elapsed = 0;
}

Vector2D& Vector2D::Update() {
    if(!inMotion) return *this;

    elapsed += Game::deltaTime;
    float progress = std::min(elapsed/time, 1.0f);

    float factor = -(cos(M_PI * progress) - 1.0f) / 2.0f;

    float nextX = (speed.x / time) * factor;
    float nextY = (speed.y / time) * factor;

    this->x += nextX;
    this->y += nextY;
    moveX -= nextX;
    moveY -= nextY;

    if(progress >= 1.0f){
        inMotion = false;
        speed = {0,0};
    }

    return *this;
}


Vector2D& operator+(Vector2D& v1, const Vector2D& v2){
    return v1.Add(v2);
}
Vector2D& operator-(Vector2D& v1, const Vector2D& v2){
    return v1.Subtract(v2);
}
Vector2D& operator*(Vector2D& v1, const Vector2D& v2){
    return v1.Multiply(v2);
}
Vector2D& operator/(Vector2D& v1, const Vector2D& v2){
    return v1.Divide(v2);
}


Vector2D& Vector2D::operator+=(const Vector2D& vec){
    return this->Add(vec);
}
Vector2D& Vector2D::operator-=(const Vector2D& vec){
    return this->Subtract(vec);
}
Vector2D& Vector2D::operator*=(const Vector2D& vec){
    return this->Multiply(vec);
}
Vector2D& Vector2D::operator/=(const Vector2D& vec){
    return this->Divide(vec);
}

Vector2D& Vector2D::operator*(const int& i) {
    this->x *= i;
    this->y *= i;

    return *this;
}

Vector2D& Vector2D::Zero() {
    this->x = 0;
    this->y = 0;

    return *this;
}

float Vector2D::Length() const {
    return sqrt(x * x + y * y);
}

Vector2D& Vector2D::Normalize() {
    float length = Length();
    if(length != 0) {
        x /= length;
        y /= length;
    }
    return *this;
}


std::ostream& operator<<(std::ostream& stream, const Vector2D& vec){
    stream << "(x: " << vec.x << ", y: " << vec.y << ")";

    return stream;
}
#pragma once
#include <math.h>

#include <iostream>

namespace utils {
template <typename T>
class Vector2D {
   public:
    T x;
    T y;

    Vector2D(T x = static_cast<T>(0), T y = static_cast<T>(0)) : x(x), y(y) {}

    Vector2D<T> operator-() const;
    Vector2D<T> operator+(const Vector2D<T>& v) const;
    Vector2D<T> operator-(const Vector2D<T>& v) const;
    Vector2D<T> operator*(const Vector2D<T>& v) const;
    Vector2D<T> operator/(const Vector2D<T>& v) const;

    Vector2D<T> operator+(const T s) const;
    Vector2D<T> operator-(const T s) const;
    Vector2D<T> operator*(const T s) const;
    Vector2D<T> operator/(const T s) const;

    Vector2D<T>& operator+=(const Vector2D<T>& v);
    Vector2D<T>& operator-=(const Vector2D<T>& v);
    Vector2D<T>& operator*=(const Vector2D<T>& v);
    Vector2D<T>& operator/=(const Vector2D<T>& v);

    Vector2D<T>& operator+=(const T s);
    Vector2D<T>& operator-=(const T s);
    Vector2D<T>& operator*=(const T s);
    Vector2D<T>& operator/=(const T s);

    bool operator==(const Vector2D<T>& v) const;
    bool operator!=(const Vector2D<T>& v) const;

    Vector2D<float> GetNormalized(const float norm = 1.f) const;
    float GetNorm() const;
    T Dot(const Vector2D<T>& v) const;
};

template <typename T>
Vector2D<T> Vector2D<T>::operator-() const {
    return {-x, -y};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& v) const {
    return {x + v.x, y + v.y};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& v) const {
    return {x - v.x, y - v.y};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator*(const Vector2D<T>& v) const {
    return {x * v.x, y * v.y};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator/(const Vector2D<T>& v) const {
    return {x / v.x, y / v.y};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator+(const T s) const {
    return {x + s, y + s};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator-(const T s) const {
    return {x - s, y - s};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator*(const T s) const {
    return {x * s, y * s};
}

template <typename T>
Vector2D<T> Vector2D<T>::operator/(const T s) const {
    return {x / s, y / s};
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& v) {
    x += v.x;
    y += v.y;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator*=(const Vector2D<T>& v) {
    x *= v.x;
    y *= v.y;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator/=(const Vector2D<T>& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator+=(const T s) {
    x += s;
    y += s;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator-=(const T s) {
    x -= s;
    y -= s;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator*=(const T s) {
    x *= s;
    y *= s;
    return *this;
}

template <typename T>
Vector2D<T>& Vector2D<T>::operator/=(const T s) {
    x /= s;
    y /= s;
    return *this;
}

template <typename T>
bool Vector2D<T>::operator==(const Vector2D<T>& v) const {
    return x == v.x && y == v.y;
}

template <typename T>
bool Vector2D<T>::operator!=(const Vector2D<T>& v) const {
    return !(*this == v);
}

template <typename T>
Vector2D<float> Vector2D<T>::GetNormalized(const float norm) const {
    if (x == 0.f && y == 0.f) {
        return {0.f, 0.f};
    }
    float coef = norm / sqrtf(static_cast<float>(x * x) + static_cast<float>(y * y));
    return {static_cast<float>(x) * coef, static_cast<float>(y) * coef};
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2D<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

template <typename T>
float Vector2D<T>::GetNorm() const {
    return sqrtf(static_cast<float>(x * x + y * y));
}

template <typename T>
T Vector2D<T>::Dot(const Vector2D<T>& v) const {
    return x * v.x + y * v.y;
}

using Vec2Df = Vector2D<float>;
}  // namespace utils

#pragma once
#include <algorithm>
#include <array>

#include "Vector2D.h"

namespace utils {
template <typename T>
struct Rectangle {
    Vector2D<T> top_left_;
    T width_;
    T height_;

    Rectangle(const Vector2D<T>& top_left = {}, T width = static_cast<T>(0), T height = static_cast<T>(0))
        : top_left_(top_left), width_(width), height_(height) {}
    template <typename T2>
    operator Rectangle<T2>() const;
    /* x1 ---- x2
        |      |
       x4 ---- x3*/
    std::array<Vector2D<T>, 4> GetVertices() const;
    bool IsCovering(const Vector2D<T>& point) const;
    bool IsIntersecting(const Rectangle<T> rect) const;
    Vector2D<T> GetCenterPoint() const;
};

template <typename T>
template <typename T2>
Rectangle<T>::operator Rectangle<T2>() const {
    return Rectangle<T2>{{static_cast<T2>(top_left_.x), static_cast<T2>(top_left_.y)},
                         static_cast<T2>(width_),
                         static_cast<T2>(height_)};
}

template <typename T>
std::array<Vector2D<T>, 4> Rectangle<T>::GetVertices() const {
    return {Vector2D<T>(top_left_.x, top_left_.y), Vector2D<T>(top_left_.x + width_, top_left_.y),
            Vector2D<T>(top_left_.x + width_, top_left_.y + height_), Vector2D<T>(top_left_.x, top_left_.y + height_)};
}

template <typename T>
bool Rectangle<T>::IsCovering(const Vector2D<T>& point) const {
    return ((point.x >= top_left_.x && point.x <= top_left_.x + width_) &&
            (point.y >= top_left_.y && point.y <= top_left_.y + height_));
}

template <typename T>
bool Rectangle<T>::IsIntersecting(const Rectangle<T> rect) const {
    auto this_vertices = GetVertices();
    auto rect_vertices = rect.GetVertices();
    bool is_intersecting = false;
    std::for_each(this_vertices.begin(), this_vertices.end(),
                  [&is_intersecting, &rect](auto& v) { is_intersecting |= rect.IsCovering(v); });
    std::for_each(rect_vertices.begin(), rect_vertices.end(),
                  [&is_intersecting, this](auto& v) { is_intersecting |= this->IsCovering(v); });
    return is_intersecting;
}

template <typename T>
Vector2D<T> Rectangle<T>::GetCenterPoint() const {
    return {static_cast<T>(top_left_.x + width_ * 0.5), static_cast<T>(top_left_.y + height_ * 0.5)};
}
}  // namespace utils

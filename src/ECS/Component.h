#pragma once
#include <cstddef>

namespace ECS {
struct BaseComponent {
    using ComponentID = std::size_t;
    virtual ~BaseComponent() = default;

   protected:
    static ComponentID component_counter_;
};

template <typename T>
struct Component : BaseComponent {
    virtual ~Component() = default;
    static ComponentID GetComponentTypeID() {
        static ComponentID id = component_counter_++;
        return id;
    }
};
}  // namespace ECS

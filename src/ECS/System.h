#pragma once
#include <cstddef>

namespace ECS {
class EntityManager;

class BaseSystem {
   public:
    using SystemID = std::size_t;
    virtual void Update(EntityManager& entity_manager, float time_delta) = 0;
    virtual ~BaseSystem();

   protected:
    static SystemID system_counter_;
};

template <class T>
class System : public BaseSystem {
   public:
    virtual ~System() {}
    static SystemID GetSystemTypeID() {
        static SystemID id = system_counter_++;
        return id;
    }
};
}  // namespace ECS

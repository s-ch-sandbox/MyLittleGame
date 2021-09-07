#pragma once
#include "../ECS/System.h"

class TransformSystem : public ECS::System<TransformSystem> {
   public:
    TransformSystem(float x_limit, float y_limit);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
    const float x_limit_;
    const float y_limit_;
};

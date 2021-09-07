#pragma once
#include "../ECS/System.h"

class MouseControllerSystem : public ECS::System<MouseControllerSystem> {
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
};

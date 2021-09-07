#pragma once
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"

class CameraSystem : public ECS::System<CameraSystem> {
   public:
    explicit CameraSystem(int world_width, int world_height);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;

    int world_width_;
    int world_height_;
};

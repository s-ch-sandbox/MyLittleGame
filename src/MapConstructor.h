#pragma once
#include <string>

#include "ECS/EntityManager.h"

void LoadMap(ECS::EntityManager& entity_manager, const std::string& path, size_t* size_x, size_t* size_y);

#pragma once
#include "ECS/EntityManager.h"

enum GroupID : ECS::EntityManager::GroupID { PLAYER = 0, MAP, ENEMY, STONE, N_GROUPS };

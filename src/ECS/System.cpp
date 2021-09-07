#include "System.h"

namespace ECS {
BaseSystem::SystemID BaseSystem::system_counter_ = 0;

BaseSystem::~BaseSystem() {}
}  // namespace ECS

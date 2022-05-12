#ifndef __GAME_WORLD_SETTINGS_HPP__
#define __GAME_WORLD_SETTINGS_HPP__

#include "CoreMinimal.hpp"

struct ENGINE_API GameWorldSettings
{
	Vector3	m_gravity = Vector3(0.0f, -9.81f, 0.0f);
};

#endif // !__GAME_WORLD_SETTINGS_HPP__
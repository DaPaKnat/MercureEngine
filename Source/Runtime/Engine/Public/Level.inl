#ifndef __LEVEL_INL__
#define __LEVEL_INL__

#include "Level.hpp"
#include "World.hpp"
#include "AssetManager.hpp"

// ==============================[Public Local Methods]============================== //

INLINE World*					    Level::GetWorld			    ()	const
{
	return m_owningWorld;
}

INLINE void							Level::SetWorld			    (World*	p_world)
{
	m_owningWorld = p_world;
}

INLINE GameWorldSettings*			Level::GetWorldSettings	    ()
{
	return &m_gameWorldSettings;
}

INLINE bool							Level::DestroyEntity		(Entity*		p_entity)
{
	if (p_entity == nullptr || m_owningWorld == nullptr)
		return false;

	auto check = std::find(m_entities.begin(), m_entities.end(), p_entity);

	if (check != m_entities.end())
		m_entities.erase(check);

    p_entity->Destroy();

    return ObjectManager::Get().DestroyObject(p_entity);
}

INLINE std::vector<Entity*> const&	Level::GetEntities		    ()  const  
{
	return m_entities;
}

INLINE size_t						Level::GetEntityCount		()	const
{
	return m_entities.size();
}

INLINE void                         Level::InitializeEntities   ()
{
    for (auto entity : m_entities)
    {
        if (entity != nullptr)
        {
            entity->PreInitialize();
            entity->Initialize();
            entity->PostInitialize();
        }
    }
}

#endif // !__LEVEL_INL__
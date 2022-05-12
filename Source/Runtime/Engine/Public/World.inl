#ifndef __WORLD_INL__
#define __WORLD_INL__

#include "GameFramework/Entity.hpp"
#include "GameWorldSettings.hpp"
#include "Level.hpp"

// ============================== [Public Local Methods] ============================== //

INLINE Level*               World::CreateLevel          (std::string const& p_levelName,
                                                         bool const         p_isCurrent)
{
    Level* level = ObjectManager::Get().CreateObject<Level>(p_levelName);

    if (p_isCurrent)
    {
        m_currentLevel = level;

        m_currentLevel->SetWorld(this);
    }

    return level;
}

INLINE Level*			    World::GetCurrentLevel      ()
{
	return m_currentLevel;
}

INLINE void                 World::SetCurrentLevel      (Level*       p_level)
{
    if (p_level != nullptr)
    {
        m_currentLevel = p_level;
    }
}

INLINE bool                 World::SaveCurrentLevel     ()
{
    if (m_currentLevel != nullptr)
    {
        return m_currentLevel->Save();
    }

    return false;
}

INLINE bool                 World::SaveAll              ()
{
    std::vector<Level*> levels  {ObjectManager::Get().GetObjectsOfType<Level>()};

    bool result = true;

    for (auto level : levels)
    {
        if (level == nullptr)
            result = false;

        result = level->Save() ? result : false;
    }

    return result;
}

INLINE Level*               World::LoadLevel            (std::string const& p_name)
{
    if (m_currentLevel == nullptr)
        CreateLevel(p_name, true);

    return m_currentLevel->Load(p_name);
}

INLINE Level*               World::OpenLevel            (std::string const& p_name)
{
    std::vector<Level*> levels = ObjectManager::Get().GetObjectsOfType<Level>();

    for (Level* level : levels)
    {
        if (level->GetName() == p_name)
        {
            m_currentLevel = level;
         
            break;
        }
    }

    return m_currentLevel;
}

INLINE GameWorldSettings*	World::GetWorldSettings     ()
{
	if (m_currentLevel != nullptr)
	{
		return m_currentLevel->GetWorldSettings();
	}
	else
		return nullptr;
}

INLINE void					World::SetGravity	        (Vector3 const&     p_gravity)
{
	if (m_currentLevel != nullptr)
	{
		m_currentLevel->GetWorldSettings()->m_gravity = p_gravity;
	}
}

INLINE Vector3				World::GetGravity	        ()	const
{
	if (m_currentLevel != nullptr)
	{
		return m_currentLevel->GetWorldSettings()->m_gravity;
	}
	else
		return Vector3::Zero;
}

INLINE Vector3				World::GetDefaultGravity    ()	const
{
	return Vector3(0.0f, -9.81f, 0.0f);
}

INLINE bool					World::HasBegunPlay		    ()  const
{
	return m_hasBegunPlay;
}

INLINE size_t				World::GetEntityCount		()	const
{
	if (m_currentLevel != nullptr)
	{
		return m_currentLevel->GetEntityCount();
	}
	else
		return 0;
}

INLINE bool					World::DestroyEntity		(Entity*		    p_entity)
{
	if (p_entity == nullptr)
		return false;

    if (m_currentLevel != nullptr)
    {
        return m_currentLevel->DestroyEntity(p_entity);
    }

    return false;
}

INLINE void                 World::InitializeLevel      ()
{
    if (m_currentLevel != nullptr)
    {
        m_currentLevel->InitializeEntities();
    }
}

#endif // !__WORLD_INL__
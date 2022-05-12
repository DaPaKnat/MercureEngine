#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include "CoreMinimal.hpp"
#include "GameWorldSettings.hpp"
#include "ObjectManager.hpp"

// ==============================[Forward Declaration]============================== //

	class	Object;
	class	Entity;
	class	World;

// ================================================================================= //

class ENGINE_API Level : public Object
{
	public:
	
	// ==============================[Public Constructor and Destructor]============================== //

		Level	()      				= default;

		Level	(Level const&   p_copy)	= default;

		Level	(Level&&        p_move)	= default;

		~Level  ()						= default;

	// ==============================[Public Local Methods]============================== //

		[[nodiscard]] World*			    GetWorld			    ()								const;

		void								SetWorld			    (World*		        p_world);

		[[nodiscard]] GameWorldSettings*	GetWorldSettings	    ();

		void								BeginPlay			    ();

		void								Tick				    (float const		p_deltaTime);

		void								EndPlay				    ();

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		T*									SpawnEntity			    (std::string const&	p_name = "",
																     Args...			p_args)
		{
			if (m_owningWorld != nullptr)
			{
				T* entity = ObjectManager::Get().CreateObject<T>(p_name, p_args...);

				if (entity != nullptr)
				{
                    entity->SetWorld(GetWorld());
                    m_entities.push_back(entity);
				}

				return entity;
			}
			else
				return nullptr;
		}

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		T*									SpawnEntity			    (Transform const&	p_transform,
																     Args...			p_args)
		{
			if (m_owningWorld != nullptr)
			{
				T* entity = ObjectManager::Get().CreateObject<T>(p_transform, p_args...);

				if (entity != nullptr)
				{
                    entity->SetWorld(GetWorld());
                    m_entities.push_back(entity);
				}

				return entity;
			}
			else
				return nullptr;
		}

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		T*									SpawnEntity			    (std::string const&	p_name,
																     Transform const&	p_transform,
																     Args...			p_args)
		{
			if (m_owningWorld != nullptr)
			{
				T* entity = ObjectManager::Get().CreateObject<T>(p_name, p_transform, p_args...);

				if (entity != nullptr)
				{
                    entity->SetWorld(GetWorld());
                    m_entities.push_back(entity);
				}

				return entity;
			}
			else
				return nullptr;
		}

		bool								DestroyEntity		    (Entity*			p_entity);

		std::vector<Entity*> const&			GetEntities			    ()  const;

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<EntityComponent, T>>>
        std::vector<T*>                     GetComponents           ()  const
        {
            std::vector<T*> components = ObjectManager::Get().GetObjectsOfType<T>();
            std::vector<T*> output;

            for (T* component : components)
            {
                Entity* owner = component->GetOwner();

                if (owner && std::find(m_entities.begin(), m_entities.end(), owner) != m_entities.end() && !owner->IsPendingKill())
                    output.push_back(component);
            }

            return output;
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<EntityComponent, T>>>
        void                                GetComponents           (std::vector<T*>&   p_outComponents)  const
        {
            std::vector<T*> components = ObjectManager::Get().GetObjectsOfType<T>();

            for (T* component : components)
            {
                Entity* owner = component->GetOwner();

                if (owner && std::find(m_entities.begin(), m_entities.end(), component->GetOwner()) != m_entities.end() && !owner->IsPendingKill())
                    p_outComponents.push_back(component);
            }
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<SceneComponent, T>>>
        void                                GetVisibleComponents    (std::vector<T*>&   p_outComponents)  const
        {
            std::vector<T*> components = ObjectManager::Get().GetObjectsOfType<T>();

            for (T* component : components)
            {
                Entity* owner = component->GetOwner();

                if (component && component->IsVisible() && owner &&
                    std::find(m_entities.begin(), m_entities.end(), component->GetOwner()) != m_entities.end() &&
                    !owner->IsPendingKill())
                {
                    p_outComponents.push_back(component);
                }
            }
        }

		template <typename T, typename = std::enable_if_t<std::is_base_of_v<Entity, T>>>
		std::vector<Entity*> const&			GetEntities			    ()
		{
			std::vector<Entity*> entities;

			size_t typeID = typeid(T).hash_code();

			for (auto entity : m_entities)
			{
				if (entity != nullptr)
				{
					if (entity->GetInstanceType() == typeID)
					{
						entities.push_back(entity)
					}
				}
			}

			return entities;
		}

		size_t								GetEntityCount		    ()								const;

        void                                InitializeEntities      ();

        Level*                              Load                    (std::string const& p_name);

        bool                                Save                    ();

        bool                                CheckContentDirectory   ()                              const;

    // ==============================[Private Local Properties]============================== //

        Level&  operator=   (Level const&   p_copy) = default;

	private:

	// ==============================[Private Local Properties]============================== //

		World*			        m_owningWorld;

		GameWorldSettings		m_gameWorldSettings;

		std::vector<Entity*>	m_entities;

}; // !class Level

#include "Level.inl"

#endif // !__LEVEL_HPP__
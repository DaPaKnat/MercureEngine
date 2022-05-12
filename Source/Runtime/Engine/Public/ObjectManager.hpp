#ifndef __OBJECT_MANAGER_HPP__
#define __OBJECT_MANAGER_HPP__

#include "EngineModule.hpp"
#include "Object/Object.hpp"

class ENGINE_API ObjectManager : public EngineModule
{
	public:

	// ============================== [Public Static Method] ============================== //

        static INLINE ObjectManager& Get() noexcept
        { 
            return GEngine->GetModuleManager()->Get<ObjectManager>();
        }

	// ============================== [Public Constructors and Destructor] ============================== //

		ObjectManager	()						= default;

		~ObjectManager	()                      = default;
		
    // ============================== [Public Local Methods] ============================== //

		template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>, typename... Args>
		T*				CreateObject	    (std::string const& p_name = "",
										     Args&&...			p_args)
		{
			size_t typeID	= typeid(T).hash_code();
			size_t id		= m_objects[typeID].size();

			for (size_t i = 0; i < id; i++)
			{
				if (m_objects[typeID][i] != nullptr)
				{
					if (m_objects[typeID][i]->IsPendingKill())
					{
                        m_objects[typeID][i].reset(new T(std::forward<Args>(p_args)...));

						m_objects[typeID][i]->m_instanceID = i;

						m_objects[typeID][i]->m_instanceType = typeID;

						m_objects[typeID][i]->m_instanceName = p_name;

						return static_cast<T*>(m_objects[typeID][i].get());
					}
				}
			}

			m_objects[typeID].push_back(std::make_shared<T>(std::forward<Args>(p_args)...));

			m_objects[typeID][id]->m_instanceID = id;

			m_objects[typeID][id]->m_instanceType = typeID;

			m_objects[typeID][id]->m_instanceName = p_name;

			return static_cast<T*>(m_objects[typeID][id].get());
		}

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
        T*              GetObjectOfType     (size_t             p_id)
        {   
            size_t typeId = typeid(T).hash_code();

            if (m_objects.find(typeId) != m_objects.end())
            {
                for (auto object : m_objects[typeId])
                {
                    if (object != nullptr)
                    {
                        if (object->GetInstanceID() == p_id)
                        {
                            return static_cast<T*>(object.get());
                        }
                    }
                }
            }

            return nullptr;
        }

        Object*         GetObjectOfType     (size_t             p_typeId,
                                             size_t             p_id)
        {   
            if (m_objects.find(p_typeId) != m_objects.end())
            {
                for (auto object : m_objects[p_typeId])
                {
                    if (object != nullptr)
                    {
                        if (object->GetInstanceID() == p_id)
                        {
                            return object.get();
                        }
                    }
                }
            }

            return nullptr;
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
        std::vector<T*> GetObjectsOfType    ()
        {
            std::vector<T*> objects;

            size_t typeId = typeid(T).hash_code();

            if (m_objects.find(typeId) != m_objects.end())
            {
                for (auto object : m_objects[typeId])
                    objects.push_back(static_cast<T*>(object.get()));
            }

            return objects;
        }

		template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
		bool			DestroyObject	    (size_t const		p_id)
		{
			size_t typeID = typeid(T).hash_code();

			if (m_objects.find(typeID) == m_objects.end())
				return false;

            for (auto object : m_objects[typeID])
            {
                if (object.get() != nullptr)
                {
                    if (object->m_instanceID == p_id)
                    {
                        object->m_isPendingKill = true;

                        return true;
                    }
                }
            }

            return false;
		}
		
		bool			DestroyObject	    (Object*		    p_object)
		{
			if (p_object == nullptr)
				return false;

			size_t typeID = p_object->GetInstanceType();

			if (m_objects.find(typeID) == m_objects.end())
				return false;

			for (auto& object : m_objects[typeID])
			{
				if (object != nullptr)
				{
					if (object.get() == p_object)
					{
						object->m_isPendingKill = true;

						return true;
					}
				}
			}

			return false;
		}

		void			Initialize		    (EngineKey const&   p_passkey) override final {}

		void			Update			    (EngineKey const&   p_passkey) override final {}

		void			Shutdown		    (EngineKey const&   p_passkey) override final {}

	private:
	
	// ============================== [Private Local Properties] ============================== //

		std::unordered_map<size_t, std::vector<std::shared_ptr<Object>>>	m_objects;

}; // !class ObjectManager

#endif // !__OBJECT_MANAGER_HPP__
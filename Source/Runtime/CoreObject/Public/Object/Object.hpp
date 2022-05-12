#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "CoreMinimal.hpp"
#include "EngineModuleManager.hpp"

// ==============================[Forward Declaration]============================== //

	class ObjectManager;
    class ObjectFactory;

// ================================================================================= //

class ENGINE_API Object
{
    REFLECT(Object)

	friend ObjectManager;
    friend ObjectFactory;

	using ObjectID      = size_t;
	using ObjectTypeID  = size_t;

	public:

	// ============================== [Public Constructors and Destructor] ============================== //

		Object	() :
            m_instanceName  { "" },
            m_instanceType  { 0 },
            m_instanceID    { 0 },
            m_isPendingKill { false }
        {}

		~Object	() = default;

	// ============================== [Public Local Methods] ============================== //

		ObjectID		GetInstanceID	()	                            const;

		ObjectTypeID	GetInstanceType	()	                            const;

		char const*		GetName			()	                            const;

        void            SetName         (char const*    p_name);        

		bool			IsPendingKill	()	                            const;

        virtual void    Serialize       (Json&          p_serialize)    const;

        virtual void    Deserialize     (Json const&    p_deserialize,
                                         Json const&    p_components);

	protected:

	// ============================== [Protected Local Methods] ============================== //
		
		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
		T*		        CreateObject    (Args...	p_args)
		{
			return EngineModuleManager::Get<ObjectManager>()->CreateObject<T>(p_args);
		}

	private:

	// ============================== [Private Local Properties] ============================== //

		/** Instance name (can be empty). */
        PROPERTY()
		std::string		m_instanceName;

		/** Instance ID (unique compared to his type). */
        PROPERTY()
		ObjectID		m_instanceID;

		/** Object type id (unique). */
        PROPERTY()
		ObjectTypeID	m_instanceType;

		/** Is the Object waiting to be destroyed? */
        PROPERTY()
		bool			m_isPendingKill;

}; // !class Object

#include "Object.inl"
#include "Object.generated.hpp"

#endif // !__REWORK_OBJECT_HPP__
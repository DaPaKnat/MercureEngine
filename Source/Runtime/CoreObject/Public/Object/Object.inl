#ifndef __OBJECT_INL__
#define __OBJECT_INL__

#include "Object.hpp"

INLINE size_t		Object::GetInstanceID   ()	                            const
{
	return m_instanceID;
}

INLINE size_t		Object::GetInstanceType	()	                            const
{
	return m_instanceType;
}

INLINE char const*	Object::GetName			()	                            const
{
	return m_instanceName.c_str();
}

INLINE void         Object::SetName         (char const*    p_name)
{
    m_instanceName = p_name;
}

INLINE bool			Object::IsPendingKill   ()	                            const
{
	return m_isPendingKill;
}

INLINE void			Object::Serialize       (Json&          p_seriliaze)    const
{
    p_seriliaze = Json(*this);
}

INLINE void			Object::Deserialize     (Json const&    p_deseriliaze,
                                             Json const&    p_components)
{
    *this = p_deseriliaze;
}

#endif // !__OBJECT_INL__
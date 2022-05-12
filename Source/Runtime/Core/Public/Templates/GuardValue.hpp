#ifndef __GUARD_VALUE_HPP__
#define __GUARD_VALUE_HPP__

#include "PCH.hpp"

#include "Helpers/NonCopyable.hpp"

template <typename RefType, 
          typename AssignedType = RefType>
class GuardValue : private NonCopyable
{
    public :
    
    // ==============================[Public Constructor + Destructor]============================== //

        GuardValue  (RefType&               p_referenceValue,
                     AssignedType const&    p_newValue)
        :   m_refValue  {p_referenceValue},
            m_oldValue  {p_referenceValue}
        {
            m_refValue = p_newValue;
        }

    // ==============================[Protected Constructor + Destructor]============================== //

        virtual ~GuardValue ()  override
        {
            m_refValue = m_oldValue;
        }

    private :

    // ==============================[Private Local Properties]============================== //

        RefType&        m_refValue;

        AssignedType    m_oldValue;
};



#endif // !__GUARD_VALUE_HPP__
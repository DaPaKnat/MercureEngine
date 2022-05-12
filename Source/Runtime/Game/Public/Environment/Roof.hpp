#ifndef __ROOF_HPP__
#define __ROOF_HPP__

#include "CoreMinimal.hpp"

#include "GameFramework/Entity.hpp"

#include "Components/BoxComponent.hpp"
#include "Components/StaticMeshComponent.hpp"

DECLARE_LOG_CATEGORY(LogRoof);

class ENGINE_API Roof : public Entity
{
    REFLECT(Roof)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Roof    ();

        Roof    (Roof const&)   = default;

        Roof    (Roof&&)        = default;

        ~Roof   ()              = default;

    // ============================== [Public Local Operators] ============================== //

        Roof&   operator=   (Roof const&)   = default;

        Roof&   operator=   (Roof&&)        = default;

    private:

    // ============================== [Private Local Variables] ============================== //

        BoxComponent*           m_collider  = nullptr;
        
        StaticMeshComponent*    m_mesh      = nullptr;

    // ============================== [Private Local Methods] ============================== //

        bool    CreateDefaultComponents ();

        void    InitializeCollider      ();

        void    InitializeHierarchy     ();

        void    InitializeMesh          ();

        void    InitializeProperties    ();

};  // !class Roof

#include "Roof.generated.hpp"

#endif // !__ROOF_HPP__
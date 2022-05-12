#ifndef __FLOOR_HPP__
#define __FLOOR_HPP__

#include "CoreMinimal.hpp"

#include "GameFramework/Entity.hpp"

#include "Components/BoxComponent.hpp"
#include "Components/StaticMeshComponent.hpp"

DECLARE_LOG_CATEGORY(LogFloor);

class ENGINE_API Floor : public Entity
{
    REFLECT(Floor)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Floor   ();

        Floor   (Floor const&)  = default;

        Floor   (Floor&&)       = default;

        ~Floor  ()              = default;

    // ============================== [Public Local Operators] ============================== //

        Floor&  operator=   (Floor const&)  = default;

        Floor&  operator=   (Floor&&)       = default;

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

};  // !class Floor

#include "Floor.generated.hpp"

#endif // !__FLOOR_HPP__
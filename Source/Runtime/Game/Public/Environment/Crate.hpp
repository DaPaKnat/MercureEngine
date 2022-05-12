#ifndef __CRATE_HPP__
#define __CRATE_HPP__

#include "CoreMinimal.hpp"

#include "GameFramework/Entity.hpp"

#include "Components/BoxComponent.hpp"
#include "Components/StaticMeshComponent.hpp"

class ENGINE_API Crate : public Entity
{
    REFLECT(Crate)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Crate    ();

        Crate    (Crate const&)   = default;

        Crate    (Crate&&)        = default;

        ~Crate   ()              = default;

    // ============================== [Public Local Operators] ============================== //

        Crate&   operator=   (Crate const&)   = default;

        Crate&   operator=   (Crate&&)        = default;

    private:

    // ============================== [Private Local Variables] ============================== //

        BoxComponent*           m_collider  = nullptr;
        
        StaticMeshComponent*    m_mesh      = nullptr;

    // ============================== [Private Local Methods] ============================== //

        bool    ComponentsCreation      ();

        void    InitializeCollider      ();

        void    InitializeHierarchy     ();

        void    InitializeMesh          ();

        void    InitializeProperties    ();

};  // !class Crate

#include "Crate.generated.hpp"

#endif // !__CRATE_HPP__
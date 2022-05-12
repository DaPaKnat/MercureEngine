#ifndef __WALL_HPP__
#define __WALL_HPP__

#include "CoreMinimal.hpp"

#include "GameFramework/Entity.hpp"

#include "Components/BoxComponent.hpp"
#include "Components/StaticMeshComponent.hpp"

DECLARE_LOG_CATEGORY(LogWall);

class ENGINE_API Wall : public Entity
{
    REFLECT(Wall)

    typedef Entity Super;

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Wall    ();

        Wall    (Wall const&)   = default;

        Wall    (Wall&&)        = default;

        ~Wall   ()              = default;

    // ============================== [Public Local Operators] ============================== //

        Wall&   operator=   (Wall const&)   = default;

        Wall&   operator=   (Wall&&)        = default;

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

};  // !class Wall

#include "Wall.generated.hpp"

#endif // !__WALL_HPP__
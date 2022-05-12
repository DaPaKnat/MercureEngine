#ifndef __BOX_COMPONENT_HPP__
#define __BOX_COMPONENT_HPP__

#include "PrimitiveComponent.hpp"

class ENGINE_API BoxComponent : public PrimitiveComponent
{
	REFLECT(BoxComponent)

    typedef PrimitiveComponent Super;

    public:

    // ==============================[Public Constructor + Destructor]============================== //

        BoxComponent    ();

        BoxComponent    (BoxComponent const& p_copy)    = default;

        BoxComponent    (BoxComponent&&      p_move)    = default;

        ~BoxComponent   ()                              = default;

    // ==============================[Virtual Public Local Methods]============================== //

//      virtual BoxSphereBound  ComputeBounds       (Transform const&   p_localToWorld) const override;

        virtual CollisionShape  GetCollisionShape   ()                                  const override;

        virtual EGeometryType   GetGeometryType     ()                                  const override;

        virtual bool            IsZeroExtent        ()                                  const override;

//      virtual void            UpdateBodySetup     ()                                  override;


    // ==============================[Public Local Methods]============================== //

        FORCEINLINE Vector3 GetScaledBoxExtent      ()                                          const;

        FORCEINLINE Vector3 GetUnscaledBoxExtent    ()                                          const;


        void                SetBoxExtent            (Vector3 const& p_newBoxExtent,
                                                     bool const     p_updateOverlaps = true);

        void                SetBoxExtent            (float const    p_newBoxExtentXYZ,
                                                     bool const     p_updateOverlaps = true);

        void                SetBoxExtent            (float const    p_boxExtentX,
                                                     float const    p_boxExtentY,
                                                     float const    p_boxExtentZ,
                                                     bool const     p_updateOverlaps = true);

        FORCEINLINE void    InitBoxExtent           (Vector3 const& p_boxExtent);

        FORCEINLINE void    InitBoxExtent           (float const    p_boxExtentXYZ);


        FORCEINLINE void    InitBoxExtent           (float const    p_boxExtentX,
                                                     float const    p_boxExtentY,
                                                     float const    p_boxExtentZ);

    // ==============================[Public Local Operators]============================== //

        BoxComponent&   operator=   (BoxComponent const&    p_copy) = default;

    protected:
    
    // ==============================[Protected Local Properties]============================== //

        /** The extents of the box */
		PROPERTY()
        Vector3 m_boxExtent;

        /** ... */
        float   m_lineThickness;

}; // !class BoxComponent

#include "BoxComponent.inl"
#include "BoxComponent.generated.hpp"

#endif // !__BOX_COMPONENT_HPP__
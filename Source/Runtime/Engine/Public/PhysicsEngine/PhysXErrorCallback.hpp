#ifndef __PHYSX_ERROR_CALLBACK_HPP__
#define __PHYSX_ERROR_CALLBACK_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"

/**
 * Class used to report PhysX Error.
 */
class PhysXErrorCallback: public physx::PxErrorCallback
{
    public:

    // ==============================[Public Constructor + Destructor]============================== //

        PhysXErrorCallback  ()                                  = default;
        
        PhysXErrorCallback  (PhysXErrorCallback const&  p_copy) = default;
        
        PhysXErrorCallback  (PhysXErrorCallback&&       p_move) = default;

        ~PhysXErrorCallback ()                                  = default;

    // ==============================[Virtual Public Local Methods]============================== //
        
        virtual void    reportError (physx::PxErrorCode::Enum   p_code, 
                                     char const*                p_message, 
                                     char const*                p_file, 
                                     int                        p_line);

}; // !class PhysXErrorCallbak

#endif // !__PHYSX_ERROR_CALLBACK_HPP__

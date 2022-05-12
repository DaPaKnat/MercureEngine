#include "PCH.hpp"

#include "PhysicsEngine/PhysXErrorCallback.hpp"

// ==============================[Virtual Public Local Methods]============================== //

/**
 * Reports PhysX error code.
 *
 * @param p_code    The error code (See physx::PxErrorCode).
 * @param p_message The error message to display.
 * @param p_file    The file error occured in.
 * @param p_line    The Line number error occured on.
 */
void    PhysXErrorCallback::reportError (physx::PxErrorCode::Enum  p_code, 
                                         char const*               p_message, 
                                         char const*               p_file, 
                                         int                       p_line)
{
    switch (p_code)
    {
        case physx::PxErrorCode::eABORT:
            LOG(LogPhysics, Error, "PhysX: (%s %d) %s : %s", p_file, p_line, "eABORT", p_message);
            break;

        case physx::PxErrorCode::eDEBUG_INFO:
            LOG(LogPhysics, Log, "PhysX: (%s %d) %s : %s", p_file, p_line, "eDEBUG_INFO", p_message);
            break;

#ifndef NDEBUG

        case physx::PxErrorCode::eDEBUG_WARNING:
            LOG(LogPhysics, Warning, "PhysX: (%s %d) %s : %s", p_file, p_line, "eDEBUG_WARNING", p_message);
            break;

#endif // !NDEBUG 

        case physx::PxErrorCode::eINTERNAL_ERROR:
            LOG(LogPhysics, Warning, "PhysX: (%s %d) %s : %s", p_file, p_line, "eINTERNAL_ERROR", p_message);
            break;

        case physx::PxErrorCode::eINVALID_OPERATION:
            LOG(LogPhysics, Error, "PhysX: (%s %d) %s : %s", p_file, p_line, "eINVALID_OPERATION", p_message);
            break;

        case physx::PxErrorCode::eINVALID_PARAMETER:
            LOG(LogPhysics, Error, "PhysX: (%s %d) %s : %s", p_file, p_line, "eINVALID_PARAMETER", p_message);
            break;

        case physx::PxErrorCode::eNO_ERROR:
            LOG(LogPhysics, Log, "PhysX: (%s %d) %s : %s", p_file, p_line, "eNO_ERROR", p_message);
            break;

        case physx::PxErrorCode::eOUT_OF_MEMORY:
            LOG(LogPhysics, Error, "PhysX: (%s %d) %s : %s", p_file, p_line, "eOUT_OF_MEMORY", p_message);
            break;

        case physx::PxErrorCode::ePERF_WARNING:
            LOG(LogPhysics, Warning, "PhysX: (%s %d) %s : %s", p_file, p_line, "ePERF_WARNING", p_message);
            break;

        case physx::PxErrorCode::eMASK_ALL:
            LOG(LogPhysics, Log, "PhysX: (%s %d) %s : %s", p_file, p_line, "eMASK_ALL", p_message);
            break;

        default:
            LOG(LogPhysics, Log, "PhysX: (%s %d) %s : %s", p_file, p_line, "Unknown Error Code", p_message);
            break;
    }
}

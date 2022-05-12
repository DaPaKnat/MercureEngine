#ifndef __PHYSICS_SIMULATION_FILTER_SHADER_HPP__
#define __PHYSICS_SIMULATION_FILTER_SHADER_HPP__

#include "PxFiltering.h"

// ==============================[Global Methods]============================== //

physx::PxFilterFlags    PhysicsSimulationFilterShader   (physx::PxFilterObjectAttributes    p_attributes0, 
                                                         physx::PxFilterData                p_filterData0, 
                                                         physx::PxFilterObjectAttributes    p_attributes1,
                                                         physx::PxFilterData                p_filterData1,
                                                         physx::PxPairFlags&                p_pairFlags, 
                                                         const void*                        p_constantBlock, 
                                                         physx::PxU32                       p_constantBlockSize);

#endif // !__PHYSICS_SIMULATION_FILTER_SHADER_HPP__

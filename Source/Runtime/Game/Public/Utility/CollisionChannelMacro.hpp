#ifndef __COLLISION_CHANNEL_MACRO_HPP__ 
#define __COLLISION_CHANNEL_MACRO_HPP__ 

#include "EngineTypes.hpp" 

#include "CoreMinimal.hpp"

#define ECC_BLOCK_ALL           ECollisionChannel::GameTraceChannel1 
#define ECC_UNCOLORE_OBJECT     ECollisionChannel::GameTraceChannel2 
#define ECC_BLUE_OBJECT         ECollisionChannel::GameTraceChannel3 
#define ECC_ORANGE_OBJECT       ECollisionChannel::GameTraceChannel4 
#define ECC_PLAYER              ECollisionChannel::GameTraceChannel5 
#define ECC_BULLET              ECollisionChannel::GameTraceChannel6 

#define ECC_IGNORED             ECollisionChannel::GameTraceChannel30 

#include "CollisionChannelMacro.generated.hpp"

#endif // !__COLLISION_CHANNEL_MACRO_HPP__ 

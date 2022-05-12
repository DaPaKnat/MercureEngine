#ifndef __OBJECT_FACTORY_HPP__
#define __OBJECT_FACTORY_HPP__

class Entity;
class EntityComponent;

class ENGINE_API ObjectFactory
{
    public:

        static Entity*                  CreateEntityFromTypeID      (Json const&  p_node, Json const&);

        static std::vector<std::string> GetDerivedEntityNames       ();
};

#include "ObjectFactory.generated.hpp"

#endif // !__OBJECT_FACTORY_HPP__
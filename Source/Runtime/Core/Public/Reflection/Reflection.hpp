#ifndef __REFLECTION_HPP__
#define __REFLECTION_HPP__

#include "MetaEnum.hpp"
#include "MetaSerialize.hpp"

#define REFLECT(Type) \
	friend auto Reflect::RegisterMembers<Type>();

#if defined(__PARSING__)
#	define PROPERTY() __attribute__((annotate("Reflect")))
#else
#	define PROPERTY()
#endif

#endif // !__REFLECTION_HPP__
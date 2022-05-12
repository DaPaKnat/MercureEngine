#ifndef __META_STRING_INL__
#define __META_STRING_INL__

#include "MetaString.hpp"

INLINE std::string	Reflect::Detail::CastToString	(bool const&		p_value)
{
	return p_value ? "true" : "false";
}

INLINE std::string	Reflect::Detail::CastToString	(int const&			p_value)
{
	return std::to_string(p_value);
}

INLINE std::string	Reflect::Detail::CastToString	(float const&		p_value)
{
	return std::to_string(p_value);
}

INLINE std::string	Reflect::Detail::CastToString	(std::string const& p_value)
{
	return p_value;
}

template <>
INLINE bool			Reflect::Detail::FromString		(std::string const& p_valueStr)
{
	if (p_valueStr == "true")
		return true;
	else if (p_valueStr == "false")
		return false;
}

template <>
INLINE int			Reflect::Detail::FromString		(std::string const& p_valueStr)
{
	return std::stoi(p_valueStr);
}

template <>
INLINE float		Reflect::Detail::FromString		(std::string const& p_valueStr)
{
	return std::stof(p_valueStr);
}

template <>
INLINE std::string	Reflect::Detail::FromString		(std::string const& p_valueStr)
{
	return p_valueStr;
}

#endif // !__META_STRING_INL__
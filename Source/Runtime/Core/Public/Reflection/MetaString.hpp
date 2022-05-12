#ifndef __META_STRING_HPP__
#define __META_STRING_HPP__

namespace Reflect
{
	namespace Detail
	{
		template <typename T>
		std::string	CastToString	(T const&			p_value);

		std::string CastToString	(bool const&		p_value);

		std::string CastToString	(int const&			p_value);

		std::string CastToString	(float const&		p_value);

		std::string CastToString	(std::string const&	p_value);

		template <typename T>
		T			FromString		(std::string const& p_value);

		template <>
		bool		FromString		(std::string const& valueStr);

		template <>
		int			FromString		(std::string const& valueStr);

		template <>
		float		FromString		(std::string const& valueStr);

		template <>
		std::string FromString		(std::string const& valueStr);

		template <typename T>
		std::string CastToString	(const T&)
		{
			return std::string();
		}

		template <typename T>
		T			FromString		(std::string const&)
		{
			return T();
		}

	} // !namespace Detail

} // !namespace Reflect

#include "MetaString.inl"

#endif // !__META_STRING_HPP__
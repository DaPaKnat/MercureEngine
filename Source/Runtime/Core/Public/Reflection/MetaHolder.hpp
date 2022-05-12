#ifndef __META_HOLDER_HPP__
#define __META_HOLDER_HPP__

namespace Reflect
{
	namespace Detail
	{
		template <typename T, typename TupleType>
		struct MetaHolder 
		{
			static TupleType members;

			static char const*	Name	()
			{
				return RegisterName<T>();
			}

		}; // !struct MetaHolder

		template <typename T, typename TupleType>
		TupleType MetaHolder<T, TupleType>::members = RegisterMembers<T>();

	} // !namespace Detail

} // !namespace Reflect

#endif 
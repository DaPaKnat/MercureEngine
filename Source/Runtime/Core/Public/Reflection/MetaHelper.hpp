#ifndef __META_HELPER_HPP__
#define __META_HELPER_HPP__

namespace Reflect 
{
	namespace Detail 
	{
		template <typename F, typename... Args>
		void ForEachArg	(F&&					p_f, 
						 Args&&...				p_args);

		template <typename F, typename TupleT>
		void ForTuple	(F&&					p_f, 
						 TupleT&&				p_tuple);

		template <typename F>
		void ForTuple	(F&&					p_f, 
						 std::tuple<> const&	p_tuple);

		template <bool Test, typename F, typename... Args, typename = std::enable_if_t<Test>>
		void CallIf		(F&&					p_f, 
						 Args&&...				p_args);

		template <bool Test, typename F, typename... Args, typename = std::enable_if_t<!Test>, typename = void>
		void CallIf		(F&&					p_f,
						 Args&&...				p_args);

	} // !namespace Detail

} // !namespace Reflect

#include "MetaHelper.inl"

#endif // !__META_HELPER_HPP__
#ifndef __META_HELPER_INL__
#define __META_HELPER_INL__

#include "MetaHelper.hpp"

namespace Reflect 
{
	namespace Detail 
	{
		template <typename F, typename... Args>
		void					ForEachArg				(F&&						p_f, 
														 Args&&...					p_args)
		{
			using expand = bool[];

			(void)expand
			{
				(
					p_f(std::forward<Args>(p_args)),
					true
				)...
			};
		}

		template <size_t N>
		decltype(auto)			MakeIndexSequence		()
		{
			return std::make_index_sequence<N>();
		}

		template <>
		INLINE decltype(auto)	MakeIndexSequence<1>	()
		{
			return std::index_sequence<0>();
		}

		template <typename F, typename Tuple, size_t... I>
		decltype(auto)			ApplyImpl				(F&&						p_f, 
														 Tuple&&					p_t, 
														 std::index_sequence<I...>)
		{
			return p_f(std::get<I>(std::forward<Tuple>(p_t))...);
		}

		template <typename F, typename Tuple>
		decltype(auto)			Apply					(F&& p_f, 
														 Tuple&&					p_t)
		{
			constexpr size_t tupleSize = std::tuple_size<std::decay_t<Tuple>>::value;

			return ApplyImpl(std::forward<F>(p_f), std::forward<Tuple>(p_t), MakeIndexSequence<tupleSize>());
		}

		template <typename F, typename TupleT>
		void					ForTuple				(F&& p_f, 
														 TupleT&&					p_tuple)
		{
			// ambiguity with std::Apply
			Reflect::Detail::Apply(
				[&p_f](auto&&... p_elems) 
				{
					ForEachArg(p_f, std::forward<decltype(p_elems)>(p_elems)...);
				},
				std::forward<TupleT>(p_tuple)
			);
		}

		template <typename F>
		void					ForTuple				(F&&, 
														 const std::tuple<>&)
		{
			//
		}

		template <bool Test, typename F, typename... Args, typename>
		void					CallIf					(F&&						p_f, 
														 Args&&...					p_args)
		{
			p_f(std::forward<Args>(p_args)...);
		}

		template <bool Test, typename F, typename... Args, typename, typename>
		void					CallIf					(F&&, 
														 Args&&...)
		{ 
			//
		}

	} // !namespace Detail

} // !namespace Reflect

#endif // !__META_HELPER_INL__
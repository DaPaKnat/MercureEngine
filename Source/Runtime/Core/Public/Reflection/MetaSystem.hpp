#ifndef __META_SYSTEM_HPP__
#define __META_SYSTEM_HPP__

template <typename... Args>
struct TypeList
{
	template <std::size_t N>
	using Type = std::tuple_element_t<N, std::tuple<Args...>>;

	using Indices = std::index_sequence_for<Args...>;

	static const size_t size = sizeof...(Args);

}; // !struct TypeList

namespace Reflect
{
	template <typename T>
	struct ConstructorArgs
	{
		using types = TypeList<>;

	}; // !struct ConstructorArgs

	template <typename T>
	using ConstructorArguments = typename ConstructorArgs<T>::types;

	template <typename... Args>
	auto					Members					(Args&&... args);

	template <typename Class>
	INLINE auto				RegisterMembers			();

	template <typename Class>
	constexpr auto			RegisterName			();

	template <typename Class>
	constexpr auto			GetName					();

	template <typename Class>
	constexpr std::size_t	GetMemberCount			();

	template <typename Class>
	const auto&				GetMembers				();

	template <typename Class>
	constexpr bool			IsRegistered			();

	template <typename Class>
	constexpr bool			IsConstructorRegistered	();

	template <typename Class>
	bool					HasMember				(const char* name);

	template <typename Class, typename F, typename = std::enable_if_t<IsRegistered<Class>()>>
	void					DoForAllMembers			(F&& f);

	template <typename Class, typename F, typename = std::enable_if_t<!IsRegistered<Class>()>, typename = void>
	void					DoForAllMembers			(F&& f);

	template <typename Class, typename T, typename F>
	void					DoForMember				(const char* name, F&& f);

	template <typename T, typename Class>
	T						GetMemberValue			(Class& obj, const char* name);

	template <typename T, typename Class, typename V, typename = std::enable_if_t<std::is_constructible<T, V>::value>>
	void					SetMemberValue			(Class& obj, const char* name, V&& value);

} // !namespace Reflect

#include "MetaSystem.inl"

#endif // !__META_SYSTEM_HPP__
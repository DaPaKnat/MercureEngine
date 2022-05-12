#ifndef __META_SYSTEM_INL__
#define __META_SYSTEM_INL__

#include "MetaSystem.hpp"

#include "MetaMember.hpp"
#include "MetaHelper.hpp"
#include "MetaHolder.hpp"

namespace Reflect
{
	template <typename... Args>
	auto					Members					(Args&&...		p_args)
	{
	    return std::make_tuple(std::forward<Args>(p_args)...);
	}
	
	template <typename Class>
	INLINE auto				RegisterMembers			()
	{
	    return std::make_tuple();
	}
	
	template <typename Class>
	constexpr auto			RegisterName			()
	{
	    return "";
	}
	
	template <typename Class>
	constexpr auto			GetName					()
	{
	    return Detail::MetaHolder<Class, decltype(RegisterMembers<Class>())>::Name();
	}
	
	template <typename Class>
	constexpr std::size_t	GetMemberCount			()
	{
		return std::tuple_size<decltype(RegisterMembers<Class>())>::value;
	}
	
	template <typename Class>
	const auto&				GetMembers				()
	{
	    return Detail::MetaHolder<Class, decltype(RegisterMembers<Class>())>::members;
	}
	
	template <typename Class>
	constexpr bool			IsRegistered			()
	{
	    return !std::is_same<std::tuple<>, decltype(RegisterMembers<Class>())>::value;
	}
	
	template <typename Class>
	constexpr bool			IsConstructorRegistered	()
	{
	    return !std::is_same<TypeList<>, ConstructorArguments<Class>>::value;
	}
	
	template <typename Class>
	bool					HasMember				(char const*	p_name)
	{
	    bool found = false;

	    DoForAllMembers<Class>(
	        [&found, &p_name](const auto& p_member)
	        {
	            if (!strcmp(p_name, p_member.GetName())) 
				{
	                found = true;
	            }
	        }
	    );

	    return found;
	}
	
	template <typename Class, typename F, typename>
	void					DoForAllMembers			(F&&			p_f)
	{
	    Detail::ForTuple(std::forward<F>(p_f), GetMembers<Class>());
	}
	
	template <typename Class, typename F, typename, typename>
	void					DoForAllMembers			(F&&)
	{
		//
	}
	
	template <typename Class, typename T, typename F>
	void					DoForMember				(const char*	p_name, 
													 F&&			p_f)
	{
	    DoForAllMembers<Class>(
	        [&](const auto& p_member)
	        {
	            if (!strcmp(p_name, p_member.GetName())) 
				{
	                using MemberT = Reflect::GetMemberType<decltype(p_member)>;

	                assert((std::is_same<MemberT, T>::value) && "Member doesn't have type T");

	                Detail::CallIf<std::is_same<MemberT, T>::value>(std::forward<F>(p_f), p_member);
	            }
	        }
	    );
	}
	
	template <typename T, typename Class>
	T						GetMemberValue			(Class&			p_obj, 
													 char const*	p_name)
	{
	    T value;

	    DoForMember<Class, T>(p_name,
	        [&value, &p_obj](const auto& p_member)
	        {
	            value = p_member.GetCopy(p_obj);
	        }
	    );

	    return value;
	}
	
	template <typename T, typename Class, typename V, typename>
	void					SetMemberValue			(Class&			p_obj, 
													 const char*	p_name, 
													 V&&			p_value)
	{
	    DoForMember<Class, T>(p_name,
	        [&p_obj, p_value = std::forward<V>(p_value)](const auto& p_member)
	        {
	            p_member.Set(p_obj, p_value);
	        }
	    );
	}

} // !namespace Reflect

#endif // !__META_SYSTEM_INL__
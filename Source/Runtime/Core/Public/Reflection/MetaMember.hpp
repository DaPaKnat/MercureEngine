#ifndef __META_MEMBER_HPP__
#define __META_MEMBER_HPP__

namespace Reflect
{
	template <typename Class, typename T>
	using MemberPtrT				= T Class::*;

	template <typename Class, typename T>
	using RefGetterFuncPtrT			= const T& (Class::*)() const;

	template <typename Class, typename T>
	using RefSetterFuncPtrT			= void (Class::*)(T const&);

	template <typename Class, typename T> 
	using ValGetterFuncPtrT			= T(Class::*)() const;

	template <typename Class, typename T>
	using ValSetterFuncPtrT			= void (Class::*)(T);

	template <typename Class, typename T>
	using NonConstRefGetterFuncPtrT = T& (Class::*)();

	template <typename MemberType>
	using GetMemberType				= typename std::decay_t<MemberType>::MemberType;

	template <typename Class, typename T>
	class MetaMember 
	{
		public:

			using ClassType = Class;

			using MemberType = T;

			MetaMember	(char const*					p_name, 
						 MemberPtrT<Class, T>			p_ptr);

			MetaMember	(char const*					p_name, 
						 RefGetterFuncPtrT<Class, T>	p_getterPtr, 
						 RefSetterFuncPtrT<Class, T>	p_setterPtr);

			MetaMember	(char const*					p_name, 
						 ValGetterFuncPtrT<Class, T>	p_getterPtr, 
						 ValSetterFuncPtrT<Class, T>	p_setterPtr);

			MetaMember&				AddNonConstGetter	(NonConstRefGetterFuncPtrT<Class, T>	p_nonConstRefGetterPtr);

			T const&				Get					(Class const&							p_obj)	const;

			T						GetCopy				(Class const&							p_obj)	const;

			T&						GetRef				(Class&									p_obj)	const;

			MemberPtrT<Class, T>	GetPtr				()												const;

			template <typename V, typename = std::enable_if_t<std::is_constructible<T, V>::value>>
			void					Set					(Class&									p_obj, 
														 V&&									value)	const;
			
			template <typename V>
			void					Set					(Class&									p_obj, 
														 V*										value)	const;

			char const*				GetName				() const { return name; }

			bool					HasPtr				() const { return hasMemberPtr; }

			bool					HasGetter			() const { return refGetterPtr || valGetterPtr; }

			bool					HasSetter			() const { return refSetterPtr || valSetterPtr; }

			bool					CanGetConstRef		() const { return hasMemberPtr || refGetterPtr; }

			bool					CanGetRef			() const { return hasMemberPtr || nonConstRefGetterPtr; }

		private:

			char const*							name;

			MemberPtrT<Class, T>				ptr;

			bool								hasMemberPtr;

			RefGetterFuncPtrT<Class, T>			refGetterPtr;

			RefSetterFuncPtrT<Class, T>			refSetterPtr;

			ValGetterFuncPtrT<Class, T>			valGetterPtr;

			ValSetterFuncPtrT<Class, T>			valSetterPtr;

			NonConstRefGetterFuncPtrT<Class, T> nonConstRefGetterPtr;

	}; // !struct MetaMember

	template <typename Class, typename T>
	MetaMember<Class, T>	Member	(char const*					p_name, 
									 T Class::*						p_ptr);

	template <typename Class, typename T>
	MetaMember<Class, T>	Member	(char const*					p_name, 
									 RefGetterFuncPtrT<Class, T>	p_getterPtr, 
									 RefSetterFuncPtrT<Class, T>	p_setterPtr);

	template <typename Class, typename T>
	MetaMember<Class, T>	Member	(char const*					p_name, 
									 ValGetterFuncPtrT<Class, T>	p_getterPtr, 
									 ValSetterFuncPtrT<Class, T>	p_setterPtr);

} //!namespace Reflect

#include "MetaMember.inl"

#endif 
#ifndef __META_MEMBER_INL__
#define __META_MEMBER_INL__

#include "MetaMember.hpp"

template <typename Class, typename T>
Reflect::MetaMember<Class, T>::MetaMember	(const char*					p_name, 
											 MemberPtrT<Class, T>			ptr) :
	name					{ p_name },
	ptr						{ ptr },
	hasMemberPtr			{ true },
	refGetterPtr			{ nullptr },
	refSetterPtr			{ nullptr },
	valGetterPtr			{ nullptr },
	valSetterPtr			{ nullptr },
	nonConstRefGetterPtr	{ nullptr }
{ }

template <typename Class, typename T>
Reflect::MetaMember<Class, T>::MetaMember	(const char*					p_name, 
											 RefGetterFuncPtrT<Class, T>	p_getterPtr, 
											 RefSetterFuncPtrT<Class, T>	p_setterPtr) :
	name					{ p_name },
	ptr						{ nullptr },
	hasMemberPtr			{ false },
	refGetterPtr			{ p_getterPtr },
	refSetterPtr			{ p_setterPtr },
	valGetterPtr			{ nullptr },
	valSetterPtr			{ nullptr },
	nonConstRefGetterPtr	{ nullptr }
{ }

template <typename Class, typename T>
Reflect::MetaMember<Class, T>::MetaMember	(const char*					p_name, 
											 ValGetterFuncPtrT<Class, T>	p_getterPtr, 
											 ValSetterFuncPtrT<Class, T>	p_setterPtr) :
	name					{ p_name },
	ptr						{ nullptr },
	hasMemberPtr			{ false },
	refGetterPtr			{ nullptr },
	refSetterPtr			{ nullptr },
	valGetterPtr			{ p_getterPtr },
	valSetterPtr			{ p_setterPtr },
	nonConstRefGetterPtr	{ nullptr }
{ }


template <typename Class, typename T>
Reflect::MetaMember<Class, T>&	Reflect::MetaMember<Class, T>::AddNonConstGetter	(NonConstRefGetterFuncPtrT<Class, T>	p_nonConstRefGetterPtr)
{
	this->nonConstRefGetterPtr = p_nonConstRefGetterPtr;

	return *this;
}

template <typename Class, typename T>
const T&						Reflect::MetaMember<Class, T>::Get					(const Class&							p_obj) const
{
	if (refGetterPtr) 
	{
		return (p_obj.*refGetterPtr)();
	}
	else if (hasMemberPtr) 
	{
		return p_obj.*ptr;
	}

	throw std::runtime_error("Cannot return const ref to Member: no getter or Member pointer set");
}

template <typename Class, typename T>
T								Reflect::MetaMember<Class, T>::GetCopy				(const Class&							p_obj) const
{
	if (refGetterPtr)
	{
		return (p_obj.*refGetterPtr)();
	}
	else if (valGetterPtr) 
	{
		return (p_obj.*valGetterPtr)();
	}
	else if (hasMemberPtr) 
	{
		return p_obj.*ptr;
	}

	throw std::runtime_error("Cannot return copy of Member: no getter or Member pointer set");
}

template <typename Class, typename T>
T&								Reflect::MetaMember<Class, T>::GetRef				(Class&									p_obj) const
{
	if (nonConstRefGetterPtr) 
	{
		return (p_obj.*nonConstRefGetterPtr)();
	}
	else if (hasMemberPtr) 
	{
		return p_obj.*ptr;
	}

	throw std::runtime_error("Cannot return ref to Member: no getter or Member pointer set");
}

template <typename Class, typename T>
Reflect::MemberPtrT<Class, T>	Reflect::MetaMember<Class, T>::GetPtr				() const 
{
	if (HasPtr()) 
	{
		return ptr;
	}

	throw std::runtime_error("Cannot get pointer to Member: it wasn't set");
}

template<typename Class, typename T>
template <typename V, typename>
void							Reflect::MetaMember<Class, T>::Set					(Class&									p_obj, 
																					 V&&									p_value) const
{
	if (refSetterPtr) 
	{
		(p_obj.*refSetterPtr)(p_value);
	}
	else if (valSetterPtr) 
	{
		(p_obj.*valSetterPtr)(p_value);
	}
	else if (hasMemberPtr)
	{
		p_obj.*ptr = p_value;
	}
	else
		throw std::runtime_error("Cannot access Member: no setter or Member pointer set");
}

template<typename Class, typename T>
template <typename V>
void							Reflect::MetaMember<Class, T>::Set					(Class&									p_obj, 
																					 V*										p_value) const
{
	if (hasMemberPtr)
	{
		p_obj.*ptr = p_value;
	}
	else
		throw std::runtime_error("Cannot access Member: no setter or Member pointer set");
}

template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Reflect::Member										(const char*							p_name, 
																					 T Class::*								p_ptr)
{
	return MetaMember<Class, T>(p_name, p_ptr);
}


template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Reflect::Member										(const char*							p_name, 
																					 RefGetterFuncPtrT<Class, T>			p_getterPtr, 
																					 RefSetterFuncPtrT<Class, T>			p_setterPtr)
{
	return MetaMember<Class, T>(p_name, p_getterPtr, p_setterPtr);
}

template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Reflect::Member										(const char*							p_name, 
																					 ValGetterFuncPtrT<Class, T>			p_getterPtr, 
																					 ValSetterFuncPtrT<Class, T>			p_setterPtr)
{
	return MetaMember<Class, T>(p_name, p_getterPtr, p_setterPtr);
}

template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Member												(const char*							p_name, 
																					 Reflect::RefGetterFuncPtrT<Class, T>	p_getterPtr)
{
	return Reflect::MetaMember<Class, T>(p_name, p_getterPtr, nullptr);
}

template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Member												(const char*							p_name, 
																					 Reflect::ValGetterFuncPtrT<Class, T>	p_getterPtr)
{
	return Reflect::MetaMember<Class, T>(p_name, p_getterPtr, nullptr);
}

template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Member												(const char*							p_name, 
																					 Reflect::RefSetterFuncPtrT<Class, T>	p_setterPtr)
{
	return Reflect::MetaMember<Class, T>(p_name, nullptr, p_setterPtr);
}

template <typename Class, typename T>
Reflect::MetaMember<Class, T>	Member												(const char*							p_name, 
																					 Reflect::ValSetterFuncPtrT<Class, T>	p_setterPtr)
{
	return Reflect::MetaMember<Class, T>(p_name, nullptr, p_setterPtr);
}

#endif // !__META_MEMBER_INL__
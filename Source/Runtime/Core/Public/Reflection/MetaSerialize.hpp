#ifndef __META_SERIALIZE_HPP__
#define __META_SERIALIZE_HPP__

#include "MetaSystem.hpp"
#include "MetaString.hpp"

template <typename T>
void to_json(Json& j, const T& p_obj);

template <typename T>
void from_json(const Json& j, T& p_obj);

namespace Reflect
{
	template <typename Class, typename = std::enable_if_t <Reflect::IsRegistered<Class>()>>
	Json	Serialize		(Class const&						p_obj);

	template <typename Class, typename = std::enable_if_t <!Reflect::IsRegistered<Class>()>, typename = void>
	Json	Serialize		(Class const&						p_obj);

	template <typename Class>
	Json	SerializeBasic	(Class const&						p_obj);

	template <typename T>
	Json	SerializeBasic	(std::vector<T> const&				p_obj);

	template <typename K, typename V>
	Json	SerializeBasic	(std::unordered_map<K, V> const&	p_obj);

	template <typename T, size_t S>
	Json	SerializeBasic	(std::array<T, S> const&			p_obj);

	template <typename Class, typename = std::enable_if_t<Reflect::IsRegistered<Class>()>>
	void	Deserialize		(Class&								p_obj, 
							 Json const&						p_object);

	template <typename Class, typename = std::enable_if_t<!Reflect::IsRegistered<Class>()>, typename = void>
	void	Deserialize		(Class&								p_obj, 
							 Json const&						p_object);

	template <typename T>
	void	Deserialize		(std::vector<T>&					p_obj, 
							 Json const&						p_object);

	template <typename K, typename V>
	void	Deserialize		(std::unordered_map<K, V>&			p_obj, 
							 Json const&						p_object);

	template <typename T, size_t S>
	void	Deserialize		(std::array<T, S>&					p_obj, 
							 Json const&						p_object);

} // !namespace Reflect

#include "MetaSerialize.inl"

#endif // !__META_SERIALIZE_HPP__
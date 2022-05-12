#ifndef __META_SERIALIZE_INL__
#define __META_SERIALIZE_INL__

#include "MetaSerialize.hpp"

template <typename T>
void	to_json		(Json&			p_j, 
					 const T&		p_obj)
{
	p_j = Reflect::Serialize(p_obj);
}

template <typename T>
void	from_json	(const Json&	p_j, 
					 T&				p_obj)
{
	Reflect::Deserialize(p_obj, p_j);
}

namespace Reflect
{
	template <typename Class,typename>
	Json	Serialize		(Class const&						p_obj)
	{
		Json value;

		Reflect::DoForAllMembers<Class>(
			[&p_obj, &value](auto& p_member)
		{
			auto& valueName = value[p_member.GetName()];

			if (p_member.CanGetConstRef()) 
			{
				valueName = p_member.Get(p_obj);
			}
			else if (p_member.HasGetter()) 
			{
				valueName = p_member.GetCopy(p_obj);
			}
		}
		);

		return value;
	}

	template <typename Class, typename, typename>
	Json	Serialize		(Class const&						p_obj)
	{
		return SerializeBasic(p_obj);
	}

	template <typename Class>
	Json	SerializeBasic	(Class const&						p_obj)
	{
		return Json(p_obj);
	}

	template <typename T>
	Json	SerializeBasic	(std::vector<T> const&				p_obj)
	{
		Json value;
		int i = 0;

		for (auto& elem : p_obj) 
		{
			value[i] = elem;
			++i;
		}

		return value;
	}

	template <typename K, typename V>
	Json	SerializeBasic	(std::unordered_map<K, V> const&	p_obj)
	{
		Json value;

		for (auto& pair : p_obj) 
		{
			value.emplace(Reflect::Detail::CastToString(pair.first), pair.second);
		}

		return value;
	}

	template <typename T, size_t S>
	Json	SerializeBasic	(std::array<T, S> const&				p_obj)
	{
		Json value;
		int i = 0;

		for (auto& elem : p_obj) 
		{
			value[i] = elem;
			++i;
		}

		return value;
	}

	template <typename Class>
	Class	Deserialize		(Json const&						p_obj)
	{
		Class c;

		Deserialize(c, p_obj);

		return c;
	}

	template <typename Class, typename>
	void	Deserialize		(Class&								p_obj, 
							 Json const&						p_object)
	{
		if (p_object.is_object()) 
		{
			Reflect::DoForAllMembers<Class>(
			[&p_obj, &p_object](auto& p_member)
			{
				auto& objName = p_object[p_member.GetName()];

				if (!objName.is_null()) 
				{
					using MemberT = Reflect::GetMemberType<decltype(p_member)>;

					if (p_member.HasSetter()) 
					{
						p_member.Set(p_obj, objName.template get<MemberT>());
					}
					else if (p_member.CanGetRef()) 
					{
						p_member.GetRef(p_obj) = objName.template get<MemberT>();
					}
					else 
					{
						throw std::runtime_error("Error: can't Deserialize member because it's read only");
					}
				}
			}
			);
		}
		else
			throw std::runtime_error("Error: can't Deserialize from Json::Json to Class.");
	}

	template <typename Class, typename, typename>
	void	Deserialize		(Class&								p_obj, 
							 Json const&						p_object)
	{
		p_obj = p_object.get<Class>();
	}

	template <typename T>
	void	Deserialize		(std::vector<T>&					p_obj, 
							 Json const&						p_object)
	{
		p_obj.reserve(p_object.size());

		for (auto& elem : p_object) 
		{
			p_obj.push_back(elem);
		}
	}

	template <typename K, typename V>
	void	Deserialize		(std::unordered_map<K, V>&			p_obj, 
							 Json const&						p_object)
	{
		for (auto it = p_object.begin(); it != p_object.end(); ++it) 
		{
			p_obj.emplace(FromString<K>(it.key()), it.value());
		}
	}

	template <typename T, size_t S>
	void	Deserialize		(std::array<T, S>&					p_obj, 
							 Json const&						p_object)
	{
		for (size_t i = 0; i < p_object.size() && i < S; i++)
		{
			p_obj[i] = p_object[i];
		}
	}

} // !namespace Reflect

#endif // !__META_SERIALIZE_INL__
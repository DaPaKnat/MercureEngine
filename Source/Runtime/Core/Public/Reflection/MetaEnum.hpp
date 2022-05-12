#ifndef __META_ENUM_HPP__
#define __META_ENUM_HPP__

#if !defined(ENUM_RANGE)
#  define ENUM_RANGE 256
#endif

namespace Reflect 
{
    static_assert(ENUM_RANGE > 0, "ENUM_RANGE must be positive and greater than zero.");
    static_assert(ENUM_RANGE < std::numeric_limits<int>::max(), "ENUM_RANGE must be less INT_MAX.");

    namespace Detail 
    {
        template <typename E>
        struct MetaEnum final 
        {
            using D = std::decay_t<E>;
            static_assert(std::is_enum_v<D>, "Reflect::Detail::MetaEnum require enum type.");

            using U = std::underlying_type_t<D>;
            using C = std::common_type_t<int, U>;

            static constexpr int    min         = std::max<C>(std::is_signed_v<U> ? -ENUM_RANGE : 0, std::numeric_limits<U>::min());
            static constexpr int    max         = std::min<C>(ENUM_RANGE, std::numeric_limits<U>::max());
            static constexpr auto   sequence    = std::make_integer_sequence<int, max - min + 1>{};

            static constexpr bool   Contains    (int    p_value) noexcept 
            {
                return static_cast<int>(p_value) <= MetaEnum<D>::max && static_cast<int>(p_value) >= MetaEnum<D>::min;
            }
        };

		[[nodiscard]] constexpr bool              IsNameChar	(char   p_c,
																 bool   p_front) noexcept 
        {
            return (!p_front && p_c >= '0' && p_c <= '9') || 
                   (p_c >= 'a' && p_c <= 'z') || 
                   (p_c >= 'A' && p_c <= 'Z') || 
                    p_c == '_';
        }

        template <typename E, E V>
		[[nodiscard]] constexpr std::string_view  NameImpl		() noexcept
        {
            static_assert(std::is_enum_v<E>, "Reflect::Detail::NameImpl require enum type.");

            std::string_view name{ __FUNCSIG__ };
            constexpr auto suffix = sizeof(">(void) noexcept") - 1;

            name.remove_suffix(suffix);

            for (std::size_t i = name.size(); i > 0; --i)
                if (!IsNameChar(name[i - 1], false)) 
                {
                    name.remove_prefix(i);
                    break;
                }

            if (name.length() > 0 && IsNameChar(name.front(), true)) 
                return name;
            else
                return {}; // Enum value does not have name.
        }

        template <typename E, int... I>
		[[nodiscard]] constexpr decltype(auto)    StringsImpl	(std::integer_sequence<int, I...>) noexcept
        {
            static_assert(std::is_enum_v<E>, "Reflect::Detail::StringsImpl require enum type.");

            constexpr std::array<std::string_view, sizeof...(I)> names{ {NameImpl<E, static_cast<E>(I + MetaEnum<E>::min)>()...} };

            return names;
        }

        template <typename E>
		[[nodiscard]] constexpr std::string_view  NameImpl		(int p_value) noexcept
        {
            static_assert(std::is_enum_v<E>, "Reflect::Detail::NameImpl require enum type.");

            constexpr auto names = StringsImpl<E>(MetaEnum<E>::sequence);

            return MetaEnum<E>::Contains(p_value) ? names[p_value - MetaEnum<E>::min] : std::string_view{};
        }

        template <typename E, int... I>
		[[nodiscard]] constexpr decltype(auto)    ValuesImpl	(std::integer_sequence<int, I...>) noexcept
        {
            static_assert(std::is_enum_v<E>, "Reflect::Detail::ValuesImpl require enum type.");

            constexpr int                   n = sizeof...(I);
            constexpr std::array<bool, n>   valid{ {!NameImpl<E, static_cast<E>(I + MetaEnum<E>::min)>().empty()...} };
            constexpr int                   num_valid = ((valid[I] ? 1 : 0) + ...);

            std::array<E, num_valid> enums{};

            for (int i = 0, v = 0; i < n && v < num_valid; ++i) 
                if (valid[i])
                    enums[v++] = static_cast<E>(i + MetaEnum<E>::min);

            return enums;
        }

        template <typename E, std::size_t... I>
		[[nodiscard]] constexpr decltype(auto)    NamesImpl		(std::integer_sequence<std::size_t, I...>) noexcept
        {
            static_assert(std::is_enum_v<E>, "Reflect::Detail::NamesImpl require enum type.");

            constexpr auto                                          enums = ValuesImpl<E>(MetaEnum<E>::sequence);
            constexpr std::array<std::string_view, sizeof...(I)>    names{ {NameImpl<E, enums[I]>()...} };

            return names;
        }

        template <typename E>
		[[nodiscard]] constexpr std::optional<E>  EnumCastImpl	(std::string_view   p_value) noexcept
        {
            static_assert(std::is_enum_v<E>, "Reflect::Detail::EnumCastImpl require enum type.");

            constexpr auto values   = ValuesImpl<E>(MetaEnum<E>::sequence);
            constexpr auto count    = values.size();
            constexpr auto names    = NamesImpl<E>(std::make_index_sequence<count>{});

            for (std::size_t i = 0; i < count; ++i)
                if (names[i] == p_value)
                    return values[i];

            return std::nullopt;
        }

		template<typename T, typename U>
		[[nodiscard]] constexpr bool				IsEnum		(T					p_enum, 
																 U					p_value) noexcept
		{
			if constexpr (std::is_enum_v<decltype(value)>)
			{
				return std::is_same_v<T, U>;
			}
			else if constexpr (std::is_same_v<std::string, decltype(value) ||
				std::is_same_v<char const*, decltype(value)> ||
				std::is_same_v<int, decltype(value)>)
			{
				auto check = Reflect::EnumCast<T>(value);

				return check.has_value();
			}

			return false;
		}

        template<class E>
        using enable_if_enum_t = typename std::enable_if<std::is_enum_v<std::decay_t<E>>>::type;

    } // !namespace Reflect::Detail

    template <typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr std::optional<E>                  EnumCast		(std::string_view           p_value) noexcept
    {
        using D = std::decay_t<E>;

        return Detail::EnumCastImpl<D>(p_value);
    }

    template <typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr std::optional<E>                  EnumCast		(std::underlying_type_t<E>	p_value) noexcept
    {
        using D = std::decay_t<E>;

        if (Detail::NameImpl<D>(static_cast<int>(p_value)).empty())
            return std::nullopt;
        else
            return static_cast<D>(p_value);
    }

    template<typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr E                                 GetEnumValue	(std::size_t                p_i) noexcept
    {
        using D = std::decay_t<E>;
        constexpr auto values = Detail::ValuesImpl<D>(Detail::MetaEnum<D>::sequence);

        return assert(p_i < values.size()), values[p_i];
    }

    template <typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr decltype(auto)                    GetEnumValues	() noexcept
    {
        using D = std::decay_t<E>;
        constexpr auto values = Detail::ValuesImpl<D>(Detail::MetaEnum<D>::sequence);

        return values;
    }

    template <typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr std::size_t                       GetEnumCount	() noexcept
    {
        using D = std::decay_t<E>;
        constexpr auto count = Detail::ValuesImpl<D>(Detail::MetaEnum<D>::sequence).size();

        return count;
    }

    template <typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr std::optional<std::string_view>   GetEnumName	(E                          p_value) noexcept
    {
        using D = std::decay_t<E>;
        const auto name = Detail::NameImpl<D>(static_cast<int>(p_value));

        if (name.empty())
            return std::nullopt;
        else
            return name;
    }

    template <typename E, typename = Detail::enable_if_enum_t<E>>
	[[nodiscard]] constexpr decltype(auto)                    GetEnumNames	() noexcept
    {
        using D = std::decay_t<E>;
        constexpr auto count = Detail::ValuesImpl<D>(Detail::MetaEnum<D>::sequence).size();
        constexpr auto names = Detail::NamesImpl<D>(std::make_index_sequence<count>{});

        return names;
    }

} // !namespace Reflect

template <typename E, typename = Reflect::Detail::enable_if_enum_t<E>>
std::ostream&   operator<<  (std::ostream&      p_os, 
                             E                  p_value)
{
    using D = std::decay_t<E>;

    const auto name = Reflect::Detail::NameImpl<D>(static_cast<int>(p_value));

    if (!name.empty())
        p_os << name;

    return p_os;
}

template <typename E, typename = Reflect::Detail::enable_if_enum_t<E>>
std::ostream&   operator<<  (std::ostream&      p_os, 
                             std::optional<E>   p_value)
{
    using D = std::decay_t<E>;

    if (p_value.has_value())
    {
        const auto name = Reflect::Detail::NameImpl<D>(static_cast<int>(p_value.value()));

        if (!name.empty())
            p_os << name;
    }

    return p_os;
}

#endif // !__META_ENUM_HPP__
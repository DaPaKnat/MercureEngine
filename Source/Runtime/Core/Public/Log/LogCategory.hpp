#ifndef __LOG_CATEGORY_HPP__
#define __LOG_CATEGORY_HPP__

#include "LogLevel.hpp"

struct ENGINE_API LogCategoryBase
{
    // ============================== [Public Constructors and Destructor] ============================== //

        LogCategoryBase     (std::string const& p_categoryName);

        ~LogCategoryBase    ();
    
    // ============================== [Public Local Methods] ============================== //

        std::string GetCategoryName ()  const;

    // ============================== [Public Local Operators] ============================== //

        bool    operator==  (LogCategoryBase const& p_other)    const;

    protected:

    // ============================== [Protected Local Properties] ============================== //

        /** Category name, used during the logging. */
        std::string m_categoryName;

}; // !struct LogCategoryBase

struct ENGINE_API LogCategory : public LogCategoryBase
{
    // ============================== [Public Constructors and Destructor] ============================== //

        LogCategory    (std::string const&  p_categoryName) : 
            LogCategoryBase(p_categoryName)
        {}

}; // !struct LogCategory

namespace std
{
    template<>
    struct hash<LogCategoryBase>
    {
        size_t  operator()  (LogCategoryBase const& p_obj)  const
        {
            return hash<string>()(p_obj.GetCategoryName());
        }
    };
}

#include "LogCategory.inl"

#endif // !__LOG_CATEGORY_HPP__
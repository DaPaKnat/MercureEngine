#ifndef __LOG_CATEGORY_INL__
#define __LOG_CATEGORY_INL__

#include "LogCategory.hpp"

// ============================== [Public Constructors and Destructor] ============================== //

/**
 * Constructor for the LogCategryBase struct.
 */
INLINE LogCategoryBase::LogCategoryBase    (std::string const&  p_categoryName) :
    m_categoryName      { p_categoryName }
{

}

/**
 * Default destructor.
 */
INLINE LogCategoryBase::~LogCategoryBase    ()
{

}

// ============================== [Public Local Methods] ============================== //

/**
 * Return the LogCategory name as a string.
 */
INLINE std::string  LogCategoryBase::GetCategoryName        ()  const
{
    return m_categoryName;
}

// ============================== [Public Local Operators] ============================== //

/**
 * Equality operator.
 */
INLINE bool LogCategoryBase::operator== (LogCategoryBase const& p_other) const
{
    return (m_categoryName == p_other.m_categoryName);
}

#endif // !__LOG_CATEGORY_INL__
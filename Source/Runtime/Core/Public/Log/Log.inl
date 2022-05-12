#ifndef __LOG_INL__
#define __LOG_INL__

#include "Log.hpp"

// ============================== [Private Constructors and Destructor] ============================== //

/**
 * Default constructor
 */
INLINE Logger::Logger() :
    m_logs			{ std::unordered_map<LogCategoryBase, std::ofstream>() },
    m_mutex			{ },
	m_consoleHandle	{ GetStdHandle(STD_OUTPUT_HANDLE) }
{
    m_logs.emplace(LogDefault, std::ofstream("Logs\\LogDefault.log"));
}

/**
 * Default destructor
 */
INLINE  Logger::~Logger()
{

}

#endif // !__LOG_INL__
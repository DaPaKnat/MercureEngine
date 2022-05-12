#include "PCH.hpp"

#include "Log/Log.hpp"

DEFINE_LOG_CATEGORY(LogDefault);

// ============================== [Public Local Methods] ============================== //

Logger&  Logger::Get ()
{
    static Logger logInstance;

    return logInstance;
}

void        Logger::Add             (std::string const&         p_file, 
                                     int const                  p_line, 
                                     std::string const&         p_function, 
                                     LogCategoryBase const&     p_logCategory,
                                     Log::Level const           p_level,
                                     const char*         p_message,
                                     ...)
{
    if (!p_level)
       return;

    // Emplace the LogCategory if needed
    if (m_logs.find(p_logCategory) == m_logs.end())
        m_logs.emplace(p_logCategory, std::ofstream("Logs/" + p_logCategory.GetCategoryName() + ".log"));

    char    message[LOG_MESSAGE_SIZE];
    va_list arglist;
    va_start(arglist, p_message);

    CheckMessage(vsnprintf(message, LOG_MESSAGE_SIZE, p_message, arglist));

    va_end(arglist);

    // If the current logging level is lower than Log::Log, print the message in the console.
    if (p_level < Log::Log && p_level != Log::Fatal)
        PrintInConsole(p_logCategory, p_level, message);
    else if (p_level < Log::Log)
        PrintInConsole(p_logCategory, p_level, message + GetFatalData(p_file, p_line, p_function));

    PrintInFile(p_file, p_line, p_function, p_logCategory, p_level, message);

    if (p_level == Log::Fatal)
        assert(false);
}

/**
 * Print a message in the console.
 */
void        Logger::PrintInConsole  (LogCategoryBase const&     p_logCategory,
                                     Log::Level const           p_level,
                                     std::string const&         p_message)
{
    std::string output  = p_message;
    int         count   = 0;

    while (p_message[count] == '\n' || p_message[count] == '\t')
        count++;

	#if (defined(_WIN64) || defined(_WIN32))
		std::cout << output.substr(0, count) << GetConsoleLabel(p_logCategory.GetCategoryName(), p_level);
		SetConsoleTextAttribute(m_consoleHandle, 15);
		output = output.substr(count);
	#else
		output.insert(count, GetConsoleLabel(p_logCategory.GetCategoryName(), p_level));
	#endif
    
    m_logOutputDelegate.ExecuteIfBound(output + "\n");

    std::cout << output + "\n" << std::flush;
}

/**
 * Print a message in a file.
 */
void        Logger::PrintInFile     (std::string const&         p_file, 
                                     int const                  p_line, 
                                     std::string const&         p_function, 
                                     LogCategoryBase const&     p_logCategory,
                                     Log::Level const           p_level,
                                     std::string const&         p_message)
{
    std::string output  = p_message;
    uint64      count   = 0;

    while (p_message[count] == '\n' || p_message[count] == '\t')
        count++;

    output = output.substr(count);

    output.insert(0, GetFileLabel(p_level));

    count = output.size() - 1;

    while (output[count] == '\n' || output[count] == '\t')
        count--;

    output = output.substr(0, count + 1);

    output += " (In " + p_file.substr(p_file.find_last_of("/\\") + 1) + " at line " + std::to_string(p_line) + " from function " + p_function + ")\n";

    m_logs.at(p_logCategory) << output << std::flush;
}

/** 
 * Check if the vsnprintf parsing didn't failed. Throw a fatal if failed.
 */
void        Logger::CheckMessage    (int const                  p_size)             const
{
    if (p_size < 0 || p_size > LOG_MESSAGE_SIZE)
        assert(false);
}


/**
 * Return a string containg a label for the console output depending of the logging level and the LogCategory.
 */
std::string Logger::GetConsoleLabel (std::string const&         p_logCategoryName,
                                     Log::Level const           p_level)            const
{
    std::string label;
    
    switch (p_level)
    {
        #if !(defined(_WIN64) || defined(_WIN32))

            case Log::Display:
                label = "\x1B[93m[" + p_logCategoryName + ", Display]\033[0m ";
                break;

            case Log::Profiler:
                label = "\x1B[96m[" + p_logCategoryName + ", Profiler]\033[0m ";
                break;

            case Log::Warning:
                label = "\x1B[33m[" + p_logCategoryName + ", Warning]\033[0m ";
                break;

            case Log::Error:
                label = "\x1B[31m[" + p_logCategoryName + ", Error]\033[0m ";
                break;

            case Log::Fatal:
                label = "\n\x1B[31m[" + p_logCategoryName + ", Fatal]\033[0m ";
                break;

            default:
                break;

        #else

            case Log::Display:
                SetConsoleTextAttribute(m_consoleHandle, 11);
                label = "[" + p_logCategoryName + ", Display] ";
                break;

            case Log::Profiler:
                SetConsoleTextAttribute(m_consoleHandle, 13);
                label = "[" + p_logCategoryName + ", Profiler] ";
                break;

            case Log::Warning:
                SetConsoleTextAttribute(m_consoleHandle, 14);
                label = "[" + p_logCategoryName + ", Warning] ";
                break;

            case Log::Error:
                SetConsoleTextAttribute(m_consoleHandle, 12);
                label = "[" + p_logCategoryName + ", Error] ";
                break;

            case Log::Fatal:
                SetConsoleTextAttribute(m_consoleHandle, 12);
                label = "[" + p_logCategoryName + ", Fatal] ";
                break;

            default:
                break;

        #endif
    }

    return label;
}

/**
 * Return a string containg a label for the file output depending of the logging level.
 */
std::string Logger::GetFileLabel    (Log::Level const           p_level)            const
{
    std::string label;

    switch (p_level)
    {
        case Log::Log:
            label = "[Log] ";
            break;

        case Log::Display:
            label = "[Display] ";
            break;

        case Log::Profiler:
            label = "[Profiler] ";
            break;

        case Log::Warning:
            label = "[Warning] ";
            break;

        case Log::Error:
            label = "[Error] ";
            break;

        case Log::Fatal:
            label = "[Fatal] ";
            break;

        default:
            break;
    }

    return label;
}

/**
 * Return informations about a fatal error.
 */
std::string Logger::GetFatalData    (std::string const&         p_file, 
                                     int const                  p_line, 
                                     std::string const&         p_function)             const
{
    return " (In " + p_file.substr(p_file.find_last_of("/\\") + 1) + " at line " + std::to_string(p_line) + " from function " + p_function + ")\n";
}
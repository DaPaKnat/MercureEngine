#ifndef __LOG_HPP__
#define __LOG_HPP__

#include "Delegate/Delegate.hpp"
#include "LogCategory.hpp"

#define LOG_MESSAGE_SIZE 16384

#define xstr(s) str(s)
#define str(s) #s

/**
 * Declare a logging category, use this in a header file.
 */
#define DECLARE_LOG_CATEGORY(CategoryName) \
    extern struct LogCategory##CategoryName : public LogCategory \
    { \
        LogCategory##CategoryName() : \
            LogCategory(str(CategoryName)) \
        {} \
        \
    } CategoryName;

/**
 * Define a logging category, use this in the source file paired with DECLARE_LOG_CATEGORY from the header.
 */
#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;

#if !(defined(__PRETTY_FUNCTION__))
#define __PRETTY_FUNCTION__   __FUNCTION__
#endif

#define LOG(CategoryName, Verbosity, Message, ...) \
    Logger::Get().Add(__FILE__, __LINE__, __PRETTY_FUNCTION__, CategoryName, Log::##Verbosity, Message, ##__VA_ARGS__)


 // ============================== [Delegate Declarations] ============================== //

DECLARE_DELEGATE_OneParam(LogOutput, std::string const&);

template <typename T> using LogCallback = void (T::*)(std::string const&);

// ====================================================================================== //

class ENGINE_API Logger
{
    public:
        
    // ============================== [Public Static Properties] ============================== //

        /**
         * Return the singleton instance and create it if it was not 
         */
    static Logger&  Get ();

    // ============================== [Public Local Methods] ============================== //

        void    Add (std::string const&         p_file, 
                     int const                  p_line, 
                     std::string const&         p_function, 
                     LogCategoryBase const&     p_logCategory,
                     Log::Level const           p_level,
                     char const*        p_message,
                     ...);

        template <typename T, LogCallback<T> function>
        void    RegisterToLogOutput (T* p_object)
        {
            m_logOutputDelegate.Bind<T, function>(p_object); 
        }

    private:

    // ============================== [Private Constructors and Destructor] ============================== //

        Logger  ();

        Logger  (Logger const&  p_copy) = delete;

        Logger  (Logger&&       p_move) = delete;

        ~Logger ();

    // ============================== [Private Local Operators] ============================== //

        Logger& operator=   (Logger const&  p_copy) = delete;

        Logger& operator=   (Logger&&       p_move) = delete;

    // ============================== [Private Local Properties] ============================== //
        
        /** */
        std::unordered_map<LogCategoryBase, std::ofstream>  m_logs;

        /** Mutex used for thread safety */
        std::mutex                                          m_mutex;

		/** Console handle, note this is only usable under Windows */
		HANDLE												m_consoleHandle;

        /** Log output delegate for Editor */
        LogOutput                                           m_logOutputDelegate;

    // ============================== [Private Local Methods] ============================== //

        void        PrintInConsole  (LogCategoryBase const& p_logCategory,
                                     Log::Level const       p_level,
                                     std::string const&     p_message);

        void        PrintInFile     (std::string const&     p_file, 
                                     int const              p_line, 
                                     std::string const&     p_function, 
                                     LogCategoryBase const& p_logCategory,
                                     Log::Level const       p_level,
                                     std::string const&     p_message);

        void        CheckMessage    (int const              p_size)     const;

        std::string GetConsoleLabel (std::string const&     p_logCategoryName,
                                     Log::Level const       p_level)    const;

        std::string GetFileLabel    (Log::Level const       p_level)    const;

        std::string GetFatalData    (std::string const&     p_file, 
                                     int const              p_line, 
                                     std::string const&     p_function) const;
};

DECLARE_LOG_CATEGORY(LogDefault);

#include "Log.inl"

#endif // !__LOG_HPP__
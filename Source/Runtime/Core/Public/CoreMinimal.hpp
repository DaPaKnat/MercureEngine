#ifndef __CORE_MINIMAL_HPP__
#define __CORE_MINIMAL_HPP__

#include "Delegate/Delegate.hpp"

#include "Log/Log.hpp"

#include "Profiler/Profiler.hpp"

#include "Reflection/Reflection.hpp"

#include "Helpers/NonCopyable.hpp"
#include "Helpers/UniqueObject.hpp"
#include "Mathematic/Math.hpp"
#include "Profiler/Profiler.hpp"
#include "Templates/Templates.hpp"

#include "CoreObject.hpp"

DECLARE_LOG_CATEGORY(LogGUI);
DECLARE_LOG_CATEGORY(LogCore);
DECLARE_LOG_CATEGORY(LogEngine);
DECLARE_LOG_CATEGORY(LogEntity);
DECLARE_LOG_CATEGORY(LogEntityComponent);
DECLARE_LOG_CATEGORY(LogThreadPool);
DECLARE_LOG_CATEGORY(LogApplication);
DECLARE_LOG_CATEGORY(LogAssetManager);
DECLARE_LOG_CATEGORY(LogRenderer);
DECLARE_LOG_CATEGORY(LogRHI);
DECLARE_LOG_CATEGORY(LogPhysics);
DECLARE_LOG_CATEGORY(LogWorld);
DECLARE_LOG_CATEGORY(LogLevel);

#endif // !__CORE_MINIMAL_HPP__
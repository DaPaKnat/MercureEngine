#ifndef __RHI_HPP__
#define __RHI_HPP__

#if defined(VULKAN)
    #include "Vulkan/RHI.hpp"
#else
    #error UNKNOWN GRAPHICS API
#endif

#endif // !__RHI_HPP__
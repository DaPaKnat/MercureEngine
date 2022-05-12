#include "PCH.hpp"
#include "RHI.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

Instance::Instance  ()
{
    m_requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

    // Enables surface extensions depending on OS.
    #if defined(VK_USE_PLATFORM_WIN32_KHR)
    
    m_requiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    
    #endif
    
    // Enables validation layers in debug mode.
    #ifndef NDEBUG
        
    m_requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    m_requiredLayers    .push_back("VK_LAYER_KHRONOS_validation");

    #endif

    CheckInstanceExtensions();
    CheckInstanceLayers    ();
    CreateInstance         ();

    // Creates a debug messenger in debug mode.
    #ifndef NDEBUG
    
    Debug::SetupDebugging(m_instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

    #endif

    LOG(LogRHI, Display, "Created : Instance");
}

Instance::~Instance ()
{
    // Destroys the debug messenger in debug mode.
    #ifndef NDEBUG
        
    Debug::FreeDebugging(m_instance);
    
    #endif

    vkDestroyInstance(m_instance, nullptr);

    LOG(LogRHI, Display, "Destroyed : Instance");
}

// ============================== [Private Local Methods] ============================== //

void    Instance::CheckInstanceExtensions   () const noexcept
{
    // Returns up to requested number of global extension properties.
    uint32 extensionCount = 0u;

    VK_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));

    std::vector<VkExtensionProperties> supportedExtensions(extensionCount);

    VK_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data()));

    // Checks if all the required extensions are supported.
    std::set<std::string> requiredExtensions(m_requiredExtensions.cbegin(), m_requiredExtensions.cend());

    // Removes the matching extensions.
    for (auto const& extension : supportedExtensions)
        requiredExtensions.erase(extension.extensionName);

    // In case some extensions could not be found, enumerates them.
    if (!requiredExtensions.empty())
    {
        for (auto const& extension : requiredExtensions)
            LOG(LogRHI, Error, "Missing instance extension : %s", extension.c_str());

        LOG(LogRHI, Fatal, "One or more instance extensions could not be found");
    }
}

void    Instance::CheckInstanceLayers       () const noexcept
{
    // Returns up to requested number of global layer properties
    uint32 layerCount = 0u;

    VK_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    std::vector<VkLayerProperties> supportedLayers(layerCount);

    VK_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data()));

    // Checks if all the required layers are supported.
    std::set<std::string> requiredLayers(m_requiredLayers.cbegin(), m_requiredLayers.cend());

    // Removes the matching layers.
    for (auto const& layer : supportedLayers)
        requiredLayers.erase(layer.layerName);

    // In case some layers could not be found, enumerates them.
    if (!requiredLayers.empty())
    {
        for (auto const& layer : requiredLayers)
            LOG(LogRHI, Error, "Missing instance layer : %s", layer);

        LOG(LogRHI, Fatal, "One or more instance layers could not be found");
    }
}

void    Instance::CreateInstance            () noexcept
{
    // Structure specifying application info.
    VkApplicationInfo applicationInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };

    applicationInfo.pApplicationName   = ENGINE_NAME;
    applicationInfo.applicationVersion = ENGINE_VERSION;
    applicationInfo.pEngineName        = ENGINE_NAME;
    applicationInfo.engineVersion      = ENGINE_VERSION;
    applicationInfo.apiVersion         = VK_API_VERSION_1_1;

    // Structure specifying parameters of a newly created instance.
    VkInstanceCreateInfo instanceCI = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };

    instanceCI.pApplicationInfo        = &applicationInfo;
    instanceCI.enabledLayerCount       = static_cast<uint32>(m_requiredLayers.size());
    instanceCI.ppEnabledLayerNames     = m_requiredLayers.data();
    instanceCI.enabledExtensionCount   = static_cast<uint32>(m_requiredExtensions.size());
    instanceCI.ppEnabledExtensionNames = m_requiredExtensions.data();

    VK_CHECK_RESULT(vkCreateInstance(&instanceCI, nullptr, &m_instance));
}
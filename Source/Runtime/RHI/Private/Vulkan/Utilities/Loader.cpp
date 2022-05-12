#include "PCH.hpp"

#include "Vulkan/Utilities/Loader.hpp"
#include "Vulkan/Utilities/Debug.hpp"
#include "Vulkan/Utilities/Function.hpp"

#if defined(_WIN64) || defined(_WIN32)

#include "shellapi.h"

#pragma comment(lib, "Version.lib")

// ============================== [Global DLL handle] ============================== //

/** Handle to the library instance. */
static HMODULE Module = NULL;

// ============================== [Private Static Methods] ============================== //

void    Loader::Free        () noexcept
{
    if (Module == NULL)
        return;

    if (FreeLibrary(Module) == TRUE)
        LOG(LogRHI, Warning, "\nvulkan-1.dll unloaded");
    else
        LOG(LogRHI, Error,   "\nvulkan-1.dll could not be unloaded");

    Module = NULL;
}

bool    Loader::IsLoaded    () noexcept
{
    return Module != NULL;
}

bool    Loader::Load        () noexcept
{
    Module = LoadLibraryA("vulkan-1.dll");

    // In case the library could not be loaded, makes the user install it.
    if (Module == NULL)
    {
        // Retries to load the library one more time after installation.
        if (Install())
        {
            Module = LoadLibraryA("vulkan-1.dll");

            return Check();
        }

        else
            return false;
    }

    else
        return Check();
}

// ============================== [Private Static Methods] ============================== //

bool    Loader::Check                   () noexcept
{
    DWORD handle = 0ul;
    DWORD length = 0ul;

    // Determines whether the operating system can retrieve version information for the module.
    length = GetFileVersionInfoSizeA("vulkan-1.dll", &handle);

    if (length == 0ul)
    {
        LOG(LogRHI, Error, "Could not retrieve version information for vulkan-1.dll");

        return false;
    }

    ANSICHAR* buffer = new ANSICHAR[length];

    // Retrieves version information for the vulkan library.
    if (GetFileVersionInfoA("vulkan-1.dll", handle, length, &buffer[0]) == FALSE)
    {
        LOG(LogRHI, Error, "Could not retrieve version information for vulkan-1.dll");

        return false;
    }

    VS_FIXEDFILEINFO* info;

    length = sizeof(VS_FIXEDFILEINFO);

    // Retrieves the specified version's information from the version-information resource.
    if (VerQueryValueA(&buffer[0], "\\", (LPVOID*)&info, (PUINT)&length) == FALSE)
    {
        LOG(LogRHI, Error, "Could not query value from version buffer");

        return false;
    }

    DWORD major    = info->dwProductVersionMS >> 16;
    DWORD minor    = info->dwFileVersionMS     & 0xFFFF;
    DWORD patch    = info->dwFileVersionLS    >> 16;
    DWORD revision = info->dwFileVersionLS     & 0xFFFF;

    DWORD version  = ((major) << 22) | ((minor) << 12);

    // Deletes the buffer after information has been retrieved.
    delete buffer;

    // Checks if the versions are compatible.
    if (version < VK_API_VERSION_1_1)
    {
        // In case the versions are not compatible, makes the user install the required one.
        if (Install())
        {
            Free();

            Module = LoadLibraryA("vulkan-1.dll");

            if (Module == NULL)
            {
                LOG(LogRHI, Error, "The newly installed library could not be loaded");

                return false;
            }
        }

        else
        {
            Free();

            LOG(LogRHI, Error, "Could not install the required version of the library");

            return false;
        }
    }

    LOG(LogRHI, Warning, "vulkan-1.dll loaded : version %ld.%ld.%ld.%ld\n", major, minor, patch, revision);

    return true;
}

bool    Loader::Install                 () noexcept
{
    ANSICHAR path[MAX_PATH];

    // Sets the installation's parameters.
    if (GetCurrentDirectoryA(MAX_PATH, path) == 0ul)
        return false;

    SHELLEXECUTEINFOA info = {};

    info.cbSize       = sizeof(info);
    info.fMask        = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd         = NULL;
    info.lpVerb       = "open";
    info.lpFile       = "VulkanRuntimeInstaller\\VulkanRT-1.1.92.1-Installer.exe";
    info.lpParameters = NULL;
    info.lpDirectory  = path;
    info.nShow        = SW_SHOWNORMAL;

    // Loops until the installation is done.
    if (ShellExecuteExA(&info) == TRUE)
    {
        WaitForSingleObject(info.hProcess, INFINITE);
        CloseHandle        (info.hProcess);

        return true;
    }

    return false;
}

#endif
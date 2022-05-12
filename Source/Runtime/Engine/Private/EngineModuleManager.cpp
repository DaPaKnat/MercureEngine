#include "PCH.hpp"
#include "RHI.hpp"
#include "Renderer.hpp"
#include "ThreadPool.hpp"
#include "Application.hpp"
#include "AssetManager.hpp"
#include "ObjectManager.hpp"
#include "EngineModuleManager.hpp"

#include "PhysicsEngine/PhysicsEngine.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EngineModuleManager::EngineModuleManager    ()
{
    Add<ThreadPool>   ();
    Add<Application>  ();
    Add<AssetManager> ();
    Add<RHI>          ();
    Add<Renderer>     ();
    Add<PhysicsEngine>();
    Add<ObjectManager>();
}

EngineModuleManager::~EngineModuleManager   ()
{
    Remove<ObjectManager>();
    Remove<PhysicsEngine>();
    Remove<Renderer>     ();
    Remove<RHI>          ();
    Remove<AssetManager> ();
    Remove<Application>  ();
    Remove<ThreadPool>   ();
}

// ============================== [Private Local Methods] ============================== //

/**
 * Initializes the Engine modules in the correct order.
 */
void    EngineModuleManager::Initialize (EngineKey const& p_passkey) noexcept
{
    Get<ThreadPool>   ().Initialize(p_passkey);
    Get<Application>  ().Initialize(p_passkey);
    Get<AssetManager> ().Initialize(p_passkey);
    Get<RHI>          ().Initialize(p_passkey);
    Get<Renderer>     ().Initialize(p_passkey);
    Get<PhysicsEngine>().Initialize(p_passkey);
    Get<ObjectManager>().Initialize(p_passkey);
}

/**
 * Shutdowns the Engine modules in the correct order.
 */
void    EngineModuleManager::Shutdown   (EngineKey const& p_passkey) noexcept
{
    Get<ObjectManager>().Shutdown(p_passkey);
    Get<PhysicsEngine>().Shutdown(p_passkey);
    Get<Renderer>     ().Shutdown(p_passkey);
    Get<AssetManager> ().Shutdown(p_passkey);
    Get<ThreadPool>   ().Shutdown(p_passkey);
    Get<RHI>          ().Shutdown(p_passkey);
    Get<Application>  ().Shutdown(p_passkey);
}
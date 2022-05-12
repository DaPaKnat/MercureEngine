#include "PCH.hpp"
#include "World.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "Application.hpp"
#include "AssetManager.hpp"
#include "GameUserSettings.hpp"
#include "EngineModuleManager.hpp"

#include "Camera/CameraComponent.hpp"

#include "PhysicsEngine/PhysicsEngine.hpp"

#include "Components/StaticMeshComponent.hpp"
#include "Components/SphereComponent.hpp"
#include "Components/BoxComponent.hpp"

// ============================== [Global Variable] ============================== //

Engine* GEngine = nullptr;

// ============================== [Public Constructor and Destructor] ============================== //

Entity*                 e1;
SphereComponent*        sphere1;

Engine::Engine  () :
    m_running          { true },
    m_moduleManager    { std::make_unique<EngineModuleManager>() },
    m_gameUserSettings { std::make_unique<GameUserSettings>   () }
{
    // Stores this instance as a global variable.
    GEngine = this;

    // Loads the settings.
    m_gameUserSettings->Load(m_passkey);
    
    // Initializes the Engine's modules.
    m_moduleManager->Initialize(m_passkey);

    // Initializes the world.
    m_world = std::make_unique<World>();

    m_world->InitializeLevel();
}

Engine::~Engine ()
{
    // Shutdowns the world.
    m_world.reset();

    // Shutdowns the Engine's modules. 
    m_moduleManager->Shutdown(m_passkey);

    // Saves the settings.
    m_gameUserSettings->Save(m_passkey);

    // Invalidates the global variable storing this instance.
    GEngine = nullptr;
}

// ============================== [Public Local Methods] ============================== //

void    Engine::Run             ()
{
    // Launches the renderer on a standalone thread.
    m_moduleManager->Get<ThreadPool>().SubmitTask([&] () { m_moduleManager->Get<Renderer>().Update(m_passkey); });

    Application& application = m_moduleManager->Get<Application>();
    Renderer&    renderer    = m_moduleManager->Get<Renderer>   ();

    auto  lastTime    = std::chrono::high_resolution_clock::now();
    auto  currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime   = 0.0f;

    while (m_running.load(std::memory_order_acquire))
    {
        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime   = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime    = currentTime;
        
        application.Update(m_passkey);
        
        m_world->Tick(deltaTime);

        if (m_world->GetCurrentLevel())
            renderer.Render(m_passkey, *m_world->GetCurrentLevel());
    }
}

float g_TestTime = 0;

void    Engine::Tick            (float p_deltaTime)
{
    Application::Get().Update(m_passkey);

    g_TestTime += p_deltaTime;

    m_world->Tick(p_deltaTime);

    if (m_world->GetCurrentLevel())
        Renderer::Get().Render(m_passkey, *m_world->GetCurrentLevel());
}

bool    Engine::IsRunning       () const noexcept
{
    return m_running.load(std::memory_order_acquire);
}

void    Engine::RequestShutdown () noexcept
{
    m_running.store(false, std::memory_order_release);
}

World*  Engine::GetWorld        () noexcept
{
    return m_world.get();
}
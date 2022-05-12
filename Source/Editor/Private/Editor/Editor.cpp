#include "PCH.hpp"
#include "World.hpp"
#include "Engine.hpp"
#include "World.hpp"
#include "Editor.hpp"
#include "AssetManager.hpp"

#include "Log/EditorLog.hpp"
#include "Menu/EditorMenu.hpp"
#include "Game/EditorGame.hpp"
#include "Explorer/EditorExplorer.hpp"
#include "Hierarchy/EditorHierarchy.hpp"
#include "Inspector/EditorInspector.hpp"

#include "PhysicsEngine/PhysicsEngine.hpp"

#include "Vulkan/Object/Fence.hpp"

DEFINE_LOG_CATEGORY(LogEditor);

// ============================== [Static Variables] ============================== //

static std::array<bool, 5> MouseJustPressed = { false, false, false, false, false };

// ============================== [Public Constructor and Destructor] ============================== //

Editor::Editor  () :
    m_engine        { std::make_unique<Engine>()   },
    m_context       { ImGui::CreateContext()       },
    m_windows       { std::vector<EditorWindow*>() },
    m_editorCamera  { EditorCamera() }
{
    ImGui::SetCurrentContext(m_context);

    AssetManager& assetManager = AssetManager::Get();

    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/gui.vert.glsl").c_str(), "Default/Shaders/");
    assetManager.FindOrAdd((std::string(SOURCE_DIRECTORY) + "Default/Shaders/gui.frag.glsl").c_str(), "Default/Shaders/");

    m_frames.resize(RHI::Get().GetSwapchain()->GetImageCount());

    SetupImGui              ();
    SetupLayout             ();
    SetupFont               ();
    SetupRenderPass         ();
    SetupDescriptorPool     ();
    SetupDescriptorSetLayout();
    SetupDescriptorSets     ();
    SetupPipelineLayout     ();
    SetupPipeline           ();
    SetupFrames             ();

    World* world = GetWorld();

    if (world != nullptr)
    {
        world->SetPhysicsScene(PhysicsEngine::Get().GetPhysicsScene());

        m_editorCamera.SetLevelCamera(world->GetMainCamera());

        world->SetMainCamera(m_editorCamera.GetEditorCamera());
    }
}

Editor::~Editor ()
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    device->WaitIdle();

    allocator->DestroyImage(m_font);

    vkDestroyImageView          (device->GetLogicalDevice(), m_font.imageView,      nullptr);
    vkDestroyRenderPass         (device->GetLogicalDevice(), m_renderPass,          nullptr);
    vkDestroyDescriptorPool     (device->GetLogicalDevice(), m_descriptorPool,      nullptr);
    vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout     (device->GetLogicalDevice(), m_pipelineLayout,      nullptr);
    vkDestroyPipeline           (device->GetLogicalDevice(), m_pipeline,            nullptr);

    for (size_t i = 0; i < m_frames.size(); ++i)
    {
        device->GetGraphicsCommandPool()->FreeCommandBuffer(m_frames[i].commandBuffer);

        if (m_frames[i].vertexBuffer.handle) allocator->DestroyBuffer(m_frames[i].vertexBuffer);
        if (m_frames[i].indexBuffer .handle) allocator->DestroyBuffer(m_frames[i].indexBuffer);

        vkDestroyFence      (device->GetLogicalDevice(), m_frames[i].fence,                   nullptr);
        vkDestroySemaphore  (device->GetLogicalDevice(), m_frames[i].imageAvailableSemaphore, nullptr);
        vkDestroySemaphore  (device->GetLogicalDevice(), m_frames[i].renderFinishedSemaphore, nullptr);
        vkDestroyFramebuffer(device->GetLogicalDevice(), m_frames[i].framebuffer,             nullptr);
    }

    ImGui::DestroyContext(m_context);

    if (World* world = GetWorld())
        world->SaveAll();

    m_engine.reset();

    for (auto window : m_windows)
    {
        delete window;

        window = nullptr;
    }
}

// ============================== [Public Local Methods] ============================== //

void                        Editor::Run     ()
{
    /*Level* level = m_engine->GetWorld()->CreateLevel("Game", true);

    {
        for (int32 i = 0; i < 10; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Floor>("Floor_" + std::to_string(i) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Floor>("Floor_" + std::to_string(i) + ".1");

            e1->SetWorldLocation(Vector3(-2.5f, -3.0f, i * 5.0f));
            e2->SetWorldLocation(Vector3(2.5f, -3.0f, i * 5.0f));

            e1->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
            e2->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
        }

        for (int32 i = 0; i < 10; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i) + ".1");
            Entity* e3 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i) + ".2");
            Entity* e4 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i) + ".3");

            e1->SetWorldLocation(Vector3(-5.5f, 5.0f, i * 5.0f));
            e2->SetWorldLocation(Vector3(-5.5f, 0.0f, i * 5.0f));
            e3->SetWorldLocation(Vector3(5.5f, 0.0f, i * 5.0f));
            e4->SetWorldLocation(Vector3(5.5f, 5.0f, i * 5.0f));

            e1->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e2->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e3->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e4->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
        }
    }

    {
        for (int32 i = 0; i < 10; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Roof>("Roof_" + std::to_string(i) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Roof>("Roof_" + std::to_string(i) + ".1");

            e1->SetWorldLocation(Vector3(-2.5f, 8.125f, i * 5.0f));
            e2->SetWorldLocation(Vector3(2.5f, 8.125f, i * 5.0f));

            e1->SetWorldScale(Vector3(5.0f, 1.25f, 5.0f));
            e2->SetWorldScale(Vector3(5.0f, 1.25f, 5.0f));
        }

        for (int32 i = 0; i < 4; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".1");
            Entity* e3 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".2");
            Entity* e4 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".3");
            Entity* e5 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".4");
            Entity* e6 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".5");
            Entity* e7 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".6");
            Entity* e8 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 10) + ".7");

            e1->SetWorldLocation(Vector3(-5.5f, i * 10.0f + 5.0f, 10 * 5.0f));
            e2->SetWorldLocation(Vector3(-5.5f, i * 10.0f, 10 * 5.0f));
            e3->SetWorldLocation(Vector3(5.5f, i * 10.0f + 5.0f, 10 * 5.0f));
            e4->SetWorldLocation(Vector3(5.5f, i * 10.0f, 10 * 5.0f));
            e5->SetWorldLocation(Vector3(-5.5f, i * 10.0f + 5.0f, 11 * 5.0f));
            e6->SetWorldLocation(Vector3(-5.5f, i * 10.0f, 11 * 5.0f));
            e7->SetWorldLocation(Vector3(5.5f, i * 10.0f + 5.0f, 11 * 5.0f));
            e8->SetWorldLocation(Vector3(5.5f, i * 10.0f, 11 * 5.0f));

            e1->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e2->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e3->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e4->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e5->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e6->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e7->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e8->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
        }

        for (int32 i = 10; i < 12; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Roof>("Roof_" + std::to_string(i) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Roof>("Roof_" + std::to_string(i) + ".1");

            e1->SetWorldLocation(Vector3(-2.5f, 38.0f, i * 5.0f));
            e2->SetWorldLocation(Vector3(2.5f, 38.0f, i * 5.0f));

            e1->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
            e2->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
        }

        MovingPlatform* p = m_engine->GetWorld()->SpawnEntity<MovingPlatform>("Platform_0");

        p->SetWorldLocation(Vector3(0.0f, -3.0f, 52.5f));
        p->SetWorldScale(Vector3(10.0f, 1.0f, 10.0f));
        p->SetDirection(Vector3::Up);
        p->SetDistance(30.0f);
        p->SetSpeed(0.2f); 
        
        for (int32 i = 0; i < 6; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 14) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 14) + ".1");

            e1->SetWorldLocation(Vector3(2.5f, i * 5.0f + 11.25f, 47.0f));
            e1->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e2->SetWorldLocation(Vector3(-2.5f, i * 5.0f + 11.25f, 47.0f));
            e2->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));

            Entity* e10 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 14) + ".3");
            Entity* e12 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 14) + ".4");

            e10->SetWorldLocation(Vector3(-2.5f, i * 5.0f - 2.5f, 58.0f));
            e12->SetWorldLocation(Vector3(2.5f, i * 5.0f - 2.5f, 58.0f));

            e10->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e12->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
        }
    }

    {
        for (int32 i = 0; i < 5; ++i)
        {
            for (int32 j = 0; j < 5; ++j)
            {
                Entity* e1 = m_engine->GetWorld()->SpawnEntity<Floor>("Floor_" + std::to_string(i+10) + "." + std::to_string(j));
                Entity* e2 = m_engine->GetWorld()->SpawnEntity<Floor>("Floor_" + std::to_string(i+10) + "." + std::to_string(j));

                e1->SetWorldLocation(Vector3(-2.5f + j * -5.0f, 25.5f, 60.0f + i * 5.0f));
                e2->SetWorldLocation(Vector3(2.5f + j * 5.0f, 25.5f, 60.0f + i * 5.0f));

                e1->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
                e2->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));



                Entity* e3 = m_engine->GetWorld()->SpawnEntity<Roof>("Roof_" + std::to_string(i + 10) + "." + std::to_string(j));
                Entity* e4 = m_engine->GetWorld()->SpawnEntity<Roof>("Roof_" + std::to_string(i + 10) + "." + std::to_string(j));

                e3->SetWorldLocation(Vector3(-2.5f + j * -5.0f, 38.0f, 60.0f + i * 5.0f));
                e4->SetWorldLocation(Vector3(2.5f + j * 5.0f, 38.0f, 60.0f + i * 5.0f));

                e3->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
                e4->SetWorldScale(Vector3(5.0f, 1.0f, 5.0f));
            }
        }

        for (int32 i = 0; i < 4; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".1");
            Entity* e3 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".2");
            Entity* e4 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".3");
            Entity* e5 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".4");
            Entity* e6 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".5");

            e1->SetWorldLocation(Vector3(-7.5f - 5.0f*i, 35.0f, 58.0f));
            e2->SetWorldLocation(Vector3(7.5f + 5.0f*i, 35.0f, 58.0f));
            e3->SetWorldLocation(Vector3(-7.5f - 5.0f*i, 30.0f, 58.0f));
            e4->SetWorldLocation(Vector3(7.5f + 5.0f*i, 30.0f, 58.0f));
            e5->SetWorldLocation(Vector3(-7.5f - 5.0f*i, 25.0f, 58.0f));
            e6->SetWorldLocation(Vector3(7.5f + 5.0f*i, 25.0f, 58.0f));

            e1->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e2->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e3->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e4->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e5->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e6->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
        }
        
        for (int32 i = 0; i < 5; ++i)
        {
            Entity* e1 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".0");
            Entity* e2 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".1");
            Entity* e3 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".2");
            Entity* e4 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".3");
            Entity* e5 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".4");
            Entity* e6 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".5");

            e1->SetWorldLocation(Vector3(-2.5f - 5.0f*i, 35.0f, 25.0f + 58.0f));
            e2->SetWorldLocation(Vector3(2.5f + 5.0f*i, 35.0f, 25.0f + 58.0f));
            e3->SetWorldLocation(Vector3(-2.5f - 5.0f*i, 30.0f, 25.0f + 58.0f));
            e4->SetWorldLocation(Vector3(2.5f + 5.0f*i, 30.0f, 25.0f + 58.0f));
            e5->SetWorldLocation(Vector3(-2.5f - 5.0f*i, 25.0f, 25.0f + 58.0f));
            e6->SetWorldLocation(Vector3(2.5f + 5.0f*i, 25.0f, 25.0f + 58.0f));

            e1->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e2->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e3->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e4->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e5->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));
            e6->SetWorldScale(Vector3(5.0f, 5.0f, 1.0f));

            Entity* e7 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".6");
            Entity* e8 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".7");
            Entity* e9 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".8");

            e7->SetWorldLocation(Vector3(25.5f, 35.0f, 5.0f*i + 60.0f));
            e8->SetWorldLocation(Vector3(25.5f, 30.0f, 5.0f*i + 60.0f));
            e9->SetWorldLocation(Vector3(25.5f, 25.0f, 5.0f*i + 60.0f));

            e7->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e8->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e9->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));

            Entity* e13 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".9");
            Entity* e14 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".10");
            Entity* e15 = m_engine->GetWorld()->SpawnEntity<Wall>("Wall_" + std::to_string(i + 20) + ".11");

            e13->SetWorldLocation(Vector3(-25.5f, 35.0f, 5.0f*i + 60.0f));
            e14->SetWorldLocation(Vector3(-25.5f, 30.0f, 5.0f*i + 60.0f));
            e15->SetWorldLocation(Vector3(-25.5f, 25.0f, 5.0f*i + 60.0f));

            e13->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e14->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
            e15->SetWorldScale(Vector3(1.0f, 5.0f, 5.0f));
        }
    }*/

    auto lastTime    = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    ImGuiIO&         IO         = ImGui::GetIO        ();
    ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();

    while (GEngine->IsRunning())
    {
        ImGui::SetCurrentContext(m_context);

        currentTime  = std::chrono::high_resolution_clock::now();
        IO.DeltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime     = currentTime;

        if (IO.DeltaTime > 0.25f)
            IO.DeltaTime = 0.25f;

        m_engine->Tick(IO.DeltaTime);

        m_editorCamera.Tick(IO.DeltaTime);
       
        FocusOnEntity();

        UpdateWindow            (IO, platformIO);
        UpdateMousePosAndButtons(IO, platformIO);
        UpdateMouseCursor       (IO, platformIO);

        ImGui::NewFrame         ();
        UpdateLayout            ();
        ImGui::Render           ();

        Draw();
    }
}

void                        Editor::Close   ()  noexcept
{
    if (m_engine != nullptr)
    {
        if (!m_engine->GetWorld()->HasBegunPlay())
            m_engine->RequestShutdown();
    }
}

World*                      Editor::GetWorld            ()                              noexcept
{
    return m_engine->GetWorld();
}

void                        Editor::TooglePlay          ()                              noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        if (world->HasBegunPlay())
        {
            world->EndPlay();

            world->GetCurrentLevel()->Load(world->GetCurrentLevel()->GetName());

            world->SetMainCamera(m_editorCamera.GetEditorCamera());
        }
        else
        {
            if (world->GetCurrentLevel())
            {
                world->GetCurrentLevel()->Save();

                world->SetMainCamera(m_editorCamera.GetLevelCamera());

                world->BeginPlay();
            }
        }
    }
}

void                        Editor::CreateLevel         (std::string const& p_level)    noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        if (world->GetCurrentLevel())
            world->SaveCurrentLevel();

        world->CreateLevel(p_level, true);
    }
}

void                        Editor::OpenLevel           (std::string const& p_level)    noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        if (world->SaveCurrentLevel())
            world->OpenLevel(p_level);
    }
}

void                        Editor::LoadLevel           (std::string const& p_level)    noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        world->LoadLevel(p_level);
    }
}

void                        Editor::ImportLevel         (std::string const& p_level)    noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        Level* level = ObjectManager::Get().CreateObject<Level>(p_level);

        if (level)
        {
            world->SetCurrentLevel(level->Load(p_level));

            level->SetWorld(world);

            std::vector<CameraComponent*> cam   { level->GetComponents<CameraComponent>() };

            if (!cam.empty())
                m_editorCamera.SetLevelCamera(cam[0]);
        }
    }
}

void                        Editor::SaveLevel           ()                              noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        world->SaveCurrentLevel();
    }
}

Entity* const*              Editor::GetLevelEntities    ()                              noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        if (world->GetCurrentLevel())
            return world->GetCurrentLevel()->GetEntities().data();
    }

    return nullptr;
}

size_t                      Editor::GetLevelEntityCount ()                              noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        if (world->GetCurrentLevel())
            return world->GetCurrentLevel()->GetEntityCount();
    }

    return 0;
}

Entity*                     Editor::GetSelectedEntity   ()                              noexcept
{
    for (auto window : m_windows)
    {
        EditorHierarchy* hierarchy = dynamic_cast<EditorHierarchy*>(window);

        if (hierarchy != nullptr)
        {
            return hierarchy->GetSelectedEntity();
        }
    }

    return nullptr;
}

std::vector<std::string>    Editor::GetLevelNames       ()  noexcept
{
    std::vector<std::string> names;

    std::vector<Level*> levels  { ObjectManager::Get().GetObjectsOfType<Level>() };

    for (auto const& level : levels)
        names.push_back(level->GetName());

    return names;
}

EditorCamera&               Editor::GetEditorCamera     ()  noexcept
{
    return m_editorCamera;
}

bool                        Editor::IsLevelOpen         (std::string_view   p_name) const noexcept
{
    std::vector<Level*> levels = ObjectManager::Get().GetObjectsOfType<Level>();

    for (auto const& level : levels)
    {
        if (level->GetName() == p_name)
            return true;
    }

    return false;
}

bool                        Editor::HasAnyLevelOpen     ()  const noexcept
{
    return !ObjectManager::Get().GetObjectsOfType<Level>().empty();
}

// ============================== [Private Static Methods] ============================== //

ANSICHAR const* Editor::ImGuiGetClipboardText   (void* p_userData) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(p_userData))
    {
        return window->GetClipboardString();
    }

    else
        LOG(LogEditor, Error, "The user pointer has not been set");

    return nullptr;
}

void            Editor::ImGuiSetClipboardText   (void*           p_userData,
                                                 ANSICHAR const* p_text) noexcept
{
    if (Window* window = reinterpret_cast<Window*>(p_userData))
    {
        return window->SetClipboardString(p_text);
    }

    else
        LOG(LogEditor, Error, "The user pointer has not been set");
}

// ============================== [Private Local Methods] ============================== //

void    Editor::SetupImGui                  () noexcept
{
    Application& application = Application::Get     ();
    ImGuiIO&     io          = ImGui      ::GetIO   ();
    ImGuiStyle&  style       = ImGui      ::GetStyle();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

    io.KeyMap[ImGuiKey_Tab]        = static_cast<int32>(EKey::TAB);
    io.KeyMap[ImGuiKey_LeftArrow]  = static_cast<int32>(EKey::LEFT);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int32>(EKey::RIGHT);
    io.KeyMap[ImGuiKey_UpArrow]    = static_cast<int32>(EKey::UP);
    io.KeyMap[ImGuiKey_DownArrow]  = static_cast<int32>(EKey::DOWN);
    io.KeyMap[ImGuiKey_PageUp]     = static_cast<int32>(EKey::PAGE_UP);
    io.KeyMap[ImGuiKey_PageDown]   = static_cast<int32>(EKey::PAGE_DOWN);
    io.KeyMap[ImGuiKey_Home]       = static_cast<int32>(EKey::HOME);
    io.KeyMap[ImGuiKey_End]        = static_cast<int32>(EKey::END);
    io.KeyMap[ImGuiKey_Insert]     = static_cast<int32>(EKey::INSERT);
    io.KeyMap[ImGuiKey_Delete]     = static_cast<int32>(EKey::DEL);
    io.KeyMap[ImGuiKey_Backspace]  = static_cast<int32>(EKey::BACKSPACE);
    io.KeyMap[ImGuiKey_Space]      = static_cast<int32>(EKey::SPACE);
    io.KeyMap[ImGuiKey_Enter]      = static_cast<int32>(EKey::ENTER);
    io.KeyMap[ImGuiKey_Escape]     = static_cast<int32>(EKey::ESCAPE);
    io.KeyMap[ImGuiKey_A]          = static_cast<int32>(EKey::A);
    io.KeyMap[ImGuiKey_C]          = static_cast<int32>(EKey::C);
    io.KeyMap[ImGuiKey_V]          = static_cast<int32>(EKey::V);
    io.KeyMap[ImGuiKey_X]          = static_cast<int32>(EKey::X);
    io.KeyMap[ImGuiKey_Y]          = static_cast<int32>(EKey::Y);
    io.KeyMap[ImGuiKey_Z]          = static_cast<int32>(EKey::Z);

    io.SetClipboardTextFn = ImGuiSetClipboardText;
    io.GetClipboardTextFn = ImGuiGetClipboardText;
    io.ClipboardUserData  = application.GetMainWindow().get();

    application.GetMainWindow()->BindMouseButtonCallback<Editor, &Editor::ImGuiMouseButtonCallback>(this);
    application.GetMainWindow()->BindScrollCallback     <Editor, &Editor::ImGuiScrollCallback>     (this);
    application.GetMainWindow()->BindKeyCallback        <Editor, &Editor::ImGuiKeyCallback>        (this);
    application.GetMainWindow()->BindCharCallback       <Editor, &Editor::ImGuiCharCallback>       (this);

    ImGui::GetMainViewport()->PlatformHandle = application.GetMainWindow().get();
}

void    Editor::SetupLayout                 () noexcept
{
    m_windows.push_back(new EditorMenu);
    m_windows.push_back(new EditorInspector);
    m_windows.push_back(new EditorHierarchy);

    m_hierarchy = m_windows.back();

    m_windows.push_back(new EditorGame);
    m_windows.push_back(new EditorLog);
    m_windows.push_back(new EditorExplorer);

    for (auto window : m_windows)
        window->Init(this);
}

void    Editor::SetupDocking                () noexcept
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Editor", NULL, window_flags);
}

void    Editor::LinkDocking                 () noexcept
{
    m_layoutInfo.dockspaceID = ImGui::GetID("EditorDockspace");

    if (!m_layoutInfo.isInitialized)
    {
        m_layoutInfo.isInitialized = true;

        ImGui::DockBuilderRemoveNode(m_layoutInfo.dockspaceID);
        ImGui::DockBuilderAddNode(m_layoutInfo.dockspaceID);

        ImGuiID dock_main_id = m_layoutInfo.dockspaceID;

        ImGuiID dock_id_hierarchy = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, NULL, &dock_main_id);
        ImGuiID dock_id_inspector = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, NULL, &dock_main_id);
        ImGuiID dock_id_game = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.6f, NULL, &dock_main_id);
        m_layoutInfo.logsDockID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.f, NULL, &dock_main_id);
        m_layoutInfo.metricsDockID = ImGui::DockBuilderAddNode(m_layoutInfo.logsDockID);

        ImGui::DockBuilderDockWindow("Inspector", dock_id_inspector);
        ImGui::DockBuilderDockWindow("Game", dock_id_game);
        ImGui::DockBuilderDockWindow("Hierarchy", dock_id_hierarchy);
        ImGui::DockBuilderDockWindow("Logs", m_layoutInfo.logsDockID);
        ImGui::DockBuilderDockWindow("ImGui Metrics", m_layoutInfo.metricsDockID);

        ImGui::DockBuilderFinish(m_layoutInfo.dockspaceID);
    }

    ImGui::DockSpace(m_layoutInfo.dockspaceID, ImVec2(0, 0));
}

void    Editor::SetupFont                   () noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& allocator = RHI::Get().GetAllocator();

    uint8* pixels; int32 width; int32 height;

    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    Fence         fence;
    CommandBuffer cmdBuffer(device->GetTransferCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));

    // Staging buffer.
    Buffer             buffer   = {};
    VkBufferCreateInfo bufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

    bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCI.size  = width * height * 4;

    allocator->CreateBuffer(buffer, bufferCI, VMA_ALLOCATION_CREATE_MAPPED_BIT, VMA_MEMORY_USAGE_CPU_ONLY, 0u);

    memcpy(buffer.allocationInfo.pMappedData, pixels, width * height * 4);

    // Actual image.
    VkImageCreateInfo imageCI = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

    imageCI.imageType     = VK_IMAGE_TYPE_2D;
    imageCI.format        = VK_FORMAT_R8G8B8A8_UNORM;
    imageCI.extent.width  = static_cast<uint32>(width);
    imageCI.extent.height = static_cast<uint32>(height);
    imageCI.extent.depth  = 1u;
    imageCI.mipLevels     = 1u;
    imageCI.arrayLayers   = 1u;
    imageCI.samples       = VK_SAMPLE_COUNT_1_BIT;
    imageCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
    imageCI.usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    allocator->CreateImage(m_font, imageCI, 0u, VMA_MEMORY_USAGE_GPU_ONLY, 0u);

    // Copies data from staging buffer to actual image.
    cmdBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };

    barrier.srcAccessMask               = 0u;
    barrier.dstAccessMask               = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout                   = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout                   = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex         = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                       = m_font.handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = 1u;
    barrier.subresourceRange.layerCount = 1u;

    cmdBuffer.InsertMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);

    VkImageSubresourceLayers layers = {};

    layers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    layers.layerCount = 1u;

    cmdBuffer.CopyBufferToImage(layers, buffer, m_font);

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    cmdBuffer.InsertMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);

    cmdBuffer.End();

    device->GetTransferQueue()->Submit(cmdBuffer.GetHandle(), fence.GetHandle());

    // Waits for the transfer operation to complete before destroying the staging buffer.
    fence.Wait();

    // Staging buffer is temporary so it needs to be destroyed after usage.
    allocator->DestroyBuffer(buffer);

    // The command buffer can be freed after the transfer operation has been completed.
    device->GetTransferCommandPool()->FreeCommandBuffer(cmdBuffer);

    // Actual image's view.
    VkImageViewCreateInfo imageViewCI = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };

    imageViewCI.image                       = m_font.handle;
    imageViewCI.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCI.format                      = VK_FORMAT_R8G8B8A8_UNORM;
    imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCI.subresourceRange.levelCount = 1u;
    imageViewCI.subresourceRange.layerCount = 1u;

    VK_CHECK_RESULT(vkCreateImageView(device->GetLogicalDevice(), &imageViewCI, nullptr, &m_font.imageView));

    Debug::SetImageName    (device->GetLogicalDevice(), m_font.handle,    "Font_Image");
    Debug::SetImageViewName(device->GetLogicalDevice(), m_font.imageView, "Font_ImageView");
}

void    Editor::SetupRenderPass             () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkAttachmentDescription attachment = {};

    attachment.format         = RHI::Get().GetSwapchain()->GetFormat();
    attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference reference = {};

    reference.attachment = 0u;
    reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};

    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1u;
    subpass.pColorAttachments    = &reference;

    std::array<VkSubpassDependency, 2> dependencies = {};

    dependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass      = 0u;
    dependencies[0].srcStageMask    = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dependencies[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass      = 0u;
    dependencies[1].dstSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassCI = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };

    renderPassCI.attachmentCount = 1u;
    renderPassCI.pAttachments    = &attachment;
    renderPassCI.subpassCount    = 1u;
    renderPassCI.pSubpasses      = &subpass;
    renderPassCI.dependencyCount = static_cast<uint32>(dependencies.size());
    renderPassCI.pDependencies   = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassCI, nullptr, &m_renderPass));

    Debug::SetRenderPassName(device, m_renderPass, "EditorPass");
}

void    Editor::SetupDescriptorPool         () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorPoolSize poolSize = {};

    poolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = 1u;

    VkDescriptorPoolCreateInfo poolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };

    poolCI.maxSets       = 1u;
    poolCI.poolSizeCount = 1u;
    poolCI.pPoolSizes    = &poolSize;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolCI, nullptr, &m_descriptorPool));

    Debug::SetDescriptorPoolName(device, m_descriptorPool, "Editor_DescriptorPool");
}

void    Editor::SetupDescriptorSetLayout    () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorSetLayoutBinding binding = {};

    binding.binding         = 0u;
    binding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    binding.descriptorCount = 1u;
    binding.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutCI.bindingCount = 1u;
    descriptorSetLayoutCI.pBindings    = &binding;

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCI, nullptr, &m_descriptorSetLayout));

    Debug::SetDescriptorSetLayoutName(device, m_descriptorSetLayout, "Editor_DescriptorSetLayout");
}

void    Editor::SetupDescriptorSets         () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkDescriptorSetAllocateInfo descriptorSetAI = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };

    descriptorSetAI.descriptorPool     = m_descriptorPool;
    descriptorSetAI.descriptorSetCount = 1u;
    descriptorSetAI.pSetLayouts        = &m_descriptorSetLayout;

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &descriptorSetAI, &m_descriptorSet));

    Debug::SetDescriptorSetName(device, m_descriptorSet, "Editor_DescriptorSet");

    VkDescriptorImageInfo imageInfo = {};

    imageInfo.sampler     = RHI::Get().GetTextureSampler();
    imageInfo.imageView   = m_font.imageView;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet writeSet = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };

    writeSet.dstSet          = m_descriptorSet;
    writeSet.dstBinding      = 0u;
    writeSet.descriptorCount = 1u;
    writeSet.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeSet.pImageInfo      = &imageInfo;

    vkUpdateDescriptorSets(device, 1u, &writeSet, 0u, nullptr);
}

void    Editor::SetupPipelineLayout         () noexcept
{
    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkPushConstantRange pushConstant = {};

    pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstant.size       = static_cast<uint32>(sizeof(ImVec2) * 2);

    VkPipelineLayoutCreateInfo layoutCI = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    layoutCI.setLayoutCount         = 1u;
    layoutCI.pSetLayouts            = &m_descriptorSetLayout;
    layoutCI.pushConstantRangeCount = 1u;
    layoutCI.pPushConstantRanges    = &pushConstant;

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &layoutCI, nullptr, &m_pipelineLayout));

    Debug::SetPipelineLayoutName(device, m_pipelineLayout, "Editor_PipelineLayout");
}

void    Editor::SetupPipeline               () noexcept
{
    RHI&          rhi          = RHI         ::Get();
    AssetManager& assetManager = AssetManager::Get();

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};

    shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = assetManager.Get<Shader>("Default/Shaders/gui.vert", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[0].pName  = "main";

    shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = assetManager.Get<Shader>("Default/Shaders/gui.frag", ELoadingMode::BLOCKING)->GetModule();
    shaderStages[1].pName  = "main";

    VkVertexInputBindingDescription binding = {};

    binding.binding   = 0u;
    binding.stride    = sizeof(ImDrawVert);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, 3> attributes = {};

    attributes[0].location = 0u;
    attributes[0].binding  = 0u;
    attributes[0].format   = VK_FORMAT_R32G32_SFLOAT;
    attributes[0].offset   = offsetof(ImDrawVert, pos);

    attributes[1].location = 1u;
    attributes[1].binding  = 0u;
    attributes[1].format   = VK_FORMAT_R32G32_SFLOAT;
    attributes[1].offset   = offsetof(ImDrawVert, uv);

    attributes[2].location = 2u;
    attributes[2].binding  = 0u;
    attributes[2].format   = VK_FORMAT_R8G8B8A8_UNORM;
    attributes[2].offset   = offsetof(ImDrawVert, col);

    VkPipelineVertexInputStateCreateInfo vertexInputStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO   };

    vertexInputStateCI.vertexBindingDescriptionCount   = 1u;
    vertexInputStateCI.pVertexBindingDescriptions      = &binding;
    vertexInputStateCI.vertexAttributeDescriptionCount = static_cast<uint32>(attributes.size());
    vertexInputStateCI.pVertexAttributeDescriptions    = attributes.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCI = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };

    inputAssemblyStateCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportStateCI = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };

    viewportStateCI.viewportCount = 1u;
    viewportStateCI.scissorCount  = 1u;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCI = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };

    rasterizationStateCI.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCI.cullMode    = VK_CULL_MODE_NONE;
    rasterizationStateCI.frontFace   = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationStateCI.lineWidth   = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCI = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };

    multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorAttachment = {};

    colorAttachment.blendEnable         = VK_TRUE;
    colorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
    colorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;
    colorAttachment.colorWriteMask      = 0xf;

    VkPipelineColorBlendStateCreateInfo colorBlendStateCI = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };

    colorBlendStateCI.attachmentCount = 1u;
    colorBlendStateCI.pAttachments    = &colorAttachment;

    std::array<VkDynamicState, 2> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicStateCI = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };

    dynamicStateCI.dynamicStateCount = static_cast<uint32>(dynamicStates.size());
    dynamicStateCI.pDynamicStates    = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineCI = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

    pipelineCI.stageCount          = static_cast<uint32>(shaderStages.size());
    pipelineCI.pStages             = shaderStages.data();
    pipelineCI.pVertexInputState   = &vertexInputStateCI;
    pipelineCI.pInputAssemblyState = &inputAssemblyStateCI;
    pipelineCI.pTessellationState  = nullptr;
    pipelineCI.pViewportState      = &viewportStateCI;
    pipelineCI.pRasterizationState = &rasterizationStateCI;
    pipelineCI.pMultisampleState   = &multisampleStateCI;
    pipelineCI.pDepthStencilState  = nullptr;
    pipelineCI.pColorBlendState    = &colorBlendStateCI;
    pipelineCI.pDynamicState       = &dynamicStateCI;
    pipelineCI.layout              = m_pipelineLayout;
    pipelineCI.renderPass          = m_renderPass;
    pipelineCI.subpass             = 0u;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(rhi.GetDevice       ()->GetLogicalDevice(),
                                              rhi.GetPipelineCache()->GetHandle       (),
                                              1u,
                                              &pipelineCI,
                                              nullptr,
                                              &m_pipeline));

    Debug::SetPipelineName(rhi.GetDevice()->GetLogicalDevice(), m_pipeline, "Editor_Pipeline");
}

void    Editor::SetupFrames                 () noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& swapchain = RHI::Get().GetSwapchain();

    VkFenceCreateInfo       fenceCI       = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO       };
    VkSemaphoreCreateInfo   semaphoreCI   = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO   };
    VkFramebufferCreateInfo framebufferCI = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };

    framebufferCI.renderPass      = m_renderPass;
    framebufferCI.attachmentCount = 1u;
    framebufferCI.width           = swapchain->GetExtent().width;
    framebufferCI.height          = swapchain->GetExtent().height;
    framebufferCI.layers          = 1u;

    for (size_t i = 0; i < m_frames.size(); ++i)
    {
        m_frames[i].commandBuffer = device->GetGraphicsCommandPool()->AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        Debug::SetCommandBufferName(device->GetLogicalDevice(), m_frames[i].commandBuffer.GetHandle(), ("Editor_CommandBuffer_" + std::to_string(i)).c_str());

        fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_CHECK_RESULT(vkCreateFence    (device->GetLogicalDevice(), &fenceCI,     nullptr, &m_frames[i].fence));
        VK_CHECK_RESULT(vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreCI, nullptr, &m_frames[i].imageAvailableSemaphore));
        VK_CHECK_RESULT(vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreCI, nullptr, &m_frames[i].renderFinishedSemaphore));

        Debug::SetFenceName    (device->GetLogicalDevice(), m_frames[i].fence,                   ("Editor_Fence_"                   + std::to_string(i)).c_str());
        Debug::SetSemaphoreName(device->GetLogicalDevice(), m_frames[i].imageAvailableSemaphore, ("Editor_ImageAvailableSemaphore_" + std::to_string(i)).c_str());
        Debug::SetSemaphoreName(device->GetLogicalDevice(), m_frames[i].renderFinishedSemaphore, ("Editor_RenderFinishedSemaphore_" + std::to_string(i)).c_str());

        framebufferCI.pAttachments = &swapchain->GetImageViews()[i];

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_frames[i].framebuffer));
     
        Debug::SetFramebufferName(device->GetLogicalDevice(), m_frames[i].framebuffer, ("Editor_Framebuffer_" + std::to_string(i)).c_str());
    }
}

void    Editor::FocusOnEntity               ()  noexcept
{
    if (m_hierarchy != nullptr &&
        ImGui::IsKeyPressed(static_cast<int32>(EKey::F)) && 
        m_editorCamera.GetEditorCamera() != nullptr && 
        !GetWorld()->HasBegunPlay())
    {
        Entity* entity = dynamic_cast<EditorHierarchy*>(m_hierarchy)->GetSelectedEntity();

        if (entity != nullptr)
        {
            SceneComponent* root = entity->GetRootComponent();

            m_editorCamera.SetFocus(root ? root->GetWorldLocation() : Vector3::Zero);
        }
    }
}

// ===================================================================================== //

void    Editor::UpdateWindow                (ImGuiIO&         p_IO,
                                             ImGuiPlatformIO& p_platformIO) noexcept
{
    auto const& window = Application::Get().GetMainWindow();

    Extent2D const& windowExtent      = window->GetExtent           ();
    Extent2D const& framebufferExtent = window->GetFramebufferExtent();

    p_IO.DisplaySize = ImVec2(static_cast<float>(windowExtent.width),
                              static_cast<float>(windowExtent.height));

    if (p_IO.DisplaySize.x > 0.0f && p_IO.DisplaySize.y > 0.0f)
    {
        p_IO.DisplayFramebufferScale = ImVec2(static_cast<float>(framebufferExtent.width)  / p_IO.DisplaySize.x,
                                              static_cast<float>(framebufferExtent.height) / p_IO.DisplaySize.y);
    }
}

void    Editor::UpdateMousePosAndButtons    (ImGuiIO&         p_IO,
                                             ImGuiPlatformIO& p_platformIO) noexcept
{
    ImVec2 const mousePosBackup = p_IO.MousePos;

    p_IO.MousePos             = ImVec2(-MAX_FLOAT, -MAX_FLOAT);
    p_IO.MouseHoveredViewport = 0;

    for (int32 i = 0; i < IM_ARRAYSIZE(p_IO.MouseDown); ++i)
    {
        p_IO.MouseDown  [i] = MouseJustPressed[i];
        MouseJustPressed[i] = false;
    }

    for (int32 i = 0; i < p_platformIO.Viewports.Size; ++i)
    {
        ImGuiViewport* viewport = p_platformIO.Viewports[i];
        Window*        window   = reinterpret_cast<Window*>(viewport->PlatformHandle);

        if (window->IsFocused())
        {
            if (p_IO.WantSetMousePos)
            {
                window->SetCursorPosition(static_cast<double>(mousePosBackup.x - viewport->Pos.x),
                                          static_cast<double>(mousePosBackup.y - viewport->Pos.y));
            }

            else
            {
                Vector2 mousePos = window->GetCursorPosition();

                if (p_IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    Offset2D windowPos = window->GetPosition();

                    p_IO.MousePos = ImVec2(mousePos.m_x + static_cast<float>(windowPos.x),
                                           mousePos.m_y + static_cast<float>(windowPos.y));
                }

                else
                {
                    p_IO.MousePos = ImVec2(mousePos.m_x, mousePos.m_y);
                }

                for (int32 i = 0; i < IM_ARRAYSIZE(p_IO.MouseDown); ++i)
                    p_IO.MouseDown[i] |= window->GetMouseButton(static_cast<EButton>(i)) != EState::RELEASED;
            }
        }
    }
}

void    Editor::UpdateMouseCursor           (ImGuiIO&         p_IO,
                                             ImGuiPlatformIO& p_platformIO) noexcept
{

}

void    Editor::UpdateLayout                () noexcept
{
    SetupDocking();

    LinkDocking();

    for (auto window : m_windows)
    {
        if (dynamic_cast<EditorExplorer*>(window) != nullptr)
        {
            ImGui::SetNextWindowDockID(m_layoutInfo.logsDockID, ImGuiCond_Once);

            window->Update();

            ImGui::SetNextWindowDockID(m_layoutInfo.metricsDockID, ImGuiCond_Once); 

            ImGui::ShowMetricsWindow();
        }
        else
            window->Update();
    }

    ImGui::End();

    ImGui::PopStyleVar(3);
}

// ======================================================================================== //

void    Editor::Rebuild () noexcept
{
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& swapchain = RHI::Get().GetSwapchain();

    vkDestroyRenderPass(device->GetLogicalDevice(), m_renderPass, nullptr);

    SetupRenderPass();

    VkFramebufferCreateInfo framebufferCI = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };

    framebufferCI.renderPass      = m_renderPass;
    framebufferCI.attachmentCount = 1u;
    framebufferCI.width           = swapchain->GetExtent().width;
    framebufferCI.height          = swapchain->GetExtent().height;
    framebufferCI.layers          = 1u;

    for (size_t i = 0; i < m_frames.size(); ++i)
    {
        framebufferCI.pAttachments = &swapchain->GetImageViews()[i];

        vkDestroyFramebuffer(device->GetLogicalDevice(), m_frames[i].framebuffer, nullptr);

        VK_CHECK_RESULT(vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferCI, nullptr, &m_frames[i].framebuffer));
    }
}

void    Editor::Draw    () noexcept
{
    ImDrawData* data = ImGui::GetDrawData();

    float width  = data->DisplaySize.x * data->FramebufferScale.x;
    float height = data->DisplaySize.y * data->FramebufferScale.y;

    if (width == 0.0f || height == 0.0f || data->TotalVtxCount == 0)
        return;

    EditorFrame& frame = m_frames[RHI::Get().GetCurrentFrame().index];

    auto const& allocator = RHI::Get().GetAllocator();
    auto const& device    = RHI::Get().GetDevice   ();
    auto const& swapchain = RHI::Get().GetSwapchain();

    VkResult result = swapchain->AcquireNextImage(frame.imageAvailableSemaphore);

    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
        Rebuild();

    if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
    {
        VK_CHECK_RESULT(vkWaitForFences(device->GetLogicalDevice(), 1u, &frame.fence, VK_TRUE, MAX_UINT_64));
        VK_CHECK_RESULT(vkResetFences  (device->GetLogicalDevice(), 1u, &frame.fence));

        // Create or resize the vertex/index buffers
        size_t vertexBufferSize = data->TotalVtxCount * sizeof(ImDrawVert);
        size_t indexBufferSize  = data->TotalIdxCount * sizeof(ImDrawIdx);

        if (frame.vertexBuffer.handle == VK_NULL_HANDLE || frame.vertexBuffer.allocationInfo.size < vertexBufferSize)
        {
            if (frame.vertexBuffer.handle != VK_NULL_HANDLE)
                allocator->DestroyBuffer(frame.vertexBuffer);

            VkBufferCreateInfo bufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

            bufferCI.size  = vertexBufferSize;
            bufferCI.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

            allocator->CreateBuffer(frame.vertexBuffer, bufferCI, 0u, VMA_MEMORY_USAGE_CPU_TO_GPU, 0u);
        }

        if (frame.indexBuffer.handle == VK_NULL_HANDLE || frame.indexBuffer.allocationInfo.size < vertexBufferSize)
        {
            if (frame.indexBuffer.handle != VK_NULL_HANDLE)
                allocator->DestroyBuffer(frame.indexBuffer);

            VkBufferCreateInfo bufferCI = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };

            bufferCI.size  = vertexBufferSize;
            bufferCI.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

            allocator->CreateBuffer(frame.indexBuffer, bufferCI, 0u, VMA_MEMORY_USAGE_CPU_TO_GPU, 0u);
        }

        // Upload vertex/index data into a single contiguous GPU buffer
        allocator->Map(frame.vertexBuffer);
        allocator->Map(frame.indexBuffer);

        ImDrawVert* vertices = reinterpret_cast<ImDrawVert*>(frame.vertexBuffer.allocationInfo.pMappedData);
        ImDrawIdx*  indices  = reinterpret_cast<ImDrawIdx*> (frame.indexBuffer .allocationInfo.pMappedData);

        for (int32 i = 0; i < data->CmdListsCount; ++i)
        {
            ImDrawList const* cmdList = data->CmdLists[i];

            memcpy(vertices, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(indices,  cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));

            vertices += cmdList->VtxBuffer.Size;
            indices  += cmdList->IdxBuffer.Size;
        }

        allocator->Unmap(frame.vertexBuffer);
        allocator->Unmap(frame.indexBuffer);

        VkClearValue const clearValue = { 0.45f, 0.55f, 0.60f, 1.00f };

        frame.commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkRenderPassBeginInfo renderPassBI = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };

        renderPassBI.renderPass               = m_renderPass;
        renderPassBI.framebuffer              = m_frames[swapchain->GetImageIndex()].framebuffer;
        renderPassBI.renderArea.extent.width  = swapchain->GetExtent().width;
        renderPassBI.renderArea.extent.height = swapchain->GetExtent().height;
        renderPassBI.clearValueCount          = 1u;
        renderPassBI.pClearValues             = &clearValue;

        vkCmdBeginRenderPass(frame.commandBuffer.GetHandle(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(frame.commandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

        // Bind Vertex And Index Buffer:
        VkDeviceSize offset = 0;

        vkCmdBindVertexBuffers(frame.commandBuffer.GetHandle(), 0u, 1u, &frame.vertexBuffer.handle, &offset);

        vkCmdBindIndexBuffer(frame.commandBuffer.GetHandle(),
                             frame.indexBuffer.handle,
                             0,
                             sizeof(ImDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);

        // Setup viewport
        VkViewport viewport = {
            0.0f,
            0.0f,
            width,
            height,
            0.0f,
            1.0f
        };

        vkCmdSetViewport(frame.commandBuffer.GetHandle(), 0u, 1u, &viewport);

        // Setup scale and translation:
        float scale    [2];
        float translate[2];

        scale    [0] =  2.0f / data->DisplaySize.x;
        scale    [1] =  2.0f / data->DisplaySize.y;
        translate[0] = -1.0f - data->DisplayPos.x * scale[0];
        translate[1] = -1.0f - data->DisplayPos.y * scale[1];

        vkCmdPushConstants(frame.commandBuffer.GetHandle(),
                           m_pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           sizeof(float) * 0,
                           sizeof(float) * 2,
                           scale);

        vkCmdPushConstants(frame.commandBuffer.GetHandle(),
                           m_pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           sizeof(float) * 2,
                           sizeof(float) * 2,
                           translate);

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clipOff   = data->DisplayPos;
        ImVec2 clipScale = data->FramebufferScale;

        // Render command lists
        uint32 vertexOffset = 0u;
        uint32 indexOffset  = 0u;

        for (int32 i = 0; i < data->CmdListsCount; ++i)
        {
            ImDrawList const* cmdList = data->CmdLists[i];

            for (int32 cmdIndex = 0; cmdIndex < cmdList->CmdBuffer.Size; ++cmdIndex)
            {
                ImDrawCmd const* cmd = &cmdList->CmdBuffer[cmdIndex];

                vkCmdBindDescriptorSets(frame.commandBuffer.GetHandle(),
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        m_pipelineLayout,
                                        0u,
                                        1u,
                                        cmd->TextureId ? reinterpret_cast<VkDescriptorSet*>(cmd->TextureId) : &m_descriptorSet,
                                        0u,
                                        nullptr);

                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clipRect;

                clipRect.x = (cmd->ClipRect.x - clipOff.x) * clipScale.x;
                clipRect.y = (cmd->ClipRect.y - clipOff.y) * clipScale.y;
                clipRect.z = (cmd->ClipRect.z - clipOff.x) * clipScale.x;
                clipRect.w = (cmd->ClipRect.w - clipOff.y) * clipScale.y;

                if (clipRect.x < width && clipRect.y < height && clipRect.z >= 0.0f && clipRect.w >= 0.0f)
                {
                    // Negative offsets are illegal for vkCmdSetScissor
                    if (clipRect.x < 0.0f)
                        clipRect.x = 0.0f;
                    if (clipRect.y < 0.0f)
                        clipRect.y = 0.0f;

                    // Apply scissor/clipping rectangle
                    VkRect2D scissor = {
                        static_cast<int32> (clipRect.x),
                        static_cast<int32> (clipRect.y),
                        static_cast<uint32>(clipRect.z - clipRect.x),
                        static_cast<uint32>(clipRect.w - clipRect.y)
                    };

                    vkCmdSetScissor(frame.commandBuffer.GetHandle(), 0u, 1u, &scissor);

                    vkCmdDrawIndexed(frame.commandBuffer.GetHandle(), cmd->ElemCount, 1u, indexOffset, vertexOffset, 0u);
                }

                indexOffset += cmd->ElemCount;
            }

            vertexOffset += static_cast<uint32>(cmdList->VtxBuffer.Size);
        }

        vkCmdEndRenderPass(frame.commandBuffer.GetHandle());

        frame.commandBuffer.End();

        device->GetGraphicsQueue()->Submit(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                           frame.imageAvailableSemaphore,
                                           frame.renderFinishedSemaphore,
                                           frame.commandBuffer.GetHandle(),
                                           frame.fence);

        device->GetPresentQueue()->Present(swapchain->GetImageIndex(),
                                           swapchain->GetHandle    (),
                                           frame.renderFinishedSemaphore);
    }
}

// ======================================================================================== //

void    Editor::ImGuiMouseButtonCallback    (EButton p_button,
                                             EState  p_state) noexcept
{
    if (p_state == EState::PRESSED)
        MouseJustPressed[static_cast<int32>(p_button)] = true;
}

void    Editor::ImGuiScrollCallback         (double p_xOffset,
                                             double p_yOffset) noexcept
{
    ImGuiIO& io = ImGui::GetIO();

    io.MouseWheelH += static_cast<float>(p_xOffset);
    io.MouseWheel  += static_cast<float>(p_yOffset);
}

void    Editor::ImGuiKeyCallback            (EKey   p_key,
                                             EState p_state) noexcept
{
    if (p_key == EKey::ESCAPE)
        Application::Get().GetMainWindow()->RemoveKeyCallback<Editor, &Editor::ImGuiKeyCallback>(this);

    ImGuiIO& io = ImGui::GetIO();

    if (p_state == EState::PRESSED)
        io.KeysDown[static_cast<int32>(p_key)] = true;
    if (p_state == EState::RELEASED)
        io.KeysDown[static_cast<int32>(p_key)] = false;

    io.KeyCtrl  = io.KeysDown[static_cast<int32>(EKey::LEFT_CONTROL)] || io.KeysDown[static_cast<int32>(EKey::RIGHT_CONTROL)];
    io.KeyShift = io.KeysDown[static_cast<int32>(EKey::LEFT_SHIFT)]   || io.KeysDown[static_cast<int32>(EKey::RIGHT_SHIFT)];
    io.KeyAlt   = io.KeysDown[static_cast<int32>(EKey::LEFT_ALT)]     || io.KeysDown[static_cast<int32>(EKey::RIGHT_ALT)];
    io.KeySuper = io.KeysDown[static_cast<int32>(EKey::LEFT_SUPER)]   || io.KeysDown[static_cast<int32>(EKey::RIGHT_SUPER)];
}

void    Editor::ImGuiCharCallback           (uint32 p_code) noexcept
{
    if (p_code > 0u && p_code < 0x10000)
        ImGui::GetIO().AddInputCharacter(static_cast<uint16>(p_code));
}

void    Editor::SpawnEntity                 (std::string const& p_name)     noexcept
{
    World* world = GetWorld();

    if (world != nullptr)
    {
        world->SpawnEntity<Entity>(p_name);
    }
}
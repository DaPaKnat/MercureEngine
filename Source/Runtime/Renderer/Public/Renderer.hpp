#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "EngineModule.hpp"

#include "RenderScene.hpp"
#include "RenderList.hpp"

class ENGINE_API Renderer : public EngineModule
{
    public:

    // ============================== [Public Static Methods] ============================== //

        static INLINE Renderer& Get() noexcept
        {
            return GEngine->GetModuleManager()->Get<Renderer>();
        }

    // ============================== [Public Constructor and Destructor] ============================== //

        Renderer    ();

        ~Renderer   () = default;

    // ============================== [Module Protected Local Methods] ============================== //

        virtual void    Initialize  (EngineKey const& p_passkey)   final override;

        virtual void    Update      (EngineKey const& p_passkey)   final override;

        virtual void    Shutdown    (EngineKey const& p_passkey)   final override;

    // ============================== [Public Local Methods] ============================== //

        void    Render  (EngineKey   const& p_passkey,
                         class Level const& p_level) noexcept;

    private:

    // ============================== [Private Local Properties] ============================== //

        std::atomic_bool                m_running;

        std::unique_ptr<RenderScene>    m_activeScene;

        std::unique_ptr<RenderScene>    m_inactiveScene;

        std::unique_ptr<RenderList>     m_renderList;

};  // !class Renderer

#endif // !__RENDERER_HPP__
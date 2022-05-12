#ifndef __EDITOR_HIERARCHY_HPP__
#define __EDITOR_HIERARCHY_HPP__

#include "EditorWindow.hpp"
#include "GameFramework/Entity.hpp"

DECLARE_DELEGATE_RetVal(Entity* const*, EditorLevelEntities);
DECLARE_DELEGATE_RetVal(size_t, EditorLevelEntityCount);

class EditorHierarchy : public EditorWindow
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorHierarchy     ();

        ~EditorHierarchy    ();

    // ============================== [Public Local Methods] ============================== //

        void    Init                (class Editor*  p_editor)   noexcept override;

        void    Update              ()                          noexcept override;

        void    ShowEntity          (Entity*        p_entity)   noexcept;

        void    MenuBar             ()                          noexcept;

        void    CheckPopup          ()                          noexcept;

        void    EntityPopup         ()                          noexcept;

        Entity* GetSelectedEntity   ()                          noexcept;

        void    SpawnEntity         (std::string const& p_name);

    private:

    // ============================== [Private Local Delegates] ============================== //

        EditorLevelEntities     m_levelEntitiesDelegate;

        EditorLevelEntityCount  m_levelEntityCountDelegate;

    // ============================== [Private Local Properties] ============================== //

        Entity*                     m_selectedEntity = nullptr;

        std::string                 m_entityType;

        ImVector<char>              m_inputBuffer;

        std::vector<std::string>    m_entityTypes;

    // ============================== [Private Static Methods] ============================== //

        static int  TextInputCallBack   (ImGuiInputTextCallbackData*    p_data);

}; // !class EditorHierarchy

#include "EditorHierarchy.inl"
#include "EditorHierarchy.generated.hpp"

#endif // !__EDITOR_HIERARCHY_HPP__
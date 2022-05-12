#ifndef __EDITOR_MENU_HPP__
#define __EDITOR_MENU_HPP__

#include "EditorWindow.hpp"
#include "Camera/EditorCamera.hpp"

DECLARE_DELEGATE(EditorPlay);
DECLARE_DELEGATE_OneParam(EditorCreateLevel, std::string const&);
DECLARE_DELEGATE_OneParam(EditorLoadLevel, std::string const&)
DECLARE_DELEGATE_OneParam(EditorImportLevel, std::string const&)
DECLARE_DELEGATE_RetVal(std::vector<std::string>, EditorLevelNames);
DECLARE_DELEGATE(EditorSaveLevel);
DECLARE_DELEGATE(EditorClose);
DECLARE_DELEGATE_RetVal(EditorCamera&, EditorCameraDelegate);

struct EditorCameraParameters
{
    float   m_cameraRotation    = 0.0f;

    float   m_cameraMovement    = 0.0f;

    float   m_focusSpeed        = 0.0f;

    float   m_focusDistance     = 0.0f;
};

class EditorMenu : public EditorWindow
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorMenu  ();

        ~EditorMenu ();

     // ============================== [Public Local Methods] ============================== //

        void    Init    (class Editor*  p_editor)   noexcept override;

        void    Update  ()                          noexcept override;

    private:

    // ============================== [Private Local Properties] ============================== //
    
        bool                    m_canUndo               = false;

        bool                    m_canRedo               = false;

        bool                    m_canCopy               = false;

        bool                    m_canCut                = false;

        bool                    m_canPaste              = false;

        bool                    m_canDuplicate          = false;

        bool                    m_canDelete             = false;

        bool                    m_newLevel              = false;

        bool                    m_loadLevel             = false;

        bool                    m_setCameraParameters   = false;

        EditorCameraParameters  m_cameraParameters;

        ImVector<char>          m_inputBuffer;

     // ============================== [Private Local Callbacks] ============================== //

        EditorPlay              m_tooglePlayDelegate;

        EditorCreateLevel       m_createLevelDelegate;

        EditorLoadLevel         m_LoadLevelDelegate;;

        EditorImportLevel       m_importLevelDelegate;

        EditorSaveLevel         m_saveLevelDelegate;
    
        EditorClose             m_closeDelegate;

        EditorLevelNames        m_levelNamesDelegate;

        EditorCameraDelegate    m_editorCameraDelegate;

     // ============================== [Private Static Methods] ============================== //

        static int  TextInputCallBack   (ImGuiInputTextCallbackData*    p_data);

     // ============================== [Private Local Methods] ============================== //
        
        void    FileMenu                ()  noexcept;

        void    EditMenu                ()  noexcept;

        void    PlayButton              ()  noexcept;

        void    StyleMercure            ()  noexcept;

        void    NewLevel                ()  noexcept;

        void    LoadLevel               ()  noexcept;

        void    ImportLevel             ()  noexcept;

        void    SetCameraParameters     ()  noexcept;

        void    CheckPopups             ()  noexcept;

}; // !class EditorInspector

#include "EditorMenu.inl"

#endif // !__EDITOR_MENU_HPP__
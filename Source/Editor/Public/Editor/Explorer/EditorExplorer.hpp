#ifndef __EDITOR_EXPLORER_HPP__
#define __EDITOR_EXPLORER_HPP__

#include "EditorWindow.hpp"
#include "AssetManager.hpp"

struct ExplorerAsset
{
    std::string name        = "";

    std::string path        = "";

    std::string typeName    = "";

    EAssetType  type        = EAssetType::EMPTY;

    bool        isValid     = false;
};

struct ExplorerFolder
{
    std::string                 name;

    std::vector<ExplorerAsset>  assets;

    std::vector<ExplorerFolder> folders;
};

class EditorExplorer : public EditorWindow
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorExplorer  ();

        ~EditorExplorer ()  noexcept;

    // ============================== [Public Local Methods] ============================== //

        void    Init        (class Editor*                              p_editor)   noexcept override;

        void    Update      ()                                                      noexcept override;

    private:

    // ============================== [Public Local Properties] ============================== //

        ExplorerFolder  m_root;

        ExplorerAsset*  m_selectedAsset;

    // ============================== [Private Local Methods] ============================== //

        ExplorerFolder  LoadFolder          (std::string_view   p_path)     noexcept;

        ExplorerAsset   LoadAsset           (std::string_view   p_path);

        void            OnFileDrop          (ANSICHAR const*    p_path)     noexcept;

        void            ShowAssetInspector  ()                              noexcept;

        void            ShowFolderContent   (ExplorerFolder&    p_folder)   noexcept;
        
    // ===================================================================================== //

        template <typename T>
        void    EditAsset       (T*             p_member);

        template <typename T>
        void    EditAssetMember (T*             p_data);

        void    EditAssetMember (MaterialData*  p_data);

        void    EditAssetMember (Color*         p_data);

        void    EditAssetMember (Vector3*       p_data);

        void    EditAssetMember (float*         p_data);

}; // !class EditorExplorer

#include "EditorExplorer.inl"

#endif // !__EDITOR_EXPLORER_HPP__
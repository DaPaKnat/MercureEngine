#ifndef __EDITOR_GAME_HPP__
#define __EDITOR_GAME_HPP__

#include "EditorWindow.hpp"

class EditorGame : public EditorWindow
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorGame  () = default;

        ~EditorGame () = default;

     // ============================== [Public Local Methods] ============================== //

        void    Init    (class Editor* p_editor)    noexcept override;

        void    Update  ()                          noexcept override;

    private:

    // ============================== [Private Local Properties] ============================== //

        Editor* m_editor;

}; // !class EditorGame

#endif // !__EDITOR_GAME_HPP__
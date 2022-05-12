#ifndef __EDITOR_WINDOW_HPP__
#define __EDITOR_WINDOW_HPP__

#include "CoreMinimal.hpp"

class EditorWindow : public UniqueObject
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorWindow            ()  = default;

        virtual ~EditorWindow   ()  = default;

    // ============================== [Public Local Methods] ============================== //

        virtual void    Init    (class Editor*  p_editor)   noexcept;

        virtual void    Update  ()                          noexcept;
};

#include "EditorWindow.inl"

#endif // !__EDITOR_WINDOW_HPP__
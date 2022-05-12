#ifndef __EDITOR_WINDOW_INL__
#define __EDITOR_WINDOW_INL__

// ============================== [Public Local Methods] ============================== //

INLINE void EditorWindow::Init      (Editor*    p_editor)  noexcept
{
    (void)p_editor;
}

INLINE void EditorWindow::Update    ()                      noexcept
{
    ImGui::Begin("Window");

    ImGui::End();
}

#endif // !__EDITOR_WINDOW_INL__
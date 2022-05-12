#ifndef __EDITOR_LOG_HPP__
#define __EDITOR_LOG_HPP__

#include "EditorWindow.hpp"

class EditorLog : public EditorWindow
{
    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        EditorLog   ();

        ~EditorLog  ();

     // ============================== [Public Local Methods] ============================== //

        void    Init    (class Editor*  p_editor)   noexcept override;

        void    Update  ()                          noexcept override;

    private:

    // ============================== [Private Local Properties] ============================== //

        ImGuiTextBuffer m_buffer;

        ImGuiTextFilter m_filter;

        ImVector<int>   m_lineOffsets;

        bool            m_clear;

        char const*     m_bufferStart;

        char const*     m_bufferEnd;

    // ============================== [Public Local Methods] ============================== //

        void    AddLog          (std::string const& p_log);

        void    Clear           ()                          noexcept;

        void    EndLogEntry     ()                          noexcept;

        void    InitLogEntry    ()                          noexcept;

        void    PrintAll        ()                          noexcept;

        void    PrintWithFilter ()                          noexcept;

        void    SetLogEntry     ()                          noexcept;

        void    SetMenuBar      ()                          noexcept;
};

#include "Log/EditorLog.inl"

#endif __EDITOR_LOG_HPP__
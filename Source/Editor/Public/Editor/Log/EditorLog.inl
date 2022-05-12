#ifndef __EDITOR_LOG_INL__
#define __EDITOR_LOG_INL__

#include "Editor.hpp"

// ============================== [Public Local Methods] ============================== //

INLINE void EditorLog::Init     (Editor*    p_engine)   noexcept
{
    (void*)p_engine;

    Logger::Get().RegisterToLogOutput<EditorLog, &EditorLog::AddLog>(this);
}

INLINE void EditorLog::Update   ()                      noexcept
{
    ImGui::Begin("Logs");

    SetMenuBar();

    SetLogEntry();

    ImGui::End();
}

// ============================== [Public Local Methods] ============================== //

INLINE void EditorLog::AddLog       (std::string const& p_log)
{
    int old_size = m_buffer.size(); 

    m_buffer.append(p_log.c_str());
    
    for (int new_size = m_buffer.size(); old_size < new_size; old_size++)
    {
        if (m_buffer[old_size] == '\n')
            m_lineOffsets.push_back(old_size + 1);
    }
}

INLINE void EditorLog::Clear        ()  noexcept
{
    m_buffer.clear();

    m_lineOffsets.clear();
    m_lineOffsets.push_back(0);
}

INLINE void EditorLog::EndLogEntry  ()  noexcept
{
    ImGui::PopStyleVar();

    ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
}

INLINE void EditorLog::InitLogEntry ()  noexcept
{
    ImGui::BeginChild("Scroller", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (m_clear)
        Clear();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    m_bufferStart = m_buffer.begin();
    m_bufferEnd = m_buffer.end();
}

INLINE void EditorLog::PrintAll     ()  noexcept
{
    for (int line_no = 0; line_no < m_lineOffsets.Size; line_no++)
    {
        const char* line_start = m_bufferStart + m_lineOffsets[line_no];
        const char* line_end = (line_no + 1 < m_lineOffsets.Size) ? (m_bufferStart + m_lineOffsets[line_no + 1] - 1) : m_bufferEnd;

        if (m_filter.PassFilter(line_start, line_end))
            ImGui::TextUnformatted(line_start, line_end);
    }
}

INLINE void EditorLog::PrintWithFilter  ()  noexcept
{
    ImGuiListClipper clipper;

    clipper.Begin(m_lineOffsets.Size);

    while (clipper.Step())
    {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
        {
            const char* line_start = m_bufferStart + m_lineOffsets[line_no];
            const char* line_end = (line_no + 1 < m_lineOffsets.Size) ? (m_bufferStart + m_lineOffsets[line_no + 1] - 1) : m_bufferEnd;

            ImGui::TextUnformatted(line_start, line_end);
        }
    }

    clipper.End();
}

INLINE void EditorLog::SetMenuBar   ()  noexcept
{
    m_clear = ImGui::Button("Clear");

    ImGui::SameLine();

    m_filter.Draw("Filter", -100.0f);

    ImGui::Separator();
}

INLINE void EditorLog::SetLogEntry  ()  noexcept
{
    InitLogEntry();

    if (m_filter.IsActive())
        PrintWithFilter();
    else
        PrintAll();

    EndLogEntry();
}

#endif // !__EDITOR_LOG_INL__
#include "PCH.hpp"

#include "Log/EditorLog.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

EditorLog::EditorLog    () :
    EditorWindow    (),
    m_buffer        {},
    m_filter        {},
    m_lineOffsets   {},
    m_clear         { false }
{
    Clear();
}

EditorLog::~EditorLog   ()
{
    Clear();
}
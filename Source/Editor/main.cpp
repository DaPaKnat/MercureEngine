#include "PCH.hpp"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Editor.hpp"

int main(int argc, char* argv[])
{
    Editor editor;

    editor.Run();

    return EXIT_SUCCESS;
}
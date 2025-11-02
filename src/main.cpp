#include "core/Application.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    // Set random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize application
    Application app;
    if (!app.Initialize()) {
        MessageBox(nullptr, 
                   L"Failed to initialize application", 
                   L"Error", 
                   MB_ICONERROR);
        return -1;
    }
    
    // Run application
    int result = app.Run();
    
    // Shutdown
    app.Shutdown();
    
    return result;
}
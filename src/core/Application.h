#pragma once
#include <memory>
#include "../core/PopupManager.h"
#include "../input/KeyboardHook.h"
#include "../config/Config.h"

class Application {
public:
    Application();
    ~Application();
    
    bool Initialize();
    int Run();
    void Shutdown();
    
private:
    void OnKeyPressed(DWORD vkCode);
    
private:
    std::unique_ptr<PopupManager> m_popupManager;
    std::unique_ptr<KeyboardHook> m_keyboardHook;
    AppConfig m_config;
    bool m_running = false;
};
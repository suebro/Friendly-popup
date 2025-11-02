#include "Application.h"
#include "../config/Messages.h"
#include <iostream>

Application::Application() {}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    // Load configuration
    m_config = AppConfig(); // In real implementation, load from file
    
    // Initialize popup manager
    m_popupManager = std::make_unique<PopupManager>();
    if (!m_popupManager->Initialize(m_config)) {
        return false;
    }
    
    // Load messages
    auto messages = Messages::LoadMessagesFromFile(m_config.messageFile);
    m_popupManager->SetMessages(messages);
    
    // Initialize keyboard hook
    m_keyboardHook = std::make_unique<KeyboardHook>();
    m_keyboardHook->SetKeyCallback([this](DWORD vkCode) {
        this->OnKeyPressed(vkCode);
    });
    
    if (!m_keyboardHook->Install()) {
        std::wcout << L"Failed to install keyboard hook" << std::endl;
        return false;
    }
    
    m_running = true;
    return true;
}

int Application::Run() {
    if (!m_running) return -1;
    
    // Start creating popups
    m_popupManager->Start();
    
    // Main message loop
    MSG msg;
    while (m_running && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

void Application::Shutdown() {
    m_running = false;
    
    if (m_popupManager) {
        m_popupManager->Stop();
        m_popupManager->CloseAllPopups();
    }
    
    if (m_keyboardHook) {
        m_keyboardHook->Uninstall();
    }
    
    PopupWindow::UnregisterWindowClass();
}

void Application::OnKeyPressed(DWORD vkCode) {
    // Check for 'K' key to exit
    if (vkCode == 'K' || vkCode == 'k') {
        m_running = false;
        PostQuitMessage(0);
    }
}
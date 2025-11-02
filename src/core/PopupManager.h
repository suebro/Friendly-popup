#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include "../ui/PopupWindow.h"
#include "../config/Config.h"

class PopupManager {
public:
    PopupManager();
    ~PopupManager();
    
    bool Initialize(const AppConfig& config);
    void Start();
    void Stop();
    void CreatePopup(const std::wstring& message);
    void CloseAllPopups();
    
    void SetMessages(const std::vector<std::wstring>& messages);
    
private:
    void PopupThread();
    
private:
    std::vector<std::unique_ptr<PopupWindow>> m_popups;
    std::vector<std::wstring> m_messages;
    std::thread m_popupThread;
    std::atomic<bool> m_running{false};
    AppConfig m_config;
};
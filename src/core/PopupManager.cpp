#include "PopupManager.h"
#include <random>
#include <chrono>

PopupManager::PopupManager() {}

PopupManager::~PopupManager() {
    Stop();
    CloseAllPopups();
}

bool PopupManager::Initialize(const AppConfig& config) {
    m_config = config;
    PopupWindow::RegisterWindowClass(GetModuleHandle(nullptr));
    return true;
}

void PopupManager::Start() {
    if (m_running) return;

    m_running = true;
    m_popupThread = std::thread(&PopupManager::PopupThread, this);
}

void PopupManager::Stop() {
    m_running = false;
    if (m_popupThread.joinable()) {
        // Post a dummy message to ensure the thread's message loop wakes up
        PostThreadMessage(GetThreadId(static_cast<HANDLE>(m_popupThread.native_handle())), WM_QUIT,0,0);
        m_popupThread.join();
    }
}

void PopupManager::CreatePopup(const std::wstring& message) {
    if (!m_running || m_messages.empty()) return;

    auto popup = std::make_unique<PopupWindow>();

    int x = -1, y = -1;
    if (!m_config.randomPosition) {
        // Implement specific positioning logic if needed
    }

    if (popup->Create(message, x, y, m_config.popupWidth, m_config.popupHeight, m_config.topMost)) {
        popup->SetDuration(m_config.popupDuration);
        m_popups.push_back(std::move(popup));
    }

    // Clean up destroyed popups
    m_popups.erase(
        std::remove_if(m_popups.begin(), m_popups.end(),
            [](const std::unique_ptr<PopupWindow>& popup) {
                // We need a way to check if window is still valid
                // For now, we'll rely on the window self-destruction
                return false;
            }),
        m_popups.end()
    );
}

void PopupManager::CloseAllPopups() {
    for (auto& popup : m_popups) {
        popup->Close();
    }
    m_popups.clear();
}

void PopupManager::SetMessages(const std::vector<std::wstring>& messages) {
    m_messages = messages;
}

void PopupManager::PopupThread() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Schedule next creation time
    auto nextCreation = std::chrono::steady_clock::now();
    const auto interval = std::chrono::milliseconds(m_config.creationInterval);

    MSG msg;
    while (m_running) {
        // Process any pending messages for this thread (so windows created here remain responsive)
        while (PeekMessage(&msg, nullptr,0,0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                m_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!m_running) break;

        auto now = std::chrono::steady_clock::now();
        if (now >= nextCreation) {
            if (!m_messages.empty()) {
                std::uniform_int_distribution<> dis(0, m_messages.size() -1);
                int index = dis(gen);
                CreatePopup(m_messages[index]);
            }
            nextCreation = now + interval;
        }

        // Sleep briefly to avoid busy loop while still being responsive to messages
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Drain any remaining messages
    while (PeekMessage(&msg, nullptr,0,0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
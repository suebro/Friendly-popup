#pragma once
#include <windows.h>
#include <functional>

class KeyboardHook {
public:
    using KeyCallback = std::function<void(DWORD vkCode)>;
    
    KeyboardHook();
    ~KeyboardHook();
    
    bool Install();
    void Uninstall();
    
    void SetKeyCallback(KeyCallback callback);
    
private:
    HHOOK m_hook = nullptr;
    KeyCallback m_callback;
    
    static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
};
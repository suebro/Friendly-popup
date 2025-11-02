#include "KeyboardHook.h"

KeyboardHook* g_keyboardHookInstance = nullptr;

KeyboardHook::KeyboardHook() {
    g_keyboardHookInstance = this;
}

KeyboardHook::~KeyboardHook() {
    Uninstall();
    g_keyboardHookInstance = nullptr;
}

bool KeyboardHook::Install() {
    if (m_hook) return true;
    
    m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(nullptr), 0);
    return m_hook != nullptr;
}

void KeyboardHook::Uninstall() {
    if (m_hook) {
        UnhookWindowsHookEx(m_hook);
        m_hook = nullptr;
    }
}

void KeyboardHook::SetKeyCallback(KeyCallback callback) {
    m_callback = callback;
}

LRESULT CALLBACK KeyboardHook::HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        
        if (g_keyboardHookInstance && g_keyboardHookInstance->m_callback) {
            g_keyboardHookInstance->m_callback(kbdStruct->vkCode);
        }
    }
    
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}
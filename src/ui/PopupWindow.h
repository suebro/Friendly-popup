#pragma once
#include <windows.h>
#include <string>

class PopupWindow {
public:
    PopupWindow();
    ~PopupWindow();
    
    bool Create(const std::wstring& message, int x = -1, int y = -1, 
                int width = 300, int height = 120, bool topMost = true);
    void Close();
    void SetDuration(int milliseconds);
    
    static void RegisterWindowClass(HINSTANCE hInstance);
    static void UnregisterWindowClass();
    
private:
    HWND m_hwnd = nullptr;
    static HBRUSH m_backgroundBrush;
    static COLORREF m_textColor;
    static HFONT m_font;
    
    // Window procedure
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
    // Message handlers
    LRESULT HandleCreate();
    LRESULT HandleTimer(WPARAM timerId);
    LRESULT HandlePaint();
    LRESULT HandleDestroy();
    
    // Utility functions
    static COLORREF GenerateRandomColor();
    static COLORREF GetContrastColor(COLORREF bgColor);
};
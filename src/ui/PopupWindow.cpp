#include "PopupWindow.h"
#include <random>
#include <algorithm>
#include <cstring>

HBRUSH PopupWindow::m_backgroundBrush = nullptr;
COLORREF PopupWindow::m_textColor = RGB(0,0,0);
HFONT PopupWindow::m_font = nullptr;

PopupWindow::PopupWindow() {}

PopupWindow::~PopupWindow() {
 Close();
}

void PopupWindow::RegisterWindowClass(HINSTANCE hInstance) {
 static bool registered = false;
 if (registered) return;
 
 WNDCLASS wc = {};
 wc.lpfnWndProc = WndProc;
 wc.hInstance = hInstance;
 wc.lpszClassName = L"SweetPopupWindow";
 wc.hbrBackground = nullptr; // 使用自定义画刷进行背景绘制
 wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
 wc.style = CS_HREDRAW | CS_VREDRAW;
 
 RegisterClass(&wc);
 registered = true;

 // Create the YaHei font (微软雅黑)
 // Use a reasonable default size (16 logical units) and weight normal
 LOGFONT lf = {};
 lf.lfHeight = -16; // negative for character height in pixels
 lf.lfWeight = FW_NORMAL;
 lf.lfCharSet = DEFAULT_CHARSET;
 // Use a wide-char face name for Microsoft YaHei
 wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Microsoft YaHei");

 if (m_font) {
 DeleteObject(m_font);
 m_font = nullptr;
 }
 m_font = CreateFontIndirect(&lf);
}

void PopupWindow::UnregisterWindowClass() {
 if (m_backgroundBrush) {
 DeleteObject(m_backgroundBrush);
 m_backgroundBrush = nullptr;
 }
 if (m_font) {
 DeleteObject(m_font);
 m_font = nullptr;
 }
}

bool PopupWindow::Create(const std::wstring& message, int x, int y, 
 int width, int height, bool topMost) {
 if (m_hwnd) return false;
 
 // Generate random position if not specified
 if (x == -1 || y == -1) {
 int screenWidth = GetSystemMetrics(SM_CXSCREEN);
 int screenHeight = GetSystemMetrics(SM_CYSCREEN);
 x = rand() % ((screenWidth - width >0) ? (screenWidth - width) :1);
 y = rand() % ((screenHeight - height >0) ? (screenHeight - height) :1);
 }
 
 DWORD exStyle = topMost ? WS_EX_TOPMOST :0;
 DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU;
 
 m_hwnd = CreateWindowEx(
 exStyle,
 L"SweetPopupWindow",
 message.c_str(),
 style,
 x, y, width, height,
 nullptr, nullptr, GetModuleHandle(nullptr), this
 );
 
 if (!m_hwnd) return false;
 
 // Generate random colors
 COLORREF bgColor = GenerateRandomColor();
 m_textColor = GetContrastColor(bgColor);
 
 if (m_backgroundBrush) {
 DeleteObject(m_backgroundBrush);
 }
 m_backgroundBrush = CreateSolidBrush(bgColor);
 
 ShowWindow(m_hwnd, SW_SHOW);
 UpdateWindow(m_hwnd);
 
 return true;
}

void PopupWindow::Close() {
 if (m_hwnd) {
 DestroyWindow(m_hwnd);
 m_hwnd = nullptr;
 }
}

void PopupWindow::SetDuration(int milliseconds) {
 if (m_hwnd) {
 SetTimer(m_hwnd,1, milliseconds, nullptr);
 }
}

LRESULT CALLBACK PopupWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
 PopupWindow* pThis = nullptr;
 
 if (msg == WM_CREATE) {
 CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
 pThis = reinterpret_cast<PopupWindow*>(pCreate->lpCreateParams);
 SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
 } else {
 pThis = reinterpret_cast<PopupWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
 }
 
 if (pThis) {
 switch (msg) {
 case WM_CREATE: return pThis->HandleCreate();
 case WM_TIMER: return pThis->HandleTimer(wParam);
 case WM_PAINT: return pThis->HandlePaint();
 case WM_ERASEBKGND: {
 HDC hdc = reinterpret_cast<HDC>(wParam);
 RECT rc;
 GetClientRect(hwnd, &rc);
 if (PopupWindow::m_backgroundBrush) {
 FillRect(hdc, &rc, PopupWindow::m_backgroundBrush);
 return 1; // 已处理，防止系统默认擦除导致覆盖
 }
 break;
 }
 case WM_DESTROY: return pThis->HandleDestroy();
 }
 }
 
 return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT PopupWindow::HandleCreate() {
 SetDuration(9000); // Default9 seconds
 return 0;
}

LRESULT PopupWindow::HandleTimer(WPARAM timerId) {
 if (timerId ==1) {
 KillTimer(m_hwnd,1);
 Close();
 }
 return 0;
}

LRESULT PopupWindow::HandlePaint() {
 PAINTSTRUCT ps;
 HDC hdc = BeginPaint(m_hwnd, &ps);
 
 // Get client area
 RECT rect;
 GetClientRect(m_hwnd, &rect);

 // 填充背景（确保在 WM_ERASEBKGND 未被调用时也能正确绘制）
 if (m_backgroundBrush) {
 FillRect(hdc, &rect, m_backgroundBrush);
 }

 // Set up colors and font
 SetBkMode(hdc, TRANSPARENT);
 SetTextColor(hdc, m_textColor);

 // Select YaHei font if available
 HFONT oldFont = nullptr;
 if (m_font) {
 oldFont = reinterpret_cast<HFONT>(SelectObject(hdc, m_font));
 }
 
 // Get window text
 wchar_t text[256];
 GetWindowText(m_hwnd, text,256);
 
 // Draw text centered
 DrawText(hdc, text, -1, &rect, 
 DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

 // Restore old font
 if (oldFont) {
 SelectObject(hdc, oldFont);
 }
 
 EndPaint(m_hwnd, &ps);
 return 0;
}

LRESULT PopupWindow::HandleDestroy() {
 m_hwnd = nullptr;
 return 0;
}

COLORREF PopupWindow::GenerateRandomColor() {
 static std::random_device rd;
 static std::mt19937 gen(rd());
 static std::uniform_int_distribution<> dis(50,255); // Avoid too dark colors 
 
 return RGB(dis(gen), dis(gen), dis(gen));
}

COLORREF PopupWindow::GetContrastColor(COLORREF bgColor) {
 int r = GetRValue(bgColor);
 int g = GetGValue(bgColor);
 int b = GetBValue(bgColor);
 
 // Calculate luminance
 double luminance = (0.299 * r +0.587 * g +0.114 * b) /255.0;
 
 return (luminance >0.5) ? RGB(0,0,0) : RGB(255,255,255);
}
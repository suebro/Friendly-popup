
#pragma once
#include <string>

struct AppConfig {
    int popupWidth = 300;
    int popupHeight = 120;
    int popupDuration = 9000; // milliseconds
    int creationInterval = 100; // milliseconds
    bool randomPosition = true;
    bool topMost = true;
    std::wstring messageFile = L"messages.txt";
    
    static AppConfig LoadFromFile(const std::wstring& filePath);
    static void SaveToFile(const AppConfig& config, const std::wstring& filePath);
};
 //123
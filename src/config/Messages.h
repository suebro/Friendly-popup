#pragma once
#include <vector>
#include <string>

namespace Messages {
    extern const std::vector<std::wstring> DEFAULT_MESSAGES;
    
    std::vector<std::wstring> LoadMessagesFromFile(const std::wstring& filePath);
    void SaveMessagesToFile(const std::vector<std::wstring>& messages, const std::wstring& filePath);
}
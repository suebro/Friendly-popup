#include "Messages.h"
#include <fstream>

const std::vector<std::wstring> Messages::DEFAULT_MESSAGES = {
    L"多喝水哦~", L"保持微笑呀", L"每天都要元气满满", L"记得多吃水果",
    L"保持好心情", L"好好爱自己", L"我想你了", L"我喜欢你",
    L"今天也要加油哦", L"你是最棒的", L"休息一下吧", L"记得站起来活动"
};

std::vector<std::wstring> Messages::LoadMessagesFromFile(const std::wstring& filePath) {
    std::vector<std::wstring> messages;
    std::wifstream file(filePath);
    std::wstring line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            messages.push_back(line);
        }
    }
    
    if (messages.empty()) {
        return DEFAULT_MESSAGES;
    }
    
    return messages;
}

void Messages::SaveMessagesToFile(const std::vector<std::wstring>& messages, const std::wstring& filePath) {
    std::wofstream file(filePath);
    for (const auto& message : messages) {
        file << message << L"\n";
    }
}
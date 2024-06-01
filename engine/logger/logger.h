#pragma once

#include <iostream>
#include <string>

class Logger {
public:
    enum LogType {
        INFO = 0,
        WARNING = 1,
        ERROR = 2
    };

    static void Log(const std::int8_t type, const std::string& message);
    static void SaveToFile(const std::string& message);
    static void ClearFromFile();
};
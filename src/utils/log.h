#ifndef _LOG_H_
#define _LOG_H_
#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

// ANSI 颜色代码
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";
}

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
private:
    LogLevel current_level_;
    Logger() : current_level_(LogLevel::INFO) {}
    
    std::string getCurrentTime() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string levelWithColor(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: 
                return Color::CYAN + "DEBUG" + Color::RESET;
            case LogLevel::INFO:  
                return Color::GREEN + "INFO" + Color::RESET;
            case LogLevel::WARN:  
                return Color::YELLOW + "WARN" + Color::RESET;
            case LogLevel::ERROR: 
                return Color::RED + "ERROR" + Color::RESET;
            default: 
                return "UNKNOWN";
        }
    }

    // 处理单个参数的基本情况
    template<typename T>
    void formatMessage(std::ostringstream& oss, T&& arg) {
        oss << std::forward<T>(arg);
    }

    // 处理多个参数的递归情况
    template<typename T, typename... Args>
    void formatMessage(std::ostringstream& oss, T&& arg, Args&&... args) {
        oss << std::forward<T>(arg) << " ";
        formatMessage(oss, std::forward<Args>(args)...);
    }

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void setLevel(LogLevel level) {
        current_level_ = level;
    }

    // 支持多参数的日志方法
    template<typename... Args>
    void log(LogLevel level, Args&&... args) {
        if (level >= current_level_) {
            std::ostringstream oss;
            formatMessage(oss, std::forward<Args>(args)...);

            auto levelStr = levelWithColor(level);
            std::cout << Color::WHITE << "[" << getCurrentTime() << "]"
                     << Color::RESET << "[" << levelStr << "] " 
                     << oss.str() << Color::RESET << std::endl;
        }
    }

    template<typename... Args>
    void debug(Args&&... args) { log(LogLevel::DEBUG, std::forward<Args>(args)...); }

    template<typename... Args>
    void info(Args&&... args) { log(LogLevel::INFO, std::forward<Args>(args)...); }

    template<typename... Args>
    void warn(Args&&... args) { log(LogLevel::WARN, std::forward<Args>(args)...); }

    template<typename... Args>
    void error(Args&&... args) { log(LogLevel::ERROR, std::forward<Args>(args)...); }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#define LOG_DEBUG(...) Logger::getInstance().debug(__VA_ARGS__)
#define LOG_INFO(...)  Logger::getInstance().info(__VA_ARGS__)
#define LOG_WARN(...)  Logger::getInstance().warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::getInstance().error(__VA_ARGS__)
#define SET_LOG_LEVEL(level) Logger::getInstance().setLevel(level)  

#endif
#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace TypeNoodle {

class Logger {
public:
    static void initialize();
    static void shutdown();

    static std::shared_ptr<spdlog::logger>& get() { return s_logger; }

    // Convenience macros-like functions
    template<typename... Args>
    static void trace(Args&&... args) {
        s_logger->trace(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void debug(Args&&... args) {
        s_logger->debug(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(Args&&... args) {
        s_logger->info(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(Args&&... args) {
        s_logger->warn(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(Args&&... args) {
        s_logger->error(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void critical(Args&&... args) {
        s_logger->critical(std::forward<Args>(args)...);
    }

private:
    Logger() = default;
    static std::shared_ptr<spdlog::logger> s_logger;
};

} // namespace TypeNoodle

// Convenience macros
#define TN_TRACE(...) ::TypeNoodle::Logger::trace(__VA_ARGS__)
#define TN_DEBUG(...) ::TypeNoodle::Logger::debug(__VA_ARGS__)
#define TN_INFO(...) ::TypeNoodle::Logger::info(__VA_ARGS__)
#define TN_WARN(...) ::TypeNoodle::Logger::warn(__VA_ARGS__)
#define TN_ERROR(...) ::TypeNoodle::Logger::error(__VA_ARGS__)
#define TN_CRITICAL(...) ::TypeNoodle::Logger::critical(__VA_ARGS__)

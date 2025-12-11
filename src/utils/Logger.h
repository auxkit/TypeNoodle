#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace TypeNoodle {

class Logger {
public:
    static void initialize();
    static void shutdown();

    static std::shared_ptr<spdlog::logger>& get() { return s_logger; }

private:
    Logger() = default;
    static std::shared_ptr<spdlog::logger> s_logger;
};

} // namespace TypeNoodle

// Use spdlog macros directly with runtime format strings to avoid consteval issues
#define TN_TRACE(...) SPDLOG_LOGGER_TRACE(::TypeNoodle::Logger::get(), __VA_ARGS__)
#define TN_DEBUG(...) SPDLOG_LOGGER_DEBUG(::TypeNoodle::Logger::get(), __VA_ARGS__)
#define TN_INFO(...) SPDLOG_LOGGER_INFO(::TypeNoodle::Logger::get(), __VA_ARGS__)
#define TN_WARN(...) SPDLOG_LOGGER_WARN(::TypeNoodle::Logger::get(), __VA_ARGS__)
#define TN_ERROR(...) SPDLOG_LOGGER_ERROR(::TypeNoodle::Logger::get(), __VA_ARGS__)
#define TN_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::TypeNoodle::Logger::get(), __VA_ARGS__)

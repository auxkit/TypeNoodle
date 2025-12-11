#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <QStandardPaths>
#include <QDir>

namespace TypeNoodle {

std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::initialize() {
    try {
        // Get log file path
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir;
        dir.mkpath(appDataPath);

        QString logFilePath = appDataPath + "/typenoodle.log";

        // Create sinks
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        // Rotating file sink (5MB max size, 3 rotated files)
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logFilePath.toStdString(), 1024 * 1024 * 5, 3);
        file_sink->set_level(spdlog::level::trace);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        // Create logger with both sinks
        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        s_logger = std::make_shared<spdlog::logger>("TypeNoodle", sinks.begin(), sinks.end());

        s_logger->set_level(spdlog::level::trace);
        s_logger->flush_on(spdlog::level::err);

        // Register as default logger
        spdlog::register_logger(s_logger);
        spdlog::set_default_logger(s_logger);

        std::string logFilePathStr = logFilePath.toStdString();
        TN_INFO("Logger initialized. Log file: {}", logFilePathStr);

    } catch (const spdlog::spdlog_ex& ex) {
        fprintf(stderr, "Log initialization failed: %s\n", ex.what());
    }
}

void Logger::shutdown() {
    if (s_logger) {
        TN_INFO("Logger shutting down");
        s_logger->flush();
        spdlog::drop_all();
        s_logger.reset();
    }
}

} // namespace TypeNoodle

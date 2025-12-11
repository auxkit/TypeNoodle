#include "Config.h"
#include "Logger.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <fstream>

namespace TypeNoodle {

Config& Config::instance() {
    static Config instance;
    return instance;
}

Config::Config() {
    ensureConfigDirectory();

    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    m_configPath = configDir + "/config.json";

    loadDefaults();
    load();
}

Config::~Config() {
    save();
}

void Config::ensureConfigDirectory() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
}

void Config::loadDefaults() {
    m_config = {
        {"theme", {
            {"dark", true}
        }},
        {"preview", {
            {"defaultText", "The quick brown fox jumps over the lazy dog"},
            {"defaultSize", 48}
        }},
        {"window", {
            {"width", 1280},
            {"height", 800}
        }},
        {"directories", {
            {"custom", nlohmann::json::array()}
        }}
    };
}

void Config::load() {
    try {
        std::ifstream file(m_configPath.toStdString());
        if (file.is_open()) {
            nlohmann::json loaded;
            file >> loaded;
            file.close();

            // Merge loaded config with defaults (preserves new defaults if config is old)
            m_config.merge_patch(loaded);

            TN_INFO("Configuration loaded from: {}", m_configPath.toStdString());
        } else {
            TN_INFO("No configuration file found, using defaults");
        }
    } catch (const std::exception& e) {
        TN_ERROR("Failed to load configuration: {}", e.what());
    }
}

void Config::save() {
    try {
        std::ofstream file(m_configPath.toStdString());
        if (file.is_open()) {
            file << m_config.dump(4);
            file.close();
            TN_DEBUG("Configuration saved to: {}", m_configPath.toStdString());
        } else {
            TN_ERROR("Failed to open configuration file for writing: {}", m_configPath.toStdString());
        }
    } catch (const std::exception& e) {
        TN_ERROR("Failed to save configuration: {}", e.what());
    }
}

QString Config::configFilePath() const {
    return m_configPath;
}

bool Config::getDarkTheme() const {
    return m_config["theme"]["dark"].get<bool>();
}

void Config::setDarkTheme(bool dark) {
    m_config["theme"]["dark"] = dark;
}

QString Config::getDefaultPreviewText() const {
    return QString::fromStdString(m_config["preview"]["defaultText"].get<std::string>());
}

void Config::setDefaultPreviewText(const QString& text) {
    m_config["preview"]["defaultText"] = text.toStdString();
}

int Config::getDefaultPreviewSize() const {
    return m_config["preview"]["defaultSize"].get<int>();
}

void Config::setDefaultPreviewSize(int size) {
    m_config["preview"]["defaultSize"] = size;
}

std::optional<int> Config::getWindowWidth() const {
    if (m_config.contains("window") && m_config["window"].contains("width")) {
        return m_config["window"]["width"].get<int>();
    }
    return std::nullopt;
}

std::optional<int> Config::getWindowHeight() const {
    if (m_config.contains("window") && m_config["window"].contains("height")) {
        return m_config["window"]["height"].get<int>();
    }
    return std::nullopt;
}

void Config::setWindowSize(int width, int height) {
    m_config["window"]["width"] = width;
    m_config["window"]["height"] = height;
}

QStringList Config::getCustomFontDirectories() const {
    QStringList result;
    if (m_config.contains("directories") && m_config["directories"].contains("custom")) {
        for (const auto& dir : m_config["directories"]["custom"]) {
            result.append(QString::fromStdString(dir.get<std::string>()));
        }
    }
    return result;
}

void Config::addCustomFontDirectory(const QString& directory) {
    if (!m_config["directories"]["custom"].contains(directory.toStdString())) {
        m_config["directories"]["custom"].push_back(directory.toStdString());
    }
}

void Config::removeCustomFontDirectory(const QString& directory) {
    auto& dirs = m_config["directories"]["custom"];
    dirs.erase(std::remove(dirs.begin(), dirs.end(), directory.toStdString()), dirs.end());
}

QString Config::collectionsFilePath() const {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    return configDir + "/collections.json";
}

bool Config::isRunningOnApple() const {
#ifdef Q_OS_MACOS
    return true;
#else
    return false;
#endif
}

} // namespace TypeNoodle

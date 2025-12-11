#pragma once

#include <QString>
#include <nlohmann/json.hpp>
#include <optional>

namespace TypeNoodle {

class Config {
public:
    static Config& instance();

    // Load/save configuration
    void load();
    void save();

    // Get configuration file path
    QString configFilePath() const;

    // Application settings
    bool getDarkTheme() const;
    void setDarkTheme(bool dark);

    QString getDefaultPreviewText() const;
    void setDefaultPreviewText(const QString& text);

    int getDefaultPreviewSize() const;
    void setDefaultPreviewSize(int size);

    // Window geometry
    std::optional<int> getWindowWidth() const;
    std::optional<int> getWindowHeight() const;
    void setWindowSize(int width, int height);

    // Font directories
    QStringList getCustomFontDirectories() const;
    void addCustomFontDirectory(const QString& directory);
    void removeCustomFontDirectory(const QString& directory);

    // Collections (stored in separate file, managed by FontManager)
    QString collectionsFilePath() const;

    bool isRunningOnApple() const;

private:
    Config();
    ~Config();
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    void loadDefaults();
    void ensureConfigDirectory();

    nlohmann::json m_config;
    QString m_configPath;
};

} // namespace TypeNoodle

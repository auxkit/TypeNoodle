#include "SettingsManager.h"
#include "Config.h"

namespace TypeNoodle {

SettingsManager::SettingsManager(QObject* parent)
    : QObject(parent) {
}

QStringList SettingsManager::watchFolders() const {
    return Config::instance().getCustomFontDirectories();
}

void SettingsManager::addWatchFolder(const QString& path) {
    if (path.isEmpty()) return;
    
    Config::instance().addCustomFontDirectory(path);
    Config::instance().save();
    emit watchFoldersChanged();
    emit settingsChanged();
}

void SettingsManager::removeWatchFolder(const QString& path) {
    Config::instance().removeCustomFontDirectory(path);
    Config::instance().save();
    emit watchFoldersChanged();
    emit settingsChanged();
}

bool SettingsManager::hasWatchFolder(const QString& path) const {
    return watchFolders().contains(path);
}

QString SettingsManager::defaultPreviewText() const {
    return Config::instance().getDefaultPreviewText();
}

void SettingsManager::setDefaultPreviewText(const QString& text) {
    Config::instance().setDefaultPreviewText(text);
    Config::instance().save();
    emit settingsChanged();
}

int SettingsManager::defaultPreviewSize() const {
    return Config::instance().getDefaultPreviewSize();
}

void SettingsManager::setDefaultPreviewSize(int size) {
    Config::instance().setDefaultPreviewSize(size);
    Config::instance().save();
    emit settingsChanged();
}

void SettingsManager::save() {
    Config::instance().save();
}

} // namespace TypeNoodle

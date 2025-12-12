#pragma once

#include <QObject>
#include <QStringList>

namespace TypeNoodle {

/**
 * QML-accessible wrapper for application settings.
 * Provides methods to manage watch folders and other settings from QML.
 */
class SettingsManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList watchFolders READ watchFolders NOTIFY watchFoldersChanged)

public:
    explicit SettingsManager(QObject* parent = nullptr);
    ~SettingsManager() override = default;

    // Watch folders
    QStringList watchFolders() const;
    Q_INVOKABLE void addWatchFolder(const QString& path);
    Q_INVOKABLE void removeWatchFolder(const QString& path);
    Q_INVOKABLE bool hasWatchFolder(const QString& path) const;

    // Preview settings
    Q_INVOKABLE QString defaultPreviewText() const;
    Q_INVOKABLE void setDefaultPreviewText(const QString& text);
    Q_INVOKABLE int defaultPreviewSize() const;
    Q_INVOKABLE void setDefaultPreviewSize(int size);

    // Save settings to disk
    Q_INVOKABLE void save();

signals:
    void watchFoldersChanged();
    void settingsChanged();
};

} // namespace TypeNoodle

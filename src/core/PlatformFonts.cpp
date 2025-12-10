#include "PlatformFonts.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

namespace TypeNoodle {

QStringList PlatformFonts::systemFontDirectories() {
    QStringList directories;

#ifdef Q_OS_WIN
    // Windows font directories
    directories << "C:/Windows/Fonts";
    QString localAppData = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    directories << localAppData + "/Microsoft/Windows/Fonts";
#elif defined(Q_OS_MACOS)
    // macOS font directories
    directories << "/System/Library/Fonts";
    directories << "/Library/Fonts";
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    directories << homeDir + "/Library/Fonts";
#elif defined(Q_OS_LINUX)
    // Linux font directories
    directories << "/usr/share/fonts";
    directories << "/usr/local/share/fonts";
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    directories << homeDir + "/.fonts";
    directories << homeDir + "/.local/share/fonts";

    // Also check XDG data directories
    QString xdgDataDirs = qEnvironmentVariable("XDG_DATA_DIRS", "/usr/local/share:/usr/share");
    for (const QString& dir : xdgDataDirs.split(':', Qt::SkipEmptyParts)) {
        directories << dir + "/fonts";
    }
#endif

    // Filter to only existing directories
    QStringList existing;
    for (const QString& dir : directories) {
        if (QDir(dir).exists()) {
            existing << dir;
        }
    }

    return existing;
}

QString PlatformFonts::userFontDirectory() {
#ifdef Q_OS_WIN
    QString localAppData = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    return localAppData + "/Microsoft/Windows/Fonts";
#elif defined(Q_OS_MACOS)
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return homeDir + "/Library/Fonts";
#elif defined(Q_OS_LINUX)
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return homeDir + "/.local/share/fonts";
#else
    return QString();
#endif
}

bool PlatformFonts::canInstallFonts(const QString& directory) {
    QFileInfo dirInfo(directory);
    if (!dirInfo.exists()) {
        // Try to create it
        QDir dir;
        if (!dir.mkpath(directory)) {
            return false;
        }
    }

    return QFileInfo(directory).isWritable();
}

} // namespace TypeNoodle

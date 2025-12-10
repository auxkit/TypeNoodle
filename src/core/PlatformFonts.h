#pragma once

#include <QStringList>

namespace TypeNoodle {

class PlatformFonts {
public:
    // Get system font directories for the current platform
    static QStringList systemFontDirectories();

    // Get user font directory for the current platform
    static QString userFontDirectory();

    // Check if a directory is writable for font installation
    static bool canInstallFonts(const QString& directory);

private:
    PlatformFonts() = default;
};

} // namespace TypeNoodle

#include "FontManager.h"
#include "PlatformFonts.h"
#include <QFontDatabase>
#include <QDir>
#include <QDirIterator>
#include <algorithm>

namespace TypeNoodle {

FontManager* FontManager::s_instance = nullptr;

FontManager::FontManager(QObject* parent)
    : QObject(parent) {
    s_instance = this;
}

FontManager::~FontManager() {
    s_instance = nullptr;
}

FontManager* FontManager::instance() {
    return s_instance;
}

void FontManager::refreshFonts() {
    m_fonts.clear();
    scanSystemFonts();
    emit fontsChanged();
}

void FontManager::scanSystemFonts() {
    // Get all font directories
    QStringList directories = PlatformFonts::systemFontDirectories();

    // Scan each directory
    for (const QString& dir : directories) {
        scanDirectory(dir);
    }

    // Also use Qt's font database for already-loaded system fonts
    QFontDatabase database;
    const QStringList families = database.families();

    for (const QString& family : families) {
        const QStringList styles = database.styles(family);
        for (const QString& style : styles) {
            // Try to find the font file path
            // Note: Qt doesn't always expose this, so we may have duplicates
            // or miss some fonts. This is a fallback.

            // Check if we already have this font from directory scanning
            bool found = false;
            for (const auto& font : m_fonts) {
                if (font.family == family && font.style == style) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                // Add font even without file path
                FontInfo info;
                info.family = family;
                info.style = style;
                info.filePath = QString(); // Unknown
                info.format = "System";
                info.weight = database.weight(family, style);
                info.italic = database.italic(family, style);
                m_fonts.push_back(info);
            }
        }
    }
}

void FontManager::scanDirectory(const QString& directory) {
    QDir dir(directory);
    if (!dir.exists()) {
        return;
    }

    // Recursively scan for font files
    QStringList nameFilters;
    nameFilters << "*.ttf" << "*.otf" << "*.ttc" << "*.TTF" << "*.OTF" << "*.TTC";

    QDirIterator it(directory, nameFilters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        addFontFromFile(filePath);
    }
}

void FontManager::addFontFromFile(const QString& filePath) {
    // Load font to get metadata
    int fontId = QFontDatabase::addApplicationFont(filePath);
    if (fontId == -1) {
        return; // Failed to load font
    }

    QStringList families = QFontDatabase::applicationFontFamilies(fontId);
    QFontDatabase database;

    for (const QString& family : families) {
        const QStringList styles = database.styles(family);
        for (const QString& style : styles) {
            FontInfo info(family, style, filePath);
            info.weight = database.weight(family, style);
            info.italic = database.italic(family, style);

            // Check for duplicates
            bool duplicate = false;
            for (const auto& existing : m_fonts) {
                if (existing.family == info.family &&
                    existing.style == info.style &&
                    existing.filePath == info.filePath) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate) {
                m_fonts.push_back(info);
            }
        }
    }

    // Remove the font from application fonts (we just needed metadata)
    QFontDatabase::removeApplicationFont(fontId);
}

void FontManager::activateFont(const QString& fontId) {
    FontInfo* font = findFont(fontId);
    if (font && !font->active) {
        font->active = true;
        emit fontActivationChanged(fontId, true);
        emit fontsChanged();
    }
}

void FontManager::deactivateFont(const QString& fontId) {
    FontInfo* font = findFont(fontId);
    if (font && font->active) {
        font->active = false;
        emit fontActivationChanged(fontId, false);
        emit fontsChanged();
    }
}

FontInfo* FontManager::findFont(const QString& fontId) {
    auto it = std::find_if(m_fonts.begin(), m_fonts.end(),
        [&fontId](const FontInfo& font) { return font.id() == fontId; });

    return it != m_fonts.end() ? &(*it) : nullptr;
}

const FontInfo* FontManager::findFont(const QString& fontId) const {
    auto it = std::find_if(m_fonts.begin(), m_fonts.end(),
        [&fontId](const FontInfo& font) { return font.id() == fontId; });

    return it != m_fonts.end() ? &(*it) : nullptr;
}

void FontManager::createCollection(const QString& name, const QString& description) {
    Collection collection(name, description);
    m_fontCollection.addCollection(collection);
    emit collectionsChanged();
}

void FontManager::deleteCollection(const QString& collectionId) {
    m_fontCollection.removeCollection(collectionId);
    emit collectionsChanged();
}

void FontManager::addFontToCollection(const QString& fontId, const QString& collectionId) {
    m_fontCollection.addFontToCollection(fontId, collectionId);
    emit collectionsChanged();
}

void FontManager::removeFontFromCollection(const QString& fontId, const QString& collectionId) {
    m_fontCollection.removeFontFromCollection(fontId, collectionId);
    emit collectionsChanged();
}

} // namespace TypeNoodle

#include "FontManager.h"
#include "PlatformFonts.h"
#include "../utils/Config.h"
#include <QFontDatabase>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
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
    if (m_isScanning) {
        return;
    }
    
    m_isScanning = true;
    m_scanStatus = "Loading system fonts...";
    m_fonts.clear();
    emit isScanningChanged();
    emit scanStatusChanged();
    
    // Use Qt's font database - this is FAST because it reads from the system font cache
    // No need to scan files manually - Qt already knows all installed fonts
    const QStringList families = QFontDatabase::families();
    
    for (const QString& family : families) {
        const QStringList styles = QFontDatabase::styles(family);
        for (const QString& style : styles) {
            FontInfo info;
            info.family = family;
            info.style = style;
            info.filePath = QString(); // Not available without file scanning
            info.format = "System";
            info.weight = QFontDatabase::weight(family, style);
            info.italic = QFontDatabase::italic(family, style);
            m_fonts.push_back(info);
        }
    }
    
    // Now scan custom/watch directories - these we DO scan for files
    const QStringList customDirs = Config::instance().getCustomFontDirectories();
    if (!customDirs.isEmpty()) {
        m_scanStatus = "Scanning watch folders...";
        emit scanStatusChanged();
        
        for (const QString& dir : customDirs) {
            scanDirectory(dir, m_fonts);
        }
    }
    
    m_isScanning = false;
    m_scanStatus = QString("Loaded %1 fonts").arg(m_fonts.size());
    m_pendingFiles.clear();
    
    emit isScanningChanged();
    emit scanStatusChanged();
    emit fontsChanged();
    emit scanCompleted();
}

void FontManager::processNextBatch() {
    // No longer used
}

void FontManager::finishScanning() {
    // No longer used
}

void FontManager::onScanFinished(const std::vector<FontInfo>& fonts) {
    // No longer used
}

void FontManager::scanSystemFonts() {
    // No longer used - system fonts loaded via QFontDatabase::families()
}

void FontManager::scanDirectory(const QString& directory, std::vector<FontInfo>& fonts) {
    QDir dir(directory);
    if (!dir.exists()) {
        return;
    }
    
    // Supported font extensions
    QStringList filters;
    filters << "*.ttf" << "*.otf" << "*.ttc" << "*.woff" << "*.woff2";
    
    QDirIterator it(directory, filters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        addFontFromFile(filePath, fonts);
    }
}

void FontManager::addFontFromFile(const QString& filePath, std::vector<FontInfo>& fonts) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return;
    }
    
    // Add font to Qt's database and get the font ID
    int fontId = QFontDatabase::addApplicationFont(filePath);
    if (fontId == -1) {
        return; // Failed to load font
    }
    
    // Get families loaded from this font file
    QStringList families = QFontDatabase::applicationFontFamilies(fontId);
    
    // Determine format from extension
    QString format = fileInfo.suffix().toUpper();
    if (format == "TTF") format = "TrueType";
    else if (format == "OTF") format = "OpenType";
    else if (format == "TTC") format = "TrueType Collection";
    else if (format == "WOFF" || format == "WOFF2") format = "Web Font";
    
    for (const QString& family : families) {
        QStringList styles = QFontDatabase::styles(family);
        for (const QString& style : styles) {
            // Check if we already have this font from system fonts
            bool duplicate = false;
            for (const auto& existing : fonts) {
                if (existing.family == family && existing.style == style) {
                    duplicate = true;
                    break;
                }
            }
            
            if (!duplicate) {
                FontInfo info;
                info.family = family;
                info.style = style;
                info.filePath = filePath;
                info.format = format;
                info.weight = QFontDatabase::weight(family, style);
                info.italic = QFontDatabase::italic(family, style);
                fonts.push_back(info);
            }
        }
    }
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

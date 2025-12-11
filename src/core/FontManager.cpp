#include "FontManager.h"
#include "PlatformFonts.h"
#include <QFontDatabase>
#include <QDir>
#include <QDirIterator>
#include <algorithm>

namespace TypeNoodle {

// FontScanWorker implementation
FontScanWorker::FontScanWorker(QObject* parent)
    : QObject(parent) {
}

void FontScanWorker::scanFonts() {
    std::vector<FontInfo> fonts;
    scanSystemFonts(fonts);
    emit scanCompleted(fonts);
}

void FontScanWorker::scanSystemFonts(std::vector<FontInfo>& fonts) {
    emit scanProgress(0, "Discovering font directories...");
    
    // Get all font directories
    QStringList directories = PlatformFonts::systemFontDirectories();
    int totalDirs = directories.size();
    int processedDirs = 0;

    emit scanProgress(5, QString("Found %1 font directories").arg(totalDirs));

    // Scan each directory
    for (const QString& dir : directories) {
        emit scanProgress(5 + (processedDirs * 60 / totalDirs), QString("Scanning: %1").arg(QFileInfo(dir).fileName()));
        scanDirectory(dir, fonts, processedDirs, totalDirs);
        processedDirs++;
    }

    emit scanProgress(70, "Loading system fonts...");

    // Also use Qt's font database for already-loaded system fonts
    QFontDatabase database;
    const QStringList families = database.families();
    int familyCount = 0;
    int totalFamilies = families.size();

    for (const QString& family : families) {
        if (familyCount % 10 == 0) {
            emit scanProgress(70 + (familyCount * 25 / totalFamilies), QString("Processing system font: %1").arg(family));
        }
        
        const QStringList styles = database.styles(family);
        for (const QString& style : styles) {
            // Check if we already have this font from directory scanning
            bool found = false;
            for (const auto& font : fonts) {
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
                fonts.push_back(info);
            }
        }
        familyCount++;
    }

    emit scanProgress(95, QString("Completed scanning %1 fonts").arg(fonts.size()));
}

void FontScanWorker::scanDirectory(const QString& directory, std::vector<FontInfo>& fonts, int& processedDirs, int totalDirs) {
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
        addFontFromFile(filePath, fonts);
    }
}

void FontScanWorker::addFontFromFile(const QString& filePath, std::vector<FontInfo>& fonts) {
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
            for (const auto& existing : fonts) {
                if (existing.family == info.family &&
                    existing.style == info.style &&
                    existing.filePath == info.filePath) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate) {
                fonts.push_back(info);
            }
        }
    }

    // Remove the font from application fonts (we just needed metadata)
    QFontDatabase::removeApplicationFont(fontId);
}

// FontManager implementation
FontManager* FontManager::s_instance = nullptr;

FontManager::FontManager(QObject* parent)
    : QObject(parent)
    , m_workerThread(nullptr)
    , m_worker(nullptr)
    , m_scanProgress(0)
    , m_scanStatus("Ready")
    , m_isScanning(false) {
    s_instance = this;
    
    // Initialize worker thread
    m_workerThread = new QThread(this);
    m_worker = new FontScanWorker();
    m_worker->moveToThread(m_workerThread);
    
    // Connect signals
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &FontManager::destroyed, m_workerThread, &QThread::quit);
    connect(m_worker, &FontScanWorker::scanProgress, this, &FontManager::handleScanProgress);
    connect(m_worker, &FontScanWorker::scanCompleted, this, &FontManager::handleScanCompleted);
    
    m_workerThread->start();
}

FontManager::~FontManager() {
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
    s_instance = nullptr;
}

FontManager* FontManager::instance() {
    return s_instance;
}

void FontManager::refreshFonts() {
    if (m_isScanning) {
        return; // Already scanning
    }
    
    m_isScanning = true;
    emit isScanningChanged(true);
    
    m_scanProgress = 0;
    m_scanStatus = "Starting scan...";
    emit scanProgressChanged(0);
    emit scanStatusChanged(m_scanStatus);
    
    // Start scanning in background thread
    QMetaObject::invokeMethod(m_worker, &FontScanWorker::scanFonts, Qt::QueuedConnection);
}

void FontManager::handleScanProgress(int progress, const QString& status) {
    m_scanProgress = progress;
    m_scanStatus = status;
    emit scanProgressChanged(progress);
    emit scanStatusChanged(status);
}

void FontManager::handleScanCompleted(const std::vector<FontInfo>& fonts) {
    m_fonts = fonts;
    m_scanProgress = 100;
    m_scanStatus = QString("Loaded %1 fonts").arg(fonts.size());
    m_isScanning = false;
    
    emit scanProgressChanged(100);
    emit scanStatusChanged(m_scanStatus);
    emit isScanningChanged(false);
    emit fontsChanged();
    emit scanCompleted();
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

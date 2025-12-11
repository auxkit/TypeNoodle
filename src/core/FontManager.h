#pragma once

#include "FontInfo.h"
#include "FontCollection.h"
#include <QObject>
#include <QThread>
#include <vector>
#include <memory>

namespace TypeNoodle {

// Worker class for scanning fonts in a separate thread
class FontScanWorker : public QObject {
    Q_OBJECT

public:
    explicit FontScanWorker(QObject* parent = nullptr);

public slots:
    void scanFonts();

signals:
    void scanProgress(int progress, const QString& status);
    void scanCompleted(const std::vector<FontInfo>& fonts);

private:
    void scanSystemFonts(std::vector<FontInfo>& fonts);
    void scanDirectory(const QString& directory, std::vector<FontInfo>& fonts, int& processedDirs, int totalDirs);
    void addFontFromFile(const QString& filePath, std::vector<FontInfo>& fonts);
};

class FontManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int scanProgress READ scanProgress NOTIFY scanProgressChanged)
    Q_PROPERTY(QString scanStatus READ scanStatus NOTIFY scanStatusChanged)
    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)

public:
    explicit FontManager(QObject* parent = nullptr);
    ~FontManager() override;

    // Singleton access
    static FontManager* instance();

    // Font discovery and management
    Q_INVOKABLE void refreshFonts();
    Q_INVOKABLE void activateFont(const QString& fontId);
    Q_INVOKABLE void deactivateFont(const QString& fontId);

    // Font queries
    const std::vector<FontInfo>& fonts() const { return m_fonts; }
    FontInfo* findFont(const QString& fontId);
    const FontInfo* findFont(const QString& fontId) const;

    // Progress properties
    int scanProgress() const { return m_scanProgress; }
    QString scanStatus() const { return m_scanStatus; }
    bool isScanning() const { return m_isScanning; }

    // Collection management
    FontCollection& fontCollection() { return m_fontCollection; }
    const FontCollection& fontCollection() const { return m_fontCollection; }

    Q_INVOKABLE void createCollection(const QString& name, const QString& description = QString());
    Q_INVOKABLE void deleteCollection(const QString& collectionId);
    Q_INVOKABLE void addFontToCollection(const QString& fontId, const QString& collectionId);
    Q_INVOKABLE void removeFontFromCollection(const QString& fontId, const QString& collectionId);

signals:
    void fontsChanged();
    void collectionsChanged();
    void fontActivationChanged(const QString& fontId, bool active);
    void scanProgressChanged(int progress);
    void scanStatusChanged(const QString& status);
    void isScanningChanged(bool scanning);
    void scanCompleted();

private slots:
    void handleScanProgress(int progress, const QString& status);
    void handleScanCompleted(const std::vector<FontInfo>& fonts);

private:
    std::vector<FontInfo> m_fonts;
    FontCollection m_fontCollection;
    
    // Threading
    QThread* m_workerThread;
    FontScanWorker* m_worker;
    
    // Progress tracking
    int m_scanProgress;
    QString m_scanStatus;
    bool m_isScanning;

    static FontManager* s_instance;
};

} // namespace TypeNoodle

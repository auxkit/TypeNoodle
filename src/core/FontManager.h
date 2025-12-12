#pragma once

#include "FontInfo.h"
#include "FontCollection.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <vector>
#include <memory>

namespace TypeNoodle {

class FontManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)
    Q_PROPERTY(QString scanStatus READ scanStatus NOTIFY scanStatusChanged)

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
    
    // Scanning status
    bool isScanning() const { return m_isScanning; }
    QString scanStatus() const { return m_scanStatus; }

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
    void isScanningChanged();
    void scanStatusChanged();
    void scanCompleted();

private slots:
    void onScanFinished(const std::vector<FontInfo>& fonts);
    void processNextBatch();
    void finishScanning();

private:
    void scanSystemFonts();
    void scanDirectory(const QString& directory, std::vector<FontInfo>& fonts);
    void addFontFromFile(const QString& filePath, std::vector<FontInfo>& fonts);

    std::vector<FontInfo> m_fonts;
    FontCollection m_fontCollection;
    
    bool m_isScanning = false;
    QString m_scanStatus;
    QMutex m_mutex;
    
    // Batch processing
    QStringList m_pendingFiles;
    int m_currentFileIndex = 0;

    static FontManager* s_instance;
};

} // namespace TypeNoodle

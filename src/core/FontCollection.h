#pragma once

#include <QString>
#include <QStringList>
#include <vector>

namespace TypeNoodle {

struct Collection {
    QString id;
    QString name;
    QString description;
    QStringList fontIds;

    Collection() = default;
    Collection(const QString& name, const QString& description = QString());

    // Check if font is in collection
    bool contains(const QString& fontId) const;

    // Add/remove fonts
    void addFont(const QString& fontId);
    void removeFont(const QString& fontId);
};

class FontCollection {
public:
    FontCollection();

    // Collection management
    void addCollection(const Collection& collection);
    void removeCollection(const QString& collectionId);
    Collection* getCollection(const QString& collectionId);
    const Collection* getCollection(const QString& collectionId) const;

    // Get all collections
    std::vector<Collection>& collections() { return m_collections; }
    const std::vector<Collection>& collections() const { return m_collections; }

    // Find collections containing a font
    QStringList collectionsForFont(const QString& fontId) const;

    // Add/remove font from collection
    void addFontToCollection(const QString& fontId, const QString& collectionId);
    void removeFontFromCollection(const QString& fontId, const QString& collectionId);

private:
    std::vector<Collection> m_collections;
};

} // namespace TypeNoodle

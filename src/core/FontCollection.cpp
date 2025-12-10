#include "FontCollection.h"
#include <QUuid>
#include <algorithm>

namespace TypeNoodle {

Collection::Collection(const QString& name, const QString& description)
    : id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , name(name)
    , description(description) {
}

bool Collection::contains(const QString& fontId) const {
    return fontIds.contains(fontId);
}

void Collection::addFont(const QString& fontId) {
    if (!contains(fontId)) {
        fontIds.append(fontId);
    }
}

void Collection::removeFont(const QString& fontId) {
    fontIds.removeAll(fontId);
}

FontCollection::FontCollection() = default;

void FontCollection::addCollection(const Collection& collection) {
    m_collections.push_back(collection);
}

void FontCollection::removeCollection(const QString& collectionId) {
    m_collections.erase(
        std::remove_if(m_collections.begin(), m_collections.end(),
            [&collectionId](const Collection& c) { return c.id == collectionId; }),
        m_collections.end()
    );
}

Collection* FontCollection::getCollection(const QString& collectionId) {
    auto it = std::find_if(m_collections.begin(), m_collections.end(),
        [&collectionId](const Collection& c) { return c.id == collectionId; });

    return it != m_collections.end() ? &(*it) : nullptr;
}

const Collection* FontCollection::getCollection(const QString& collectionId) const {
    auto it = std::find_if(m_collections.begin(), m_collections.end(),
        [&collectionId](const Collection& c) { return c.id == collectionId; });

    return it != m_collections.end() ? &(*it) : nullptr;
}

QStringList FontCollection::collectionsForFont(const QString& fontId) const {
    QStringList result;
    for (const auto& collection : m_collections) {
        if (collection.contains(fontId)) {
            result.append(collection.name);
        }
    }
    return result;
}

void FontCollection::addFontToCollection(const QString& fontId, const QString& collectionId) {
    if (auto* collection = getCollection(collectionId)) {
        collection->addFont(fontId);
    }
}

void FontCollection::removeFontFromCollection(const QString& fontId, const QString& collectionId) {
    if (auto* collection = getCollection(collectionId)) {
        collection->removeFont(fontId);
    }
}

} // namespace TypeNoodle

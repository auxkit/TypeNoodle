#include "CollectionModel.h"
#include "core/FontManager.h"

namespace TypeNoodle {

CollectionModel::CollectionModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_fontManager(FontManager::instance()) {

    if (m_fontManager) {
        connect(m_fontManager, &FontManager::collectionsChanged,
                this, &CollectionModel::refresh);
    }

    refresh();
}

CollectionModel::~CollectionModel() = default;

int CollectionModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid() || !m_fontManager) {
        return 0;
    }
    return static_cast<int>(m_fontManager->fontCollection().collections().size());
}

QVariant CollectionModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || !m_fontManager) {
        return QVariant();
    }

    const auto& collections = m_fontManager->fontCollection().collections();
    if (index.row() >= static_cast<int>(collections.size())) {
        return QVariant();
    }

    const Collection& collection = collections[index.row()];

    switch (role) {
    case IdRole:
        return collection.id;
    case NameRole:
        return collection.name;
    case DescriptionRole:
        return collection.description;
    case FontCountRole:
        return collection.fontIds.size();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CollectionModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "collectionId";
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[FontCountRole] = "fontCount";
    return roles;
}

void CollectionModel::refresh() {
    beginResetModel();
    endResetModel();
    emit countChanged();
}

void CollectionModel::createCollection(const QString& name, const QString& description) {
    if (m_fontManager) {
        m_fontManager->createCollection(name, description);
    }
}

void CollectionModel::deleteCollection(const QString& collectionId) {
    if (m_fontManager) {
        m_fontManager->deleteCollection(collectionId);
    }
}

} // namespace TypeNoodle

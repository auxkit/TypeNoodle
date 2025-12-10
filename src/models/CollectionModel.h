#pragma once

#include <QAbstractListModel>
#include "core/FontCollection.h"

namespace TypeNoodle {

class FontManager;

class CollectionModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum CollectionRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        FontCountRole
    };

    explicit CollectionModel(QObject* parent = nullptr);
    ~CollectionModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void createCollection(const QString& name, const QString& description = QString());
    Q_INVOKABLE void deleteCollection(const QString& collectionId);

signals:
    void countChanged();

private:
    FontManager* m_fontManager;
};

} // namespace TypeNoodle

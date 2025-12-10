#pragma once

#include <QAbstractListModel>
#include <QString>
#include "core/FontInfo.h"

namespace TypeNoodle {

class FontManager;

class FontListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString filterQuery READ filterQuery WRITE setFilterQuery NOTIFY filterQueryChanged)
    Q_PROPERTY(QString filterCollection READ filterCollection WRITE setFilterCollection NOTIFY filterCollectionChanged)
    Q_PROPERTY(QString filterFormat READ filterFormat WRITE setFilterFormat NOTIFY filterFormatChanged)
    Q_PROPERTY(bool showActiveOnly READ showActiveOnly WRITE setShowActiveOnly NOTIFY showActiveOnlyChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum FontRoles {
        IdRole = Qt::UserRole + 1,
        FamilyRole,
        StyleRole,
        FilePathRole,
        FormatRole,
        WeightRole,
        ItalicRole,
        ActiveRole,
        CollectionsRole,
        DisplayNameRole
    };

    explicit FontListModel(QObject* parent = nullptr);
    ~FontListModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Property getters
    QString filterQuery() const { return m_filterQuery; }
    QString filterCollection() const { return m_filterCollection; }
    QString filterFormat() const { return m_filterFormat; }
    bool showActiveOnly() const { return m_showActiveOnly; }

    // Property setters
    void setFilterQuery(const QString& query);
    void setFilterCollection(const QString& collection);
    void setFilterFormat(const QString& format);
    void setShowActiveOnly(bool activeOnly);

    Q_INVOKABLE void refresh();

signals:
    void filterQueryChanged();
    void filterCollectionChanged();
    void filterFormatChanged();
    void showActiveOnlyChanged();
    void countChanged();

private:
    void updateFilteredFonts();
    bool shouldIncludeFont(const FontInfo& font) const;

    FontManager* m_fontManager;
    std::vector<const FontInfo*> m_filteredFonts;

    QString m_filterQuery;
    QString m_filterCollection;
    QString m_filterFormat;
    bool m_showActiveOnly{false};
};

} // namespace TypeNoodle

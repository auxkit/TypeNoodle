#include "FontListModel.h"
#include "core/FontManager.h"

namespace TypeNoodle {

FontListModel::FontListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_fontManager(FontManager::instance()) {

    if (m_fontManager) {
        connect(m_fontManager, &FontManager::fontsChanged,
                this, &FontListModel::refresh);
        connect(m_fontManager, &FontManager::collectionsChanged,
                this, &FontListModel::refresh);
    }

    refresh();
}

FontListModel::~FontListModel() = default;

int FontListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_filteredFonts.size());
}

QVariant FontListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(m_filteredFonts.size())) {
        return QVariant();
    }

    const FontInfo* font = m_filteredFonts[index.row()];
    if (!font) {
        return QVariant();
    }

    switch (role) {
    case IdRole:
        return font->id();
    case FamilyRole:
        return font->family;
    case StyleRole:
        return font->style;
    case FilePathRole:
        return font->filePath;
    case FormatRole:
        return font->format;
    case WeightRole:
        return font->weight;
    case ItalicRole:
        return font->italic;
    case ActiveRole:
        return font->active;
    case CollectionsRole:
        return font->collections;
    case DisplayNameRole:
        return font->displayName();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FontListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "fontId";
    roles[FamilyRole] = "family";
    roles[StyleRole] = "style";
    roles[FilePathRole] = "filePath";
    roles[FormatRole] = "format";
    roles[WeightRole] = "weight";
    roles[ItalicRole] = "italic";
    roles[ActiveRole] = "active";
    roles[CollectionsRole] = "collections";
    roles[DisplayNameRole] = "displayName";
    return roles;
}

void FontListModel::setFilterQuery(const QString& query) {
    if (m_filterQuery != query) {
        m_filterQuery = query;
        emit filterQueryChanged();
        updateFilteredFonts();
    }
}

void FontListModel::setFilterCollection(const QString& collection) {
    if (m_filterCollection != collection) {
        m_filterCollection = collection;
        emit filterCollectionChanged();
        updateFilteredFonts();
    }
}

void FontListModel::setFilterFormat(const QString& format) {
    if (m_filterFormat != format) {
        m_filterFormat = format;
        emit filterFormatChanged();
        updateFilteredFonts();
    }
}

void FontListModel::setShowActiveOnly(bool activeOnly) {
    if (m_showActiveOnly != activeOnly) {
        m_showActiveOnly = activeOnly;
        emit showActiveOnlyChanged();
        updateFilteredFonts();
    }
}

void FontListModel::refresh() {
    updateFilteredFonts();
}

void FontListModel::updateFilteredFonts() {
    beginResetModel();

    m_filteredFonts.clear();

    if (m_fontManager) {
        const auto& fonts = m_fontManager->fonts();
        for (const auto& font : fonts) {
            if (shouldIncludeFont(font)) {
                m_filteredFonts.push_back(&font);
            }
        }
    }

    endResetModel();
    emit countChanged();
}

bool FontListModel::shouldIncludeFont(const FontInfo& font) const {
    // Filter by search query
    if (!m_filterQuery.isEmpty() && !font.matchesQuery(m_filterQuery)) {
        return false;
    }

    // Filter by format
    if (!m_filterFormat.isEmpty() && font.format != m_filterFormat) {
        return false;
    }

    // Filter by active status
    if (m_showActiveOnly && !font.active) {
        return false;
    }

    // Filter by collection
    if (!m_filterCollection.isEmpty() && !font.collections.contains(m_filterCollection)) {
        return false;
    }

    return true;
}

} // namespace TypeNoodle

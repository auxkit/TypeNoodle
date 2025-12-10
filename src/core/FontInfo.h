#pragma once

#include <QString>
#include <QStringList>
#include <optional>

namespace TypeNoodle {

struct FontInfo {
    QString family;
    QString style;
    QString filePath;
    QString format;
    int weight{400};
    bool italic{false};
    bool active{true};
    QStringList collections;

    // Optional metadata
    std::optional<QString> version;
    std::optional<QString> copyright;
    std::optional<QString> designer;

    // Construct from font database info
    FontInfo() = default;
    FontInfo(const QString& family, const QString& style, const QString& filePath);

    // Unique identifier for this font
    QString id() const;

    // Display name for UI
    QString displayName() const;

    // Check if font matches search query
    bool matchesQuery(const QString& query) const;
};

} // namespace TypeNoodle

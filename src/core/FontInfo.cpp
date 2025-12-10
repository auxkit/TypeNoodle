#include "FontInfo.h"
#include <QCryptographicHash>
#include <QFileInfo>

namespace TypeNoodle {

FontInfo::FontInfo(const QString& family, const QString& style, const QString& filePath)
    : family(family), style(style), filePath(filePath) {

    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();

    if (suffix == "ttf") {
        format = "TrueType";
    } else if (suffix == "otf") {
        format = "OpenType";
    } else if (suffix == "ttc") {
        format = "TrueType Collection";
    } else if (suffix == "woff" || suffix == "woff2") {
        format = "Web Font";
    } else {
        format = "Unknown";
    }
}

QString FontInfo::id() const {
    // Generate unique ID from family + style + path
    QString combined = family + style + filePath;
    return QString(QCryptographicHash::hash(combined.toUtf8(), QCryptographicHash::Md5).toHex());
}

QString FontInfo::displayName() const {
    if (style.isEmpty() || style == "Regular") {
        return family;
    }
    return QString("%1 %2").arg(family, style);
}

bool FontInfo::matchesQuery(const QString& query) const {
    if (query.isEmpty()) {
        return true;
    }

    QString lowerQuery = query.toLower();
    return family.toLower().contains(lowerQuery) ||
           style.toLower().contains(lowerQuery) ||
           format.toLower().contains(lowerQuery);
}

} // namespace TypeNoodle

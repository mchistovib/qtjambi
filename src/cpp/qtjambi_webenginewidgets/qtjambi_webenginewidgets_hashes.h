#ifndef QTJAMBI_WEBENGINEWIDGETS_HASHES_H
#define QTJAMBI_WEBENGINEWIDGETS_HASHES_H

#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <QtCore/QtCore>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QWebEngineScript &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.isNull());
    hashCode = hashCode * 31 + qHash(value.worldId());
    hashCode = hashCode * 31 + qHash(value.sourceCode());
    hashCode = hashCode * 31 + qHash(value.injectionPoint());
    hashCode = hashCode * 31 + qHash(value.runsOnSubFrames());
    return hashCode;
}
#endif

#endif // QTJAMBI_WEBENGINEWIDGETS_HASHES_H

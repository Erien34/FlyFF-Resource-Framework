#pragma once
#include <QColor>
#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

struct ProcessedThemeColors
{
    QMap<QString, QColor> colors;

    QColor get(const QString& key, const QColor& fallback = QColor(255,255,255)) const
    {
        if (colors.contains(key))
            return colors[key];
        return fallback;
    }

    // JSON laden
    bool loadFromJson(const QString& path)
    {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly)) {
            qWarning() << "[ProcessedThemeColors] Konnte Datei nicht öffnen:" << path;
            return false;
        }

        QByteArray data = f.readAll();
        f.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject())
            return false;

        QJsonObject root = doc.object();
        if (!root.contains("colors"))
            return false;

        QJsonObject cols = root["colors"].toObject();
        colors.clear();

        for (auto it = cols.begin(); it != cols.end(); ++it)
            colors.insert(it.key(), QColor(it.value().toString()));

        qInfo() << "[ProcessedThemeColors] JSON geladen →" << colors.size() << "Farben";
        return true;
    }

    // JSON speichern
    bool saveToJson(const QString& path) const
    {
        QJsonObject root;
        QJsonObject obj;

        for (auto it = colors.begin(); it != colors.end(); ++it)
            obj.insert(it.key(), it.value().name(QColor::HexArgb));

        root.insert("colors", obj);

        QJsonDocument doc(root);

        QFile f(path);
        if (!f.open(QIODevice::WriteOnly)) {
            qWarning() << "[ProcessedThemeColors] Speichern fehlgeschlagen:" << path;
            return false;
        }

        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
        return true;
    }
};

// 🔥 GLOBAL – überall nutzbar (Renderer!)
// NICHT vom ThemeManager abhängig!
extern ProcessedThemeColors gProcessedColors;

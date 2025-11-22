#pragma once
#include <QObject>
#include <QMap>
#include <QPixmap>
#include "ControlState.h"
#include "FileManager.h"
#include "ThemeColorExtractor.h"
#include "TokenData.h"
#include "ProcessedThemeColors.h"
#include "ThemeColors.h"

class FileManager;

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    explicit ThemeManager(FileManager* fileMgr, QObject* parent = nullptr);

    struct WindowSkin {
        QPixmap tiles[12];
        bool isTileset = false;
        bool valid = false;
    };

    void refreshFromTokens(const QList<Token>& tokens);
    bool loadTheme(const QString& themeName);
    bool setCurrentTheme(const QString& themeName);
    QString currentTheme() const { return m_currentTheme; }

    QPixmap texture(const QString& key, ControlState state) const;
    WindowSkin resolveWindowSkin(const QString& texName, int wndW, int wndH) const;

    bool loadGameSourceColors(const QString& gameSourcePath);

    QColor color(const QString& key,
                 const QColor& fallback = QColor(255,255,255)) const;

    const ProcessedThemeColors& processedColors() const { return m_processedColors; }
    bool saveUiColorsJson(const QMap<QString, QColor>& colors,
                          const QString& path);

signals:
    void texturesUpdated();
    void themeChanged(const QString& name);

private:
    void clear();
    QMap<ControlState, QPixmap> mapStates(int stateCount,
                                          const QList<QPixmap>& slices,
                                          const QString& texName) const;

    int detectStateCount(const QPixmap& src) const;
    QMap<ControlState, QPixmap> splitTextureByStates(const QPixmap& src) const;

    bool hasTileSet(const QString& baseName) const;
    WindowSkin buildTileSet(const QString& baseName) const;
    bool matchesFullTexture(const QPixmap& pm, int wndW, int wndH) const;
    QPixmap textureFor(const QString& name,
                       ControlState state = ControlState::Normal) const;

    QMap<QString, QPixmap> loadPixmaps(const QString& path,
                                       const QString& themeName);

    FileManager* m_fileMgr = nullptr;
    QString m_currentTheme;

    ProcessedThemeColors m_processedColors;
    ThemeColorExtractor* m_colorExtractor = nullptr;

    QMap<QString, QMap<QString, QMap<ControlState, QPixmap>>> m_themes;

    void applyExtractedColors(const QMap<QString, QColor>& map);
    bool processExtractedColors(const QMap<QString, QColor>& extracted);
};

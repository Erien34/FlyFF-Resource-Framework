#include "ThemeManager.h"
#include "ResourceUtils.h"
#include "FileManager.h"
#include <QDir>
#include <QDebug>
#include <QFileInfo>

ThemeManager::ThemeManager(FileManager* fileMgr, QObject* parent)
    : QObject(parent), m_fileMgr(fileMgr)
{
}

void ThemeManager::refreshFromTokens(const QList<Token>& tokens)
{
    Q_UNUSED(tokens);
    emit texturesUpdated();
}

void ThemeManager::clear()
{
    m_themes.clear();
    m_currentTheme.clear();
}

QMap<QString, QPixmap> ThemeManager::loadPixmaps(const QString& dirPath,
                                                 const QString& themeName)
{
    QMap<QString, QPixmap> result;

    if (dirPath.isEmpty() || !QDir(dirPath).exists()) {
        qWarning() << "[ThemeManager] Ungültiger Theme-Pfad:" << dirPath;
        return result;
    }

    const QStringList filters = { "*.tga", "*.png", "*.jpg", "*.bmp" };
    QDirIterator it(dirPath, filters, QDir::Files, QDirIterator::Subdirectories);

    QString logName = themeName.isEmpty()
                          ? "theme_debug_log.txt"
                          : QString("theme_debug_%1_log.txt").arg(themeName);

    QFile logFile(QDir(dirPath).filePath(logName));
    logFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream log(&logFile);

    int loaded = 0;
    int failed = 0;

    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fi(filePath);

        // 🔹 Der Key ist der Basisname in lowercase
        QString key = fi.baseName().toLower();

        QPixmap pix;

        // TGA mit Flyff-spezialbehandlung
        if (fi.suffix().compare("tga", Qt::CaseInsensitive) == 0) {
            QImage tga = ResourceUtils::loadFlyffTga(filePath);
            if (!tga.isNull())
                pix = QPixmap::fromImage(tga);
        }
        else {
            QImageReader reader(filePath);
            reader.setAutoTransform(true);
            pix = QPixmap::fromImageReader(&reader);
        }

        if (pix.isNull()) {
            log << "❌ Fehler: " << filePath << "\n";
            failed++;
            continue;
        }

        // 🔹 Flyff-typische Korrekturen
        pix = ResourceUtils::clampTransparentEdges(pix);
        pix = ResourceUtils::applyMagentaMask(pix);

        log << "✓ Geladen: " << key << " (" << pix.width() << "x" << pix.height() << ")\n";
        result.insert(key, pix);
        loaded++;
    }

    log << "\nGesamt geladen: " << loaded
        << " | Fehler: " << failed
        << " | Dateien: " << (loaded + failed) << "\n";

    logFile.close();

    qInfo().noquote() << QString("[ThemeManager] Log-Datei: %1")
                             .arg(logFile.fileName());

    return result;
}

bool ThemeManager::loadTheme(const QString& themeName)
{
    if (!m_fileMgr) {
        qWarning() << "[ThemeManager] Kein FileManager gesetzt!";
        return false;
    }

    QString defaultPath = m_fileMgr->defaultThemePath();
    QString themePath   = m_fileMgr->themeFolderPath(themeName);

    if (defaultPath.isEmpty() || !QDir(defaultPath).exists()) {
        qWarning() << "[ThemeManager] Default-Theme nicht gefunden:" << defaultPath;
        return false;
    }

    qInfo().noquote() << "[ThemeManager] Lade Theme:" << themeName;

    // 1️⃣ Default-Theme laden
    QMap<QString, QPixmap> defaultPixmaps =
        loadPixmaps(defaultPath, "Default");

    // 2️⃣ Optionales Theme (z. B. English) laden
    QMap<QString, QPixmap> themePixmaps;
    if (!themePath.isEmpty() && QDir(themePath).exists())
        themePixmaps = loadPixmaps(themePath, themeName);

    // 3️⃣ Theme-Mapping vorbereiten
    QMap<QString, QMap<ControlState, QPixmap>> themeMap;

    auto processPixmaps = [&](const QMap<QString, QPixmap>& pixmaps) {
        for (auto it = pixmaps.begin(); it != pixmaps.end(); ++it) {
            const QString key = it.key().toLower();
            const QPixmap& pix = it.value();

            if (pix.isNull())
                continue;

            QMap<ControlState, QPixmap> states;

            // 🔹 1) Fenster-Tiles NIEMALS slicen
            if (key.startsWith("wndtile")) {
                states[ControlState::Normal] = pix;
            }
            // 🔹 2) Alle anderen "langen" Texturen wie gehabt in States zerlegen
            else if (pix.width() >= pix.height() * 4) {
                states = splitTextureByStates(pix);   // deine detectStateCount-Version
            }
            // 🔹 3) Normale, nicht-Strip Texturen
            else {
                states[ControlState::Normal] = pix;
            }

            themeMap[key] = states;
        }
    };

    // 4️⃣ Default zuerst, dann Custom drüber
    processPixmaps(defaultPixmaps);
    processPixmaps(themePixmaps);

    // 5️⃣ In globale Map eintragen
    m_themes.insert(themeName.toLower(), themeMap);

    qDebug() << "[ThemeManager] Stored theme:" << themeName.toLower()
             << "with" << themeMap.size() << "entries";


    // 6️⃣ Falls noch kein aktives Theme → setzen
    if (m_currentTheme.isEmpty())
        m_currentTheme = themeName.toLower();

    qInfo().noquote() << "[ThemeManager] Theme '" << themeName
                      << "' geladen (" << themeMap.size() << " Texturen)";

    emit texturesUpdated();
    return true;
}


bool ThemeManager::setCurrentTheme(const QString& themeName)
{
    QString lower = themeName.toLower();
    if (!m_themes.contains(lower)) {
        qWarning() << "[ThemeManager] Theme nicht gefunden:" << lower;
        return false;
    }

    if (m_currentTheme == lower)
        return true;

    m_currentTheme = lower;
    qInfo().noquote() << "[ThemeManager] Aktives Theme geändert zu:" << m_currentTheme;
    emit themeChanged(m_currentTheme);
    emit texturesUpdated();
    return true;
}

int ThemeManager::detectStateCount(const QPixmap& src) const
{
    if (src.isNull())
        return 1;

    static const QVector<int> candidates = {4, 6};
    int best = 4;
    int bestRest = INT_MAX;

    for (int c : candidates)
    {
        int rest = src.width() % c;
        if (rest < bestRest)
        {
            bestRest = rest;
            best = c;

            if (rest == 0)
                break; // Perfekter Treffer
        }
    }

    return best;
}

QMap<ControlState, QPixmap> ThemeManager::splitTextureByStates(const QPixmap& src) const
{
    QMap<ControlState, QPixmap> map;

    if (src.isNull())
        return map;

    int stateCount = detectStateCount(src);
    const int W = src.width();
    const int H = src.height();

    int base = W / stateCount;
    int remainder = W % stateCount;

    int x = 0;
    QList<QPixmap> slices;

    for (int i = 0; i < stateCount; ++i)
    {
        int w = base;
        if (remainder > 0) { w++; remainder--; }

        QPixmap slice = src.copy(x, 0, w, H);
        slices << slice;

        x += w;
    }

    return mapStates(stateCount, slices, QString());
}

QPixmap ThemeManager::texture(const QString& name, ControlState state) const
{
    if (m_currentTheme.isEmpty() || !m_themes.contains(m_currentTheme)) {
        qWarning() << "[ThemeManager] Kein aktives Theme!";
        return QPixmap();
    }

    QFileInfo fi(name);
    QString key = fi.completeBaseName().toLower();

    const auto& themeMap = m_themes.value(m_currentTheme);

    auto fetchPixmap = [&](const QMap<QString, QMap<ControlState, QPixmap>>& map,
                           const QString& debugName)
    {
        if (!map.contains(key))
            return QPixmap();

        const auto& states = map.value(key);

        if (states.contains(state))
            return states.value(state);

        if (states.contains(ControlState::Normal))
            return states.value(ControlState::Normal);

        return QPixmap();
    };

    // 1) Aktives Theme
    QPixmap pm = fetchPixmap(themeMap, "Active");
    if (!pm.isNull())
        return pm;

    // 2) Fallback: Default Theme
    if (m_themes.contains("default")) {
        const auto& defMap = m_themes.value("default");
        pm = fetchPixmap(defMap, "Default");

        if (!pm.isNull())
            return pm;
    }

    static QSet<QString> warned;
    if (!warned.contains(key)) {
        warned.insert(key);
        qWarning() << "[ThemeManager] Textur nicht gefunden:" << name;
    }

    return QPixmap();
}

bool ThemeManager::matchesFullTexture(const QPixmap& pm, int wndW, int wndH) const
{
    if (pm.isNull())
        return false;

    return pm.width() == wndW && pm.height() == wndH;
}

bool ThemeManager::hasTileSet(const QString& baseName) const
{
    for (int i = 0; i < 12; i++)
    {
        QString key = QString("%1%2").arg(baseName).arg(i, 2, 10, QChar('0'));

        if (textureFor(key).isNull())
            return false;
    }
    return true;
}

ThemeManager::WindowSkin ThemeManager::buildTileSet(const QString& baseName) const
{
    WindowSkin skin;

    for (int i = 0; i < 12; i++)
    {
        QString key = QString("%1%2").arg(baseName).arg(i, 2, 10, QChar('0'));

        skin.tiles[i] = textureFor(key);

        if (skin.tiles[i].isNull())
            return skin;
    }

    skin.isTileset = true;
    skin.valid = true;
    return skin;
}

QPixmap ThemeManager::textureFor(const QString& name, ControlState state) const
{
    if (!m_themes.contains(m_currentTheme))
        return QPixmap();

    const auto& theme = m_themes[m_currentTheme];

    if (!theme.contains(name))
        return QPixmap();

    const auto& states = theme[name];

    if (states.contains(state))
        return states[state];

    // Fallback auf Normal
    if (states.contains(ControlState::Normal))
        return states[ControlState::Normal];

    return QPixmap();
}

ThemeManager::WindowSkin ThemeManager::resolveWindowSkin(
    const QString& texName, int wndW, int wndH) const
{
    WindowSkin ws;

    if (texName.isEmpty())
        return ws;

    // 1) Prüfe TileSet
    if (hasTileSet(texName))
    {
        ws = buildTileSet(texName);
        if (ws.valid)
            return ws;
    }

    // 2) Prüfe FullTexture
    QPixmap pm = textureFor(texName);
    if (matchesFullTexture(pm, wndW, wndH))
    {
        ws.tiles[0] = pm;
        ws.valid = true;
        ws.isTileset = false;
        return ws;
    }

    // 3) Fallback (keine Erkennung)
    return ws;
}

bool ThemeManager::loadGameSourceColors(const QString& gameSourcePath)
{
    if (gameSourcePath.isEmpty())
        return false;

    delete m_colorExtractor;

    // FIX ✔️ Der Extractor benötigt den Source-Root, nicht den FileManager
    m_colorExtractor = new ThemeColorExtractor(gameSourcePath);

    // NEU ✔️ Wir extrahieren ALLE Farben, nicht nur UI-Farben
    QMap<QString, QColor> extracted = m_colorExtractor->extractAllColors();

    if (extracted.isEmpty()) {
        qWarning() << "[ThemeManager] WARNING: No colors extracted from source.";
        return false;
    }

    // NEU ✔️ Vereinheitlichen & in m_processedColors speichern
    if (!processExtractedColors(extracted))
        return false;

    qDebug() << "[ThemeManager] Loaded" << extracted.size()
             << "colors from client source (unified format).";

    return true;
}

/*
 * NEU:
 * Vereinheitlicht ALLE extrahierten Farben in EIN Format (QColor RGBA)
 * und schreibt sie in m_processedColors.colors
 */
bool ThemeManager::processExtractedColors(const QMap<QString, QColor>& extracted)
{
    if (extracted.isEmpty())
        return false;

    m_processedColors.colors.clear();

    for (auto it = extracted.begin(); it != extracted.end(); ++it)
    {
        const QString& key = it.key();
        const QColor& col = it.value();

        if (!col.isValid())
            continue;

        // Einheitliches RGBA-Format aus jedem möglichen Ursprung
        QColor unified(col.red(), col.green(), col.blue(), col.alpha());

        m_processedColors.colors.insert(key, unified);
    }

    // Weiter an Theme-System übergeben
    applyExtractedColors(m_processedColors.colors);

    return true;
}

/*
 * Bestehende Funktion – UNVERÄNDERT!
 * Wir übernehmen nur die unified colors.
 */
void ThemeManager::applyExtractedColors(const QMap<QString, QColor>& map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        m_processedColors.colors[it.key()] = it.value();
}

QColor ThemeManager::color(const QString& key, const QColor& fallback) const
{
    return m_processedColors.get(key, fallback);
}

bool ThemeManager::saveUiColorsJson(const QMap<QString, QColor>& colors,
                                    const QString& path)
{
    QJsonObject root;
    QJsonObject colObj;

    for (auto it = colors.begin(); it != colors.end(); ++it)
        colObj.insert(it.key(), it.value().name(QColor::HexArgb));

    root.insert("colors", colObj);

    return m_fileMgr->saveJsonObject(path, root);
}

QMap<ControlState, QPixmap> ThemeManager::mapStates(
    int stateCount,
    const QList<QPixmap>& slices,
    const QString& texName) const
{
    QMap<ControlState, QPixmap> map;

    if (stateCount == 1)
    {
        map[ControlState::Normal] = slices[0];
        return map;
    }

    // 🔥 CLIENT-KORREKT für Buttons (4 States)
    if (stateCount == 4)
    {
        // EXAKTE CLIENT-ANORDNUNG:
        // 0 = HOVER
        // 1 = NORMAL
        // 2 = PRESSED
        // 3 = DISABLED
        map[ControlState::Hover]    = slices[0];
        map[ControlState::Normal]   = slices[1];
        map[ControlState::Pressed]  = slices[2];
        map[ControlState::Disabled] = slices[3];

        return map;
    }

    // 6-State (Radio / Check)
    if (stateCount == 6)
    {
        map[ControlState::Normal]        = slices[0];
        map[ControlState::Hover]         = slices[1];
        map[ControlState::Pressed]       = slices[2];
        map[ControlState::CheckedNormal] = slices[3];
        map[ControlState::CheckedHover]  = slices[4];
        map[ControlState::CheckedPressed]= slices[5];
        return map;
    }

    // Fallback
    map[ControlState::Normal] = slices[0];
    return map;
}

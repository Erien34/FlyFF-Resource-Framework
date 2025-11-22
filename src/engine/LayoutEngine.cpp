// layout/LayoutEngine.cpp
#include "LayoutEngine.h"

#include "theme/ThemeManager.h"
#include "BehaviorManager.h"
#include "layout/model/ControlData.h"
#include "ControlTypeMapper.h"

#include <QDebug>
#include <limits>

// ------------------------------------------------------------
// Hilfsfunktionen: Editor-freundliche FlyFF-Metriken
// ------------------------------------------------------------

static int HeightFor(ControlType t)
{
    switch (t)
    {
    case ControlType::Button:          return 24;
    case ControlType::Edit:            return 18;
    case ControlType::Static:          return 20;   // Labels im Editor fix 20px
    case ControlType::CheckBox:        return 14;
    case ControlType::RadioButton:     return 14;

    case ControlType::ScrollBarV:      return 14;
    case ControlType::ScrollBarH:      return 12;
    case ControlType::ScrollBarThumbV: return 4;
    case ControlType::ScrollBarThumbH: return 4;

    case ControlType::Progress:        return 12;
    case ControlType::ListBox:         return 16;

    default:
        return 20; // Editor-Fallback
    }
}

// Kleine Typ-Offsets, damit es näher an FlyFF aussieht.
static QPoint TypeOffset(ControlType t)
{
    switch (t)
    {
    case ControlType::Button:      return QPoint(0, -1);
    case ControlType::Static:      return QPoint(0, -2);
    case ControlType::Edit:        return QPoint(0,  1);
    case ControlType::CheckBox:
    case ControlType::RadioButton: return QPoint(-1, 0);
    default:                       return QPoint(0, 0);
    }
}

struct WindowPadding {
    int left, top, right, bottom;
};

// Manuelle Korrekturen (FlyFF-verifiziert)
static QMap<QString, WindowPadding> g_WindowPaddingTable = {
    { "APP_LOGIN",        {12, 8, 10, 6} },
    { "APP_PARTY",        {14, 6, 12, 4} },
    { "APP_MESSENGER",    {10, 4,  8, 4} },
    { "APP_GUILD",        {10, 6, 10, 6} },
    { "APP_MSGBOX",        {8, 6,  8, 6} },
    };

// Standardwerte (für Fenster, die NICHT in der Tabelle stehen)
static WindowPadding DefaultPadding = {12, 6, 10, 6};

// ============================================================
// LayoutEngine
// ============================================================

LayoutEngine::LayoutEngine(ThemeManager* themeMgr,
                           BehaviorManager* behaviorMgr)
    : m_themeMgr(themeMgr)
    , m_behaviorMgr(behaviorMgr)
{
}

WindowRenderInfo LayoutEngine::computeWindowLayout(
    const std::shared_ptr<WindowData>& wnd,
    const QSize& canvasSize) const
{
    WindowRenderInfo info;
    if (!wnd || !m_themeMgr)
        return info;

    // 1) Fensterrechteck im Canvas bestimmen (ResData-Größe, zentriert)
    const QRect wndRect = computeWindowRectCentered(*wnd, canvasSize);

    // 2) Content-Bereich anhand des Tilesets bestimmen (AUTOMATISCHES PADDING)
    const QRect contentRect = computeContentRectFromTiles(wndRect);

    // 3) Controls normalisieren und fertige RenderInfos erzeugen
    const auto controlsInfo = computeControlsLayout(*wnd, contentRect);

    info.windowData      = wnd;
    info.windowRect      = wndRect;
    info.contentRect     = contentRect;
    info.titleBarRect    = QRect(wndRect.left(),
                              wndRect.top(),
                              wndRect.width(),
                              contentRect.top() - wndRect.top());
    info.closeButtonRect = QRect(); // Behavior/Theme können das später füllen
    info.helpButtonRect  = QRect();
    info.controls        = controlsInfo;

    return info;
}

// -----------------------------------------------------------------------------
// Fensterposition – Größe 1:1 aus ResData, nur zentriert
// -----------------------------------------------------------------------------
QRect LayoutEngine::computeWindowRectCentered(const WindowData& wnd,
                                              const QSize& canvasSize) const
{
    int width  = 0;
    int height = 0;

    // TODO: Falls deine WindowData andere Feldnamen hat, hier anpassen.
    //       Z.B. wnd.w, wnd.h oder wnd.rect etc.
    width  = wnd.x;   // ResData: z.B. APP_LOGIN ... 288 256 ...
    height = wnd.y;

    // Fallback, falls alte Daten/ungültig
    if (width <= 0)  width  = 320;
    if (height <= 0) height = 240;

    QRect rect(0, 0, width, height);

    const int cx = canvasSize.width()  / 2 - rect.width()  / 2;
    const int cy = canvasSize.height() / 2 - rect.height() / 2;
    rect.moveTopLeft(QPoint(cx, cy));

    return rect;
}

// -----------------------------------------------------------------------------
// ContentRect aus Tileset-Metriken (AUTOMATISCHES PADDING)
// -----------------------------------------------------------------------------
QRect LayoutEngine::computeContentRectFromTiles(const QRect& wndRect) const
{
    auto tile = [&](int i) -> QPixmap {
        QString key = QString("wndtile%1").arg(i, 2, 10, QChar('0'));
        return m_themeMgr->texture(key, ControlState::Normal);
    };

    QPixmap t01 = tile(1);   // Top gold bar
    QPixmap t04 = tile(4);   // Title/header
    QPixmap t06 = tile(6);   // Left border
    QPixmap t08 = tile(8);   // Right border
    QPixmap t10 = tile(10);  // Bottom border

    // 1) Basis-Padding aus Tiles
    int tileLeft   = t06.isNull() ? 0 : t06.width();
    int tileRight  = t08.isNull() ? 0 : t08.width();
    int tileBottom = t10.isNull() ? 0 : t10.height();

    int tileTop = 0;
    if (!t01.isNull()) tileTop += t01.height();
    if (!t04.isNull()) tileTop += t04.height();
    tileTop += 6; // FlyFF-typisches Abstand nach Header

    // 2) Fenstername ermitteln
    QString wnd = m_currentWindow.toUpper();

    // 3) Manuelles Padding (falls vorhanden)
    WindowPadding pad;
    if (g_WindowPaddingTable.contains(wnd))
        pad = g_WindowPaddingTable[wnd];
    else
        pad = DefaultPadding;

    // 4) Kombiniertes Padding (Tiles + WindowPadding)
    int finalLeft   = tileLeft   + pad.left;
    int finalTop    = tileTop    + pad.top;
    int finalRight  = tileRight  + pad.right;
    int finalBottom = tileBottom + pad.bottom;

    // 5) ContentRect berechnen
    QRect content = wndRect.adjusted(
        finalLeft,
        finalTop,
        -finalRight,
        -finalBottom
        );

    // 6) Optional minimaler Innenabstand
    content.adjust(2, 1, -2, -2);

    return content;
}

// -----------------------------------------------------------------------------
// Controls normalisieren
// -----------------------------------------------------------------------------
std::vector<ControlRenderInfo> LayoutEngine::computeControlsLayout(
    const WindowData& wnd,
    const QRect& contentRect) const
{
    std::vector<ControlRenderInfo> out;
    if (wnd.controls.empty())
        return out;

    // 1) BoundingBox im Originalkoordinatensystem
    int minX  = std::numeric_limits<int>::max();
    int minY  = std::numeric_limits<int>::max();
    int maxX1 = std::numeric_limits<int>::min();
    int maxY1 = std::numeric_limits<int>::min();

    for (const auto& c : wnd.controls) {
        minX  = std::min(minX, c->x);
        minY  = std::min(minY, c->y);
        maxX1 = std::max(maxX1, c->x1);
        maxY1 = std::max(maxY1, c->y1);
    }

    // Original-Abmessungen der Controls
    int bbW = maxX1 - minX;
    int bbH = maxY1 - minY;

    // 2) Detect AUTOMATIC padding correction
    int contentW = contentRect.width();
    int contentH = contentRect.height();

    int extraLeft   = std::max(0, -minX);
    int extraTop    = std::max(0, -minY);
    int extraRight  = std::max(0, (maxX1 - minX) - contentW);
    int extraBottom = std::max(0, (maxY1 - minY) - contentH);

    // 3) Controls innerhalb des Fensters platzieren
    int baseOffsetX = contentRect.x() - minX - extraLeft;
    int baseOffsetY = contentRect.y() - minY - extraTop;

    // 4) Controls bauen
    for (const auto& ctrl : wnd.controls)
    {
        ControlRenderInfo info;
        info.data = ctrl;
        info.state = ControlState::Normal;
        info.data->mappedType = ControlTypeMapper::map(ctrl->type);

        int width  = ctrl->x1 - ctrl->x;
        int height = HeightFor(info.data->mappedType);

        int x = ctrl->x + baseOffsetX;
        int y = ctrl->y + baseOffsetY;

        // Typabhängige Korrektur
        QPoint tOff = TypeOffset(info.data->mappedType);
        x += tOff.x();
        y += tOff.y();

        info.renderRect = QRect(x, y, width, height);
        info.clipRect = contentRect;

        out.push_back(info);
    }

    return out;
}

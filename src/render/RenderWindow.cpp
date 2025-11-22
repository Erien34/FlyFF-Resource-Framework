#include "RenderWindow.h"
#include "layout/model/WindowData.h"
#include "theme/ThemeManager.h"
#include "behavior/BehaviorManager.h"

#include <QPainter>
#include <QDebug>

RenderWindow::RenderWindow(ThemeManager* themeManager,
                           BehaviorManager* behaviorManager,
                           QObject* parent)
    : QObject(parent)
    , m_themeManager(themeManager)
    , m_behaviorManager(behaviorManager)
{
}

void RenderWindow::render(QPainter& p, const WindowRenderInfo& info)
{
    if (!info.windowData || !m_themeManager || !m_behaviorManager)
        return;

    const auto& wnd  = info.windowData;
    const QRect& wndRect = info.windowRect;

    bool drawn = false;

    if (drawDirectWindowTexture(p, info))
        drawn = true;

    if (!drawn)
        drawn = drawWindowTileset(p, info);

    if (!drawn)
        drawFallbackWindow(p, info);

    drawTitleAndButtons(p, info);
}


// ============================================================================
// DIRECT TEXTURE
// ============================================================================
bool RenderWindow::drawDirectWindowTexture(QPainter& p,
                                           const WindowRenderInfo& info)
{
    const auto& wnd = info.windowData;
    const QRect& wndRect = info.windowRect;

    if (!m_themeManager || wnd->texture.isEmpty())
        return false;

    const QPixmap& tex =
        m_themeManager->texture(wnd->texture, ControlState::Normal);

    if (tex.isNull())
        return false;

    if (tex.width() != wndRect.width() || tex.height() != wndRect.height())
        return false;

    p.save();
    p.setOpacity(1.0);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawPixmap(wndRect, tex);
    p.restore();

    return true;
}


// ============================================================================
// TILESET
// ============================================================================
bool RenderWindow::drawWindowTileset(QPainter& p, const WindowRenderInfo& info)
{
    if (!m_themeManager)
        return false;

    const QRect& area = info.windowRect;

    auto tile = [&](int i) -> QPixmap {
        QString key = QStringLiteral("wndtile%1")
        .arg(i, 2, 10, QChar('0'));
        return m_themeManager->texture(key, ControlState::Normal);
    };

    QPixmap t00 = tile(0), t01 = tile(1), t02 = tile(2);
    QPixmap t03 = tile(3), t04 = tile(4), t05 = tile(5);
    QPixmap t06 = tile(6), t07 = tile(7), t08 = tile(8);
    QPixmap t09 = tile(9), t10 = tile(10), t11 = tile(11);

    if (t00.isNull() && t01.isNull() && t02.isNull() &&
        t03.isNull() && t04.isNull() && t05.isNull() &&
        t06.isNull() && t07.isNull() && t08.isNull() &&
        t09.isNull() && t10.isNull() && t11.isNull())
    {
        return false;
    }

    p.save();
    p.setOpacity(1.0);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    const int topH    = (t01.isNull() ? 0 : t01.height())
                     + (t04.isNull() ? 0 : t04.height());
    const int leftW   = t06.isNull() ? 0 : t06.width();
    const int rightW  = t08.isNull() ? 0 : t08.width();
    const int bottomH = t10.isNull() ? 0 : t10.height();

    QRect innerRect = area.adjusted(leftW,
                                    topH,
                                    -rightW,
                                    -bottomH);

    if (!t07.isNull())
        p.drawTiledPixmap(innerRect, t07);
    else
        p.fillRect(innerRect, QColor(30, 30, 30));

    if (!t00.isNull())
        p.drawPixmap(area.topLeft(), t00);

    if (!t01.isNull())
    {
        int x = area.left() + t00.width();
        int w = area.width() - t00.width() - t02.width();
        QRect rTop(x, area.top(), w, t01.height());
        p.drawTiledPixmap(rTop, t01);
    }

    if (!t02.isNull())
        p.drawPixmap(QPoint(area.right() - t02.width() + 1, area.top()), t02);

    const int headerY = area.top() + (t01.isNull() ? 0 : t01.height());

    if (!t03.isNull())
        p.drawPixmap(QPoint(area.left(), headerY), t03);

    if (!t04.isNull())
    {
        int x = area.left() + t03.width();
        int w = area.width() - t03.width() - t05.width();
        QRect rHeader(x, headerY, w, t04.height());
        p.drawTiledPixmap(rHeader, t04);
    }

    if (!t05.isNull())
        p.drawPixmap(QPoint(area.right() - t05.width() + 1, headerY), t05);

    const int sideTop    = headerY + (t04.isNull() ? 0 : t04.height());
    const int sideBottom = area.bottom() - bottomH;
    const int sideHeight = qMax(0, sideBottom - sideTop + 1);

    if (!t06.isNull())
    {
        QRect rLeft(area.left(), sideTop, t06.width(), sideHeight);
        p.drawTiledPixmap(rLeft, t06);
    }

    if (!t08.isNull())
    {
        QRect rRight(area.right() - t08.width() + 1, sideTop, t08.width(), sideHeight);
        p.drawTiledPixmap(rRight, t08);
    }

    const int bottomY = area.bottom() - bottomH + 1;

    if (!t09.isNull())
        p.drawPixmap(QPoint(area.left(), bottomY), t09);

    if (!t10.isNull())
    {
        int x = area.left() + t09.width();
        int w = area.width() - t09.width() - t11.width();
        QRect rBottom(x, bottomY, w, t10.height());
        p.drawTiledPixmap(rBottom, t10);
    }

    if (!t11.isNull())
        p.drawPixmap(QPoint(area.right() - t11.width() + 1, bottomY), t11);

    p.restore();
    return true;
}


// ============================================================================
// FALLBACK
// ============================================================================
void RenderWindow::drawFallbackWindow(QPainter& p,
                                      const WindowRenderInfo& info)
{
    const QRect& wndRect = info.windowRect;

    p.save();
    p.fillRect(wndRect, QColor(40, 40, 40));
    p.setPen(QColor(200, 200, 200));
    p.drawRect(wndRect.adjusted(0, 0, -1, -1));
    p.restore();
}


// ============================================================================
// TITLE + BUTTONS
// ============================================================================
void RenderWindow::drawTitleAndButtons(QPainter& p,
                                       const WindowRenderInfo& info)
{
    const auto& wnd     = info.windowData;
    const QRect& wndRect = info.windowRect;

    bool wantClose = wnd->resolvedMask.contains("has_close");
    bool wantHelp  = wnd->resolvedMask.contains("has_help");

    if (!wantClose && !wantHelp)
        return;

    QPixmap t01 = m_themeManager->texture("wndtile01", ControlState::Normal);
    QPixmap t02 = m_themeManager->texture("wndtile02", ControlState::Normal);

    int hTop    = t01.isNull() ? 10 : t01.height();
    int btnSize = 12;

    int btnY = wndRect.top() + hTop - (btnSize / 2);
    int cursorX = wndRect.right() - 12;
    const int spacing = 3;

    if (wantClose)
    {
        QPixmap tex = m_themeManager->texture("buttwndexit", ControlState::Normal);
        int w = tex.isNull() ? btnSize : tex.width();
        int h = tex.isNull() ? btnSize : tex.height();

        QRect r(cursorX - w, btnY, w, h);

        if (!tex.isNull()) drawCloseButton(p, r);
        else               drawFallbackButton(p, r, QColor(255,200,80), "X");

        cursorX -= w + spacing;
    }

    if (wantHelp)
    {
        QPixmap tex = m_themeManager->texture("buttwndhelp", ControlState::Normal);
        int w = tex.isNull() ? btnSize : tex.width();
        int h = tex.isNull() ? btnSize : tex.height();

        QRect r(cursorX - w, btnY, w, h);

        if (!tex.isNull()) drawHelpButton(p, r);
        else               drawFallbackButton(p, r, QColor(150,200,255), "?");

        cursorX -= w + spacing;
    }
}

void RenderWindow::drawFallbackButton(QPainter& p,
                                      const QRect& rect,
                                      const QColor& color,
                                      const QString& text)
{
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    p.drawRect(rect);

    p.setPen(Qt::black);
    p.drawText(rect, Qt::AlignCenter, text);
    p.restore();
}

void RenderWindow::drawCloseButton(QPainter& p, const QRect& rect)
{
    const QPixmap& tex =
        m_themeManager->texture("buttwndexit", ControlState::Normal);

    if (!tex.isNull())
        p.drawPixmap(rect, tex);
    else
        p.fillRect(rect, Qt::red);
}

void RenderWindow::drawHelpButton(QPainter& p, const QRect& rect)
{
    const QPixmap& tex =
        m_themeManager->texture("buttwndhelp", ControlState::Normal);

    if (!tex.isNull())
        p.drawPixmap(rect, tex);
    else
        p.fillRect(rect, Qt::blue);
}

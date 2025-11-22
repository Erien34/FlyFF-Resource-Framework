#include "render/controls/EditBackground.h"
#include "layout/ControlLayout.h"
#include "theme/ThemeManager.h"

namespace RenderHelpers
{

void drawEditBackground(QPainter& p,
                        const QRect& rect,
                        ThemeManager* theme,
                        const ControlRenderInfo& info)
{
    if (!theme)
        return;

    p.save();

    // FlyFF EditBox Alpha
    constexpr qreal FLYFF_WINDOW_ALPHA = 200.0 / 255.0;
    p.setOpacity(FLYFF_WINDOW_ALPHA);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    //
    // 1) Tileset-Prefix automatisch erkennen
    //
    const QStringList candidates = {
        "wndedittile",
        "edit"
    };

    QString prefix;

    for (const QString& c : candidates)
    {
        QString k = QString("%1%2")
        .arg(c)
            .arg(0, 2, 10, QChar('0'));

        QPixmap test = theme->texture(k, info.state);

        if (!test.isNull()) {
            prefix = c;
            break;
        }
    }

    //
    // 2) Falls kein Tileset → Fallback
    //
    if (prefix.isEmpty())
    {
        p.fillRect(rect, QColor(20,20,20));
        p.setPen(QColor(0,0,0,150));
        p.drawRect(rect.adjusted(0,0,-1,-1));
        p.restore();
        return;
    }

    //
    // 3) Tiles holen
    //
    auto getTile = [&](int idx) -> QPixmap {
        QString key = QString("%1%2")
        .arg(prefix)
            .arg(idx, 2, 10, QChar('0'));
        return theme->texture(key, info.state);
    };

    QPixmap tl = getTile(0);
    QPixmap tm = getTile(1);
    QPixmap tr = getTile(2);
    QPixmap ml = getTile(3);
    QPixmap mm = getTile(4);
    QPixmap mr = getTile(5);
    QPixmap bl = getTile(6);
    QPixmap bm = getTile(7);
    QPixmap br = getTile(8);

    //
    // 4) Tilegröße (Fallback falls mm leer ist)
    //
    int tileW = mm.isNull() ? 4 : mm.width();
    int tileH = mm.isNull() ? 4 : mm.height();

    //
    // 5) Mitte tiled füllen
    //
    for (int y = rect.top(); y < rect.bottom(); y += tileH)
        for (int x = rect.left(); x < rect.right(); x += tileW)
            p.drawPixmap(QRect(x, y, tileW, tileH), mm);

    //
    // 6) Kanten
    //
    if (!tm.isNull())
        p.drawTiledPixmap(
            QRect(rect.left() + tl.width(),
                  rect.top(),
                  rect.width() - tl.width() - tr.width(),
                  tm.height()),
            tm
            );

    if (!bm.isNull())
        p.drawTiledPixmap(
            QRect(rect.left() + bl.width(),
                  rect.bottom() - bm.height(),
                  rect.width() - bl.width() - br.width(),
                  bm.height()),
            bm
            );

    if (!ml.isNull())
        p.drawTiledPixmap(
            QRect(rect.left(),
                  rect.top() + tl.height(),
                  ml.width(),
                  rect.height() - tl.height() - bl.height()),
            ml
            );

    if (!mr.isNull())
        p.drawTiledPixmap(
            QRect(rect.right() - mr.width(),
                  rect.top() + tr.height(),
                  mr.width(),
                  rect.height() - tr.height() - br.height()),
            mr
            );

    //
    // 7) Ecken
    //
    if (!tl.isNull()) p.drawPixmap(rect.topLeft(), tl);

    if (!tr.isNull())
        p.drawPixmap(rect.topRight() - QPoint(tr.width(), 0), tr);

    if (!bl.isNull())
        p.drawPixmap(rect.bottomLeft() - QPoint(0, bl.height()), bl);

    if (!br.isNull())
        p.drawPixmap(rect.bottomRight() - QPoint(br.width(), br.height()), br);

    p.restore();
}


} // namespace RenderHelpers

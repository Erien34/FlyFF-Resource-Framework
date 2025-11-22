#include "render/controls/RenderTabControl.h"
#include "layout/ControlLayout.h"
#include "layout/model/ControlData.h"
#include "theme/ThemeManager.h"

void RenderTabControl::render(QPainter& p,
                              const ControlRenderInfo& info,
                              ThemeManager* /*theme*/)
{
    QRect r = info.renderRect;

    p.save();

    // Tab-Leiste
    int tabHeight = 20;
    QRect barRect(r.left(), r.top(), r.width(), tabHeight);
    p.fillRect(barRect, QColor(40, 40, 70));

    // Tabs (Platzhalter: 3 Tabs)
    int tabW = r.width() / 3;
    for (int i = 0; i < 3; ++i) {
        QRect tr(r.left() + i * tabW, r.top(), tabW, tabHeight);
        p.setPen(QColor(20, 20, 30));
        p.setBrush(i == 0 ? QColor(80, 80, 120) : QColor(60, 60, 90));
        p.drawRect(tr.adjusted(0, 0, -1, -1));
        p.setPen(QColor(230, 230, 230));
        p.drawText(tr, Qt::AlignCenter,
                   QStringLiteral("TAB %1").arg(i + 1));
    }

    // Content-Bereich
    QRect content = r.adjusted(0, tabHeight, 0, 0);
    p.setPen(QColor(20, 20, 30));
    p.setBrush(QColor(15, 15, 25));
    p.drawRect(content.adjusted(0, 0, -1, -1));

    p.restore();

    // ⬇️ Später: Tabs aus ctrl->tabItems o.ä. benutzt + State.
}

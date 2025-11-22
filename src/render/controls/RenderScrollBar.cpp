#include "render/controls/RenderScrollBar.h"
#include "layout/ControlLayout.h"
#include "theme/ThemeManager.h"

void RenderScrollBar::render(QPainter& p,
                             const ControlRenderInfo& info,
                             ThemeManager* theme)
{
    const QRect& rect = info.renderRect;

    // einfach graues Scrollbar-Rechteck
    p.fillRect(rect, QColor(50,50,50));
    p.setPen(QColor(255,255,255,40));
    p.drawRect(rect.adjusted(0,0,-1,-1));
}

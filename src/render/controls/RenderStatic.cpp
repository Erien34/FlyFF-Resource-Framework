#include "render/controls/RenderStatic.h"
#include "layout/ControlLayout.h"
#include "layout/model/ControlData.h"
#include "theme/ThemeManager.h"

void RenderStatic::render(QPainter& p,
                          const ControlRenderInfo& info,
                          ThemeManager* /*theme*/)
{
    const QRect r = info.renderRect;
    auto ctrl = info.data;

    p.save();

    // optionaler Hintergrund
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::NoBrush);

    // ⬇️ Später: Text aus ctrl (z.B. caption) verwenden
    p.setPen(ctrl->disabled ? QColor(100, 100, 100) : QColor(220, 220, 220));
    p.drawText(r.adjusted(2, 0, -2, 0),
               Qt::AlignVCenter | Qt::AlignLeft,
               QStringLiteral("STATIC"));

    p.restore();
}

#include "render/controls/RenderCustom.h"
#include "layout/ControlLayout.h"
#include "layout/model/ControlData.h"
#include "theme/ThemeManager.h"

void RenderCustom::render(QPainter& p,
                          const ControlRenderInfo& info,
                          ThemeManager* theme)
{
    const QRect& rect = info.renderRect;

    // Falls Custom Texture
    auto ctrl = info.data;
    QString tex = ctrl->texture.trimmed().toLower();

    if (!tex.isEmpty())
    {
        QPixmap pm = theme->texture(tex, ControlState::Normal);
        if (!pm.isNull())
        {
            p.drawPixmap(rect,
                         pm.scaled(rect.size(),
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation));
            return;
        }
    }

    // Fallback: Gelb für Custom
    p.fillRect(rect, QColor(150,150,30,120));
    p.setPen(Qt::black);
    p.drawRect(rect.adjusted(0,0,-1,-1));
}

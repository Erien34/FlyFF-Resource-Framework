#include "render/controls/RenderComboBox.h"
#include "render/controls/EditBackground.h"
#include "layout/ControlLayout.h"
#include "layout/model/ControlData.h"
#include "theme/ThemeManager.h"

void RenderComboBox::render(QPainter& p,
                            const ControlRenderInfo& info,
                            ThemeManager* theme)
{
    const QRect& rect = info.renderRect;
    auto ctrl = info.data;

    QString tex = ctrl->texture.trimmed().toLower();

    // 1) Combo mit eigener Textur
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

    // 2) Minimaler Fallback
    p.fillRect(rect, QColor(40,40,40,100));
    p.setPen(QColor(255,255,255,40));
    p.drawRect(rect.adjusted(0,0,-1,-1));

    // 3) minimaler Pfeil (Editor-Symbol)
    QPoint center(rect.right() - 10, rect.center().y());
    p.drawLine(center + QPoint(-3, -2), center + QPoint(0, 2));
    p.drawLine(center + QPoint(3, -2), center + QPoint(0, 2));
}

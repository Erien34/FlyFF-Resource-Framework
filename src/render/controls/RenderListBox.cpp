#include "render/controls/RenderListBox.h"
#include "render/controls/EditBackground.h"
#include "layout/ControlLayout.h"
#include "layout/model/ControlData.h"
#include "theme/ThemeManager.h"

void RenderListBox::render(QPainter& p,
                           const ControlRenderInfo& info,
                           ThemeManager* theme)
{
    const QRect& rect = info.renderRect;
    auto ctrl = info.data;

    QString tex = ctrl->texture.trimmed().toLower();

    if (!tex.isEmpty())
    {
        QPixmap pm = theme->texture(tex, ControlState::Normal);
        if (!pm.isNull())
        {
            p.drawPixmap(rect, pm.scaled(rect.size(),
                                         Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));
            return;
        }
    }

    p.fillRect(rect, QColor(30,30,30,100));
    p.setPen(QColor(255,255,255,40));
    p.drawRect(rect.adjusted(0,0,-1,-1));
}

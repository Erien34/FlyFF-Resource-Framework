#include "render/controls/RenderGroupBox.h"
#include "layout/ControlLayout.h"
#include "layout/model/ControlData.h"
#include "theme/ThemeManager.h"

void RenderGroupBox::render(QPainter& p,
                            const ControlRenderInfo& info,
                            ThemeManager* theme)
{
    const QRect& rect = info.renderRect;

    if (!info.data)
    {
        p.fillRect(rect, QColor(180, 50, 50, 120)); // debug fallback
        return;
    }

    auto ctrl = info.data;

    // 1) Textur aus Control lesen
    QString texName = ctrl->texture.trimmed().toLower();

    // 2) Falls Textur gesetzt → ThemeManager nutzen
    if (!texName.isEmpty())
    {
        QPixmap pm = theme->texture(texName, ControlState::Normal);

        if (!pm.isNull())
        {
            p.drawPixmap(rect,
                         pm.scaled(rect.size(),
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation));
            return;
        }
    }

    // 3) Fallback: dezente GroupBox wie im FlyFF Editor
    p.save();
    p.setOpacity(1.0);

    p.fillRect(rect, QColor(30, 30, 30, 80));      // Hintergrund
    p.setPen(QColor(255, 255, 255, 40));           // heller Rahmen
    p.drawRect(rect.adjusted(0, 0, -1, -1));

    p.restore();
}

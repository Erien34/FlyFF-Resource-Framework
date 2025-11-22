#include "RenderButton.h"
#include "ControlLayout.h"
#include "ProcessedThemeColors.h"
#include "ThemeManager.h"
#include <QPixmap>
#include <QImage>

void RenderButton::render(QPainter& p,
                          const ControlRenderInfo& info,
                          ThemeManager* theme)
{
    const QRect rect = info.renderRect;

    QString texName = info.data->texture.toLower().trimmed();
    if (texName.isEmpty())
    {
        p.fillRect(rect, Qt::gray);
        return;
    }

    QPixmap pm = theme->texture(texName, info.state);
    if (pm.isNull())
    {
        p.fillRect(rect, Qt::red);
        return;
    }

    // ✔️ CLIENT-RENDER: Full stretch des Buttons
    p.drawPixmap(rect, pm.scaled(rect.size(),
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation));
}

// -------------------------------------
// 6) Text rendern
// -------------------------------------
// QString text = ctrl->id.trimmed();
// if (!text.isEmpty())
// {
//     QFont f = p.font();
//     f.setPixelSize(12);
//     p.setFont(f);

//     QFontMetrics fm(f);

//     int tx = rc.left() + (rc.width()  - fm.horizontalAdvance(text)) / 2;
//     int ty = rc.top()  + (rc.height() - fm.height()) / 2 + fm.ascent();

//     QColor textColor = gProcessedColors.get("Text_Button", QColor());
//     p.setPen(textColor);

//     p.drawText(tx, ty, text);
// }

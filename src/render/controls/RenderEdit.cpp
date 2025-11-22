#include "RenderEdit.h"
#include "ControlLayout.h"
#include "EditBackground.h"
#include <QPainter>
#include <QFontMetrics>
#include "ProcessedThemeColors.h"
extern ProcessedThemeColors gProcessedColors;

void RenderEdit::render(QPainter& p,
                        const ControlRenderInfo& info,
                        ThemeManager* theme)
{
    if (!theme)
        return;

    const QRect rc = info.renderRect;

    RenderHelpers::drawEditBackground(p, rc, theme, info);

    QColor textColor = gProcessedColors.get("Edit.Text", QColor(230,230,230));
    p.setPen(textColor);
    p.save();

    QFont font = p.font();
    font.setPixelSize(12);
    p.setFont(font);

    QFontMetrics fm(font);

    QString text = QStringLiteral("EDIT");

    const int pad = 4;
    QRect inner = rc.adjusted(pad, pad, -pad, -pad);

    int textY = inner.top() + (inner.height() - fm.height()) / 2 + fm.ascent();

    p.drawText(inner.left(), textY, text);

    p.restore();
}


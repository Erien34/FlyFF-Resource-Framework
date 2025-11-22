#pragma once

#include <QPainter>
#include <QRect>

class ThemeManager;
struct ControlRenderInfo;

namespace RenderHelpers
{
// Zeichnet den typischen Edit-Hintergrund (rahmen, inneres Feld)
void drawEditBackground(QPainter& p,
                        const QRect& rect,
                        ThemeManager* theme,
                        const ControlRenderInfo& info);
}

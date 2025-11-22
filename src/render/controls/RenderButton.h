#pragma once
#include "IControlRender.h"

class RenderButton : public IControlRender
{
public:
    void render(QPainter& p,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
    void renderButton(QPainter& p, const QRect& rc, const QPixmap& pm);
};

#pragma once

#include "IControlRender.h"

class RenderScrollBar : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

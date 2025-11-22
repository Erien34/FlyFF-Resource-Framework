#pragma once

#include "IControlRender.h"

class RenderStatic : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

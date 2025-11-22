#pragma once

#include "IControlRender.h"

class RenderTabControl : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

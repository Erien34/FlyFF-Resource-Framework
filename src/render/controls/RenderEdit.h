#pragma once

#include "IControlRender.h"

class RenderEdit : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

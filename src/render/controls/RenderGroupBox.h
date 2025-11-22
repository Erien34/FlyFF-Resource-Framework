#pragma once

#include "IControlRender.h"

class RenderGroupBox : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

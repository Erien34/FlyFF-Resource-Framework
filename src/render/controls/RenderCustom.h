#pragma once

#include "IControlRender.h"

class RenderCustom : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

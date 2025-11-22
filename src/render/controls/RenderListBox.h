#pragma once

#include "IControlRender.h"

class RenderListBox : public IControlRender
{
public:
    void render(QPainter& painter,
                const ControlRenderInfo& info,
                ThemeManager* theme) override;
};

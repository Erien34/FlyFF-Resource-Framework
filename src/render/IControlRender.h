#pragma once
#include <QPainter>
class ThemeManager;

struct ControlRenderInfo;

class IControlRender{
    public:virtual ~IControlRender()=default;

    virtual void render(QPainter&,const ControlRenderInfo&,ThemeManager*)=0;
};

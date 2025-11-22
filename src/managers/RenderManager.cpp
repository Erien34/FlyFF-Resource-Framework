#include "RenderManager.h"
#include "ThemeManager.h"
#include "BehaviorManager.h"
#include "WindowData.h"
#include "ControlData.h"
#include "LayoutEngine.h"

#include <QPainter>

RenderManager::RenderManager(ThemeManager* theme,
                             BehaviorManager* behavior)
    : m_themeManager(theme)
    , m_behaviorManager(behavior)
{
    m_windowRender  = std::make_unique<RenderWindow>(theme, behavior);
    m_controlRender = std::make_unique<RenderControls>(theme, behavior);
    m_layoutEngine  = std::make_unique<LayoutEngine>(theme, behavior);
}

void RenderManager::render(QPainter* painter,
                           const std::shared_ptr<WindowData>& wnd,
                           const QSize& canvasSize)
{
    if (!painter || !wnd)
        return;

    // Hintergrund
    painter->save();
    QRect canvasRect(0, 0, canvasSize.width(), canvasSize.height());
    painter->fillRect(canvasRect, QColor(45, 45, 45));
    painter->setPen(Qt::gray);
    painter->drawRect(canvasRect.adjusted(0, 0, -1, -1));
    painter->restore();

    // LayoutEngine berechnet: Position + Größe + Controls
    WindowRenderInfo layoutInfo =
        m_layoutEngine->computeWindowLayout(wnd, canvasSize);

    // Fenster rendern
    if (m_windowRender)
        m_windowRender->render(*painter, layoutInfo);

    // Controls rendern
    if (m_controlRender)
        m_controlRender->render(*painter, layoutInfo.controls);
}

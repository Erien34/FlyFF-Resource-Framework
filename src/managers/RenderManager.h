#pragma once

#include <memory>
#include <QPainter>

#include "RenderWindow.h"
#include "RenderControls.h"
#include "LayoutEngine.h"

class ThemeManager;
class BehaviorManager;
struct WindowData;

/**
 * RenderManager
 * -------------------------------------
 * Verantwortlich für:
 *  - LayoutEngine-Lauf
 *  - RenderWindow aufrufen
 *  - RenderControls aufrufen
 */
class RenderManager
{
public:
    RenderManager(ThemeManager* theme, BehaviorManager* behavior);

    void render(QPainter* painter,
                const std::shared_ptr<WindowData>& window,
                const QSize& canvasSize);

private:
    ThemeManager*      m_themeManager;
    BehaviorManager*   m_behaviorManager;

    std::unique_ptr<RenderWindow>   m_windowRender;
    std::unique_ptr<RenderControls> m_controlRender;
    std::unique_ptr<LayoutEngine>   m_layoutEngine;
};

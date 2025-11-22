// behavior/BehaviorEngine.cpp
#include "behavior/BehaviorEngine.h"
#include "BehaviorManager.h"
#include "layout/LayoutEngine.h"
#include "layout/model/WindowData.h"
#include <QDebug>

BehaviorEngine::BehaviorEngine(BehaviorManager* behaviorMgr,
                               LayoutEngine* layoutEngine,
                               QObject* parent)
    : QObject(parent)
    , m_behaviorManager(behaviorMgr)
    , m_layoutEngine(layoutEngine)
{
}

void BehaviorEngine::setActiveWindow(const std::shared_ptr<WindowData>& wnd)
{
    m_activeWindow = wnd;
}

void BehaviorEngine::mousePress(const QPoint& pos,
                                Qt::MouseButton button,
                                Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);
    m_lastPos = pos;

    // später: HitTest, Auswahl, Drag-Start etc.
    qDebug() << "[BehaviorEngine] mousePress@" << pos << "button" << button;
}

void BehaviorEngine::mouseMove(const QPoint& pos,
                               Qt::MouseButtons buttons,
                               Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);
    if (buttons & Qt::LeftButton) {
        // später: Dragging etc.
    }

    m_lastPos = pos;
}

void BehaviorEngine::mouseRelease(const QPoint& pos,
                                  Qt::MouseButton button,
                                  Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);
    qDebug() << "[BehaviorEngine] mouseRelease@" << pos << "button" << button;
    m_isDragging = false;
}

void BehaviorEngine::mouseLeave()
{
    // später: Hover-States zurücksetzen
    qDebug() << "[BehaviorEngine] mouseLeave";
}

void BehaviorEngine::hitTest(const QPoint& pos)
{
    Q_UNUSED(pos);
    // später: LayoutEngine → WindowRenderInfo cashen und dort Controls hit-testen
}

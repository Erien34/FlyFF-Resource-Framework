// Canvas.cpp
#include "Canvas.h"
#include "RenderManager.h"
#include "ProjectController.h"
#include "behavior/BehaviorEngine.h"
#include "WindowData.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Canvas::Canvas(ProjectController* controller, QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
    , m_renderManager(controller ? controller->renderManager() : nullptr)
    , m_behaviorEngine(controller ? controller->behaviorEngine() : nullptr) // NEU
{
    setMinimumSize(800, 600);
    setMouseTracking(true);

    qInfo() << "[Canvas] Initialized with RenderManager:"
            << (m_renderManager ? "OK" : "null")
            << "BehaviorEngine:"
            << (m_behaviorEngine ? "OK" : "null");
}

void Canvas::setEngines(RenderManager* rm, BehaviorEngine* be)
{
    m_renderManager = rm;
    m_behaviorEngine = be;
}

void Canvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    if (m_renderManager && m_activeWindow) {
        m_renderManager->render(&painter, m_activeWindow, size());
    } else {
        painter.fillRect(rect(), QColor(45, 45, 45));
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter,
                         "Kein aktives Fenster oder RenderManager");
    }
}

void Canvas::setActiveWindow(const std::shared_ptr<WindowData>& wnd)
{
    m_activeWindow = wnd;
    update();
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    if (m_behaviorEngine)
        m_behaviorEngine->mousePress(event->pos(), event->button(), event->modifiers());
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (m_behaviorEngine)
        m_behaviorEngine->mouseMove(event->pos(), event->buttons(), event->modifiers());
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_behaviorEngine)
        m_behaviorEngine->mouseRelease(event->pos(), event->button(), event->modifiers());
}

void Canvas::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    if (m_behaviorEngine)
        m_behaviorEngine->mouseLeave();
}

// Canvas.h
#pragma once
#include "WindowData.h"
#include <QWidget>

class ProjectController;
class RenderManager;
class BehaviorEngine;   // NEU

class Canvas : public QWidget {
    Q_OBJECT
public:
    explicit Canvas(ProjectController* controller, QWidget* parent = nullptr);
    ~Canvas() override = default;

    void setActiveWindow(const std::shared_ptr<WindowData>& wnd);
    void setEngines(RenderManager*, BehaviorEngine*);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    ProjectController* m_controller = nullptr;
    RenderManager*     m_renderManager = nullptr;
    BehaviorEngine*    m_behaviorEngine = nullptr; // nur Zeiger, Ownership beim Controller

    std::shared_ptr<WindowData> m_activeWindow;
};

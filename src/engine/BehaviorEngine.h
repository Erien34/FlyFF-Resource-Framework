// behavior/BehaviorEngine.h
#pragma once
#include <QObject>
#include <QPoint>
#include <QFlags>
#include <memory>

class BehaviorManager;
class LayoutEngine;
struct WindowData;

class BehaviorEngine : public QObject {
    Q_OBJECT
public:
    explicit BehaviorEngine(BehaviorManager* behaviorMgr,
                            LayoutEngine* layoutEngine,
                            QObject* parent = nullptr);

    void setActiveWindow(const std::shared_ptr<WindowData>& wnd);

    void mousePress(const QPoint& pos, Qt::MouseButton button, Qt::KeyboardModifiers mods);
    void mouseMove(const QPoint& pos, Qt::MouseButtons buttons, Qt::KeyboardModifiers mods);
    void mouseRelease(const QPoint& pos, Qt::MouseButton button, Qt::KeyboardModifiers mods);
    void mouseLeave();

signals:
    void selectionChanged();    // später für UI

private:
    BehaviorManager* m_behaviorManager = nullptr;
    LayoutEngine*    m_layoutEngine    = nullptr;

    std::weak_ptr<WindowData> m_activeWindow;

    QPoint m_lastPos;
    bool   m_isDragging = false;

    void hitTest(const QPoint& pos);
};

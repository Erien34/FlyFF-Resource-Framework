#pragma once

#include <QMainWindow>
#include <QJsonObject>


class ProjectController;
class LayoutManager;

class Canvas;

class PropertyPanel;
class WindowPanel;

struct WindowData;


/**
 * @brief Hauptfenster des FlyFF GUI Editors.
 *        Stellt die UI-Struktur bereit und verbindet sich mit dem ProjectController.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ProjectController* controller, QWidget* parent = nullptr);
    ~MainWindow() override = default;
    void initializeAfterLoad();

private:
    void createToolBar();
    void createDocks();
    void createStatusBar();

private:
    ProjectController* m_controller = nullptr;
    WindowPanel* m_windowPanel = nullptr;
    PropertyPanel* m_propertyPanel = nullptr;
    Canvas* m_canvas;

};

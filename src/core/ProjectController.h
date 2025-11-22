#pragma once

#include <QObject>
#include <memory>
#include <QMap>
#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QDebug>

#include "WindowData.h"
#include "ConfigManager.h"
#include "FileManager.h"
#include "LayoutParser.h"
#include "LayoutBackend.h"
#include "DefineManager.h"
#include "DefineBackend.h"
#include "FlagManager.h"
#include "TextManager.h"
#include "TextBackend.h"
#include "LayoutManager.h"
#include "RenderManager.h"
#include "ThemeManager.h"
#include "BehaviorManager.h"
#include "BehaviorEngine.h"
#include "LayoutEngine.h"

class Canvas;       // NEU: statt CanvasHandler
class WindowPanel;
class PropertyPanel;

class ProjectController : public QObject
{
    Q_OBJECT

public:
    explicit ProjectController(QObject* parent = nullptr);

    // ---- Canvas Binding (neu) ----
    void bindCanvas(Canvas* canvas);

    // Panels wie bisher
    void bindPanels(WindowPanel* wp, PropertyPanel* pp);

    bool loadProject(const QString& configPath);
    bool saveProject();

    LayoutManager* layoutManager() const { return m_layoutManager.get(); }
    TextManager* textManager() const { return m_textManager.get(); }
    BehaviorManager* behaviorManager() const { return m_behaviorManager.get(); }
    ThemeManager* themeManager() const { return m_themeManager.get(); }
    RenderManager* renderManager() const { return m_renderManager.get(); }
    BehaviorEngine* behaviorEngine() const { return m_behaviorEngine.get(); }
    LayoutEngine* layoutEngine() const { return m_layoutEngine.get(); }

    // Aktive Auswahl
    std::shared_ptr<WindowData>  currentWindow() const { return m_currentWindow; }
    std::shared_ptr<ControlData> currentControl() const { return m_currentControl; }

    void toggleControlFlag(const QString& flag, bool enable);
    void toggleWindowFlag(const QString& flag, bool enable);
    void updateWindowFlags(const QString& windowName, quint32 mask, bool enabled);
    void updateControlFlags(const QString& controlId, quint32 mask, bool enabled);

    void requestUiRefreshAsync();

signals:
    void projectLoaded();
    void projectSaved();
    void layoutsReady();
    void activeWindowChanged(const std::shared_ptr<WindowData>& wnd);
    void windowsReady(const std::vector<std::shared_ptr<WindowData>>& windows);

    void selectionChanged();
    void uiRefreshRequested();

public slots:
    void selectWindow(const QString& windowName);
    void selectControl(const QString& windowName, const QString& controlName);

private slots:
    void onTokensReady();

private:
    // !!! Alle deine bestehenden Manager bleiben !!!
    std::unique_ptr<ConfigManager>   m_configManager;
    std::unique_ptr<FileManager>     m_fileManager;
    std::unique_ptr<LayoutParser>    m_layoutParser;
    std::unique_ptr<LayoutBackend>   m_layoutBackend;
    std::unique_ptr<DefineManager>   m_defineManager;
    std::unique_ptr<DefineBackend>   m_defineBackend;
    std::unique_ptr<FlagManager>     m_flagManager;
    std::unique_ptr<TextManager>     m_textManager;
    std::unique_ptr<TextBackend>     m_textBackend;
    std::unique_ptr<LayoutManager>   m_layoutManager;
    std::unique_ptr<ThemeManager>    m_themeManager;
    std::unique_ptr<BehaviorManager> m_behaviorManager;

    // ---- NEU ----
    std::unique_ptr<LayoutEngine>    m_layoutEngine;
    std::unique_ptr<BehaviorEngine>  m_behaviorEngine;

    // RenderManager jetzt mit LayoutEngine
    std::unique_ptr<RenderManager>   m_renderManager;

    QMap<QString, QIcon>   m_icons;
    QMap<QString, QPixmap> m_themes;

    std::shared_ptr<WindowData>  m_currentWindow;
    std::shared_ptr<ControlData> m_currentControl;

    std::shared_ptr<WindowData> findWindow(const QString& name) const;
    std::shared_ptr<ControlData> findControl(const QString& id) const;

    bool m_loadingActive = false;
    bool m_tokensReady = false;
};

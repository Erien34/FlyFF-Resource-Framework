#pragma once
#include "IControlRender.h"

#include <QObject>
#include <memory>
#include <vector>
#include <map>

#include <QPainter>

class ThemeManager;
class BehaviorManager;

struct ControlRenderInfo;
struct ControlData;
class IControlRender;

// Zentraler Dispatcher, wird vom RenderManager benutzt
class RenderControls : public QObject
{
    Q_OBJECT
public:
    RenderControls(ThemeManager* theme,
                   BehaviorManager* behavior,
                   QObject* parent = nullptr);

    void render(QPainter& painter,
                const std::vector<ControlRenderInfo>& controls);

private:
    void renderSingle(QPainter& painter,
                      const ControlRenderInfo& info);

private:
    ThemeManager*    m_themeManager = nullptr;
    BehaviorManager* m_behaviorManager = nullptr;

    // Später evtl. enum-based Dispatch. Aktuell halten wir Pointer nur,
    // falls du manuell dispatchen möchtest.
    std::unique_ptr<IControlRender> m_buttonRender;
    std::unique_ptr<IControlRender> m_staticRender;
    std::unique_ptr<IControlRender> m_editRender;
    std::unique_ptr<IControlRender> m_groupBoxRender;
    std::unique_ptr<IControlRender> m_comboBoxRender;
    std::unique_ptr<IControlRender> m_listBoxRender;
    std::unique_ptr<IControlRender> m_tabControlRender;
    std::unique_ptr<IControlRender> m_scrollBarRender;
    std::unique_ptr<IControlRender> m_customRender;
};

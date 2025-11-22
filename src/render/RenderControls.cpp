#include "render/RenderControls.h"

#include "IControlRender.h"
#include "layout/ControlLayout.h"
#include "theme/ThemeManager.h"
#include "BehaviorManager.h"

// Einzelne Renderer
#include "render/controls/RenderButton.h"
#include "render/controls/RenderStatic.h"
#include "render/controls/RenderEdit.h"
#include "render/controls/RenderGroupBox.h"
#include "render/controls/RenderComboBox.h"
#include "render/controls/RenderListBox.h"
#include "render/controls/RenderTabControl.h"
#include "render/controls/RenderScrollBar.h"
#include "render/controls/RenderCustom.h"

RenderControls::RenderControls(ThemeManager* theme,
                               BehaviorManager* behavior,
                               QObject* parent)
    : QObject(parent)
    , m_themeManager(theme)
    , m_behaviorManager(behavior)
{
    m_buttonRender     = std::make_unique<RenderButton>();
    m_staticRender     = std::make_unique<RenderStatic>();
    m_editRender       = std::make_unique<RenderEdit>();
    m_groupBoxRender   = std::make_unique<RenderGroupBox>();
    m_comboBoxRender   = std::make_unique<RenderComboBox>();
    m_listBoxRender    = std::make_unique<RenderListBox>();
    m_tabControlRender = std::make_unique<RenderTabControl>();
    m_scrollBarRender  = std::make_unique<RenderScrollBar>();
    m_customRender     = std::make_unique<RenderCustom>();
}

void RenderControls::render(QPainter& painter,
                            const std::vector<ControlRenderInfo>& controls)
{
    for (const auto& info : controls)
        renderSingle(painter, info);
}

void RenderControls::renderSingle(QPainter& p, const ControlRenderInfo& info)
{
    const ControlType type = info.data->mappedType;

    switch (type)
    {
    case ControlType::Edit:
        m_editRender->render(p, info, m_themeManager);
        break;

    case ControlType::Button:
        m_buttonRender->render(p, info, m_themeManager);
        break;

    case ControlType::Static:
        m_staticRender->render(p, info, m_themeManager);
        break;

    case ControlType::GroupBox:
        m_groupBoxRender->render(p, info, m_themeManager);
        break;

    case ControlType::ComboBox:
        m_comboBoxRender->render(p, info, m_themeManager);
        break;

    case ControlType::ListBox:
        m_listBoxRender->render(p, info, m_themeManager);
        break;

    case ControlType::TabControl:
        m_tabControlRender->render(p, info, m_themeManager);
        break;

    case ControlType::ScrollBarH:
        m_scrollBarRender->render(p, info, m_themeManager);
        break;

    case ControlType::ScrollBarV:
        m_scrollBarRender->render(p, info, m_themeManager);
        break;

    case ControlType::CheckBox:
        m_comboBoxRender->render(p, info, m_themeManager);
        break;

    case ControlType::Custom:
        m_customRender->render(p, info, m_themeManager);
        break;

    default:
        // Fallback → rotes Rechteck
        p.save();
        p.setBrush(QColor(255, 0, 0, 120));
        p.drawRect(info.renderRect);
        p.restore();
        break;
    }
}

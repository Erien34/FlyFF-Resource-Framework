#include "ControlTypeMapper.h"
#include <QString>

bool ControlTypeMapper::contains(const QString& raw, std::initializer_list<const char*> list)
{
    for (auto* entry : list)
    {
        if (raw.compare(entry, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

ControlType ControlTypeMapper::map(const QString& rawtype)
{
    QString t = rawtype.trimmed().toUpper();

    // ------- BUTTON ----------
    if (contains(t, {
        "WTYPE_BUTTON",
        "WTYPE_BUTTON1",
        "WTYPE_BUTTON2",
        "WTYPE_OKBUTTON",
        "WTYPE_CHECKBUTTON",
        "WTYPE_RADIOBUTTON",
        "WTYPE_HYPERBUTTON",
        "WTYPE_TABBUTTONCTRL",
        "WTYPE_RESISTANCEBUTTON"
        })) return ControlType::Button;

    // ------- EDIT ----------
    if (contains(t, {
        "WTYPE_EDITCTRL",
        "WTYPE_EDIT",
        "WTYPE_EDITBOX"
        })) return ControlType::Edit;

    // ------- STATIC ----------
    if (contains(t, {
        "WTYPE_STATIC",
        "WTYPE_TEXT",
        "WTYPE_CAPTION"
        })) return ControlType::Static;

    // ------- GROUPBOX ----------
    if (contains(t, {
        "WTYPE_GROUPBOX"
        })) return ControlType::GroupBox;

    // ------- COMBO BOX ----------
    if (contains(t, {
        "WTYPE_COMBOBOX"
        })) return ControlType::ComboBox;

    // ------- LISTBOX ----------
    if (contains(t, {
        "WTYPE_LISTBOX",
        "WTYPE_LISTCTRL"
        })) return ControlType::ListBox;

    // ------- TAB CONTROL ----------
    if (contains(t, {
        "WTYPE_TABCTRL"
        })) return ControlType::TabControl;

    // ------- SCROLLBAR ----------
    if (contains(t, {
        "WTYPE_SCROLLBAR",
        "WTYPE_SCROLLBAR2"
        })) return ControlType::ScrollBarH;

    // ------- CHECKBOX ----------
    if (contains(t, {
        "WTYPE_CHECKBOX"
        })) return ControlType::CheckBox;

    // ------- CUSTOM ----------
    if (t.startsWith("WTYPE_CUSTOM"))
        return ControlType::Custom;

    // ------- UNKNOWN ----------
    return ControlType::Unknown;
}

#pragma once
#include <QString>

enum class ControlType
{
    Button,
    Edit,
    Static,
    GroupBox,
    ComboBox,
    ListBox,
    TabControl,
    ScrollBarH,
    ScrollBarV,
    CheckBox,
    RadioButton,
    ScrollBarThumbV,
    ScrollBarThumbH,
    Progress,
    Custom,
    Unknown
};

inline QString controlTypeToString(ControlType t)
{
    switch (t)
    {
    case ControlType::Button:     return "Button";
    case ControlType::Edit:       return "Edit";
    case ControlType::Static:     return "Static";
    case ControlType::GroupBox:   return "GroupBox";
    case ControlType::ComboBox:   return "ComboBox";
    case ControlType::ListBox:    return "ListBox";
    case ControlType::TabControl: return "TabControl";
    case ControlType::ScrollBarV:  return "ScrollBarV";
    case ControlType::ScrollBarH:  return "ScrollBarH";
    case ControlType::CheckBox:   return "CheckBox";
    case ControlType::RadioButton:   return "RadioButton";
    case ControlType::ScrollBarThumbV:     return "ScrollBarThumbV";
    case ControlType::Progress:     return "Progress";
    case ControlType::Custom:     return "Custom";
    default:                      return "Unknown";
    }
}

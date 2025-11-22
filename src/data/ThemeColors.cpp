#include "ThemeColors.h"

namespace ThemeColors
{
namespace UI
{
const QString WindowBackground = QStringLiteral("ui.window.background");
const QString WindowBorderLight = QStringLiteral("ui.window.border.light");
const QString WindowBorderDark  = QStringLiteral("ui.window.border.dark");

const QString GroupBoxBorder = QStringLiteral("ui.groupbox.border");
const QString GroupBoxFill   = QStringLiteral("ui.groupbox.fill");

const QString ScrollTrack  = QStringLiteral("ui.scroll.track");
const QString ScrollBorder = QStringLiteral("ui.scroll.border");

const QString TabActiveText   = QStringLiteral("ui.tab.active.text");
const QString TabInactiveText = QStringLiteral("ui.tab.inactive.text");
}

namespace Text
{
const QString Normal          = QStringLiteral("text.normal");
const QString Disabled        = QStringLiteral("text.disabled");
const QString Highlight       = QStringLiteral("text.highlight");
const QString EditNormal      = QStringLiteral("text.edit.normal");
const QString EditPlaceholder = QStringLiteral("text.edit.placeholder");
const QString Static          = QStringLiteral("text.static");
const QString WindowTitle     = QStringLiteral("text.window.title");
}

namespace Ctrl
{
const QString ButtonNormal  = QStringLiteral("ctrl.button.normal");
const QString ButtonHover   = QStringLiteral("ctrl.button.hover");
const QString ButtonPressed = QStringLiteral("ctrl.button.pressed");

const QString CheckBorder = QStringLiteral("ctrl.check.border");
const QString CheckFill   = QStringLiteral("ctrl.check.fill");

const QString ComboArrow = QStringLiteral("ctrl.combo.arrow");

const QString ListBackground = QStringLiteral("ctrl.list.background");
const QString ListBorder     = QStringLiteral("ctrl.list.border");
}
}

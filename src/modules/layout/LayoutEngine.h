// layout/LayoutEngine.h
#pragma once

#include <memory>
#include <vector>
#include <QSize>
#include <QRect>

#include "layout/model/WindowData.h"
#include "layout/ControlLayout.h"

class ThemeManager;
class BehaviorManager;

/// Verantwortlich für:
/// - Fenster-Rect im Canvas (zentrieren oder Position aus Daten)
/// - Content-Bereich anhand des Window-Tilesets
/// - Normalisierung der Control-Koordinaten (FlyFF → Editor)
/// - Berechnung der finalen Render-Rechtecke (Controls)
class LayoutEngine
{
public:
    LayoutEngine(ThemeManager* themeMgr,
                 BehaviorManager* behaviorMgr);

    void setCurrentWindowName(const QString& name) { m_currentWindow = name; }

    /// Hauptfunktion: Berechnet alle Layoutdaten für ein Fenster.
    WindowRenderInfo computeWindowLayout(
        const std::shared_ptr<WindowData>& wnd,
        const QSize& canvasSize) const;

private:
    ThemeManager*    m_themeMgr    = nullptr;
    BehaviorManager* m_behaviorMgr = nullptr;

    QString m_currentWindow;

    /// Zentriert das Fenster im Canvas.
    QRect computeWindowRectCentered(const WindowData& wnd,
                                    const QSize& canvasSize) const;

    /// Bestimmt den inneren Content-Bereich anhand des Tilesets.
    QRect computeContentRectFromTiles(const QRect& windowRect) const;

    /// Normalisiert die Controls in den Content-Bereich und erzeugt RenderInfos.
    std::vector<ControlRenderInfo> computeControlsLayout(
        const WindowData& wnd,
        const QRect& contentRect) const;
};

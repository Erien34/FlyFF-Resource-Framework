// RenderWindow.h
#pragma once
#include <QObject>
#include <QPainter>
#include <memory>

#include "theme/ThemeManager.h"
#include "BehaviorManager.h"
#include "layout/ControlLayout.h" // WindowRenderInfo

class RenderWindow : public QObject
{
    Q_OBJECT

public:
    explicit RenderWindow(ThemeManager* themeManager,
                          BehaviorManager* behaviorManager,
                          QObject* parent = nullptr);

    void render(QPainter& p,
                const WindowRenderInfo& info);

private:
    bool drawDirectWindowTexture(QPainter& p,
                                 const WindowRenderInfo& info);

    bool drawWindowTileset(QPainter& p,
                           const WindowRenderInfo& info);

    void drawFallbackWindow(QPainter& p,
                            const WindowRenderInfo& info);

    void drawTitleAndButtons(QPainter& p,
                             const WindowRenderInfo& info);

    void drawCloseButton(QPainter& p, const QRect& rect);
    void drawHelpButton(QPainter& p, const QRect& rect);
    void drawFallbackButton(QPainter& p,
                            const QRect& rect,
                            const QColor& color,
                            const QString& text);

private:
    ThemeManager*    m_themeManager   = nullptr;
    BehaviorManager* m_behaviorManager = nullptr;
};

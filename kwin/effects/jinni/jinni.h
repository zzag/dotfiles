#ifndef KWIN_JINNI_H
#define KWIN_JINNI_H

#include <kwineffects.h>

#include "timeline.h"

using KWin::Effect;
using KWin::EffectWindow;
using KWin::ScreenPrePaintData;
using KWin::WindowPrePaintData;
using KWin::WindowPaintData;


class JinniEffect : public Effect {
    Q_OBJECT

public:
    JinniEffect();

    virtual void reconfigure(ReconfigureFlags);
    virtual void prePaintScreen(ScreenPrePaintData& data, int time);
    virtual void prePaintWindow(EffectWindow* w, WindowPrePaintData& data, int time);
    virtual void paintWindow(EffectWindow* w, int mask, QRegion region, WindowPaintData& data);
    virtual void postPaintScreen();
    virtual bool isActive() const;

    int requestedEffectChainPosition() const override {
        return 50;
    }

    static bool supported();

private Q_SLOTS:
    void windowDeleted(KWin::EffectWindow *w);
    void windowMinimized(KWin::EffectWindow *w);
    void windowUnminimized(KWin::EffectWindow *w);

private:
    float nonlinearity(float x);

    void minimizeBottom(
      float progress, QRect geometry,
      QRect icon, WindowPaintData& data);

    void minimizeTop(
      float progress, QRect geometry,
      QRect icon, WindowPaintData& data);

    void minimizeLeft(
      float progress, QRect geometry,
      QRect icon, WindowPaintData& data);

    void minimizeRight(
      float progress, QRect geometry,
      QRect icon, WindowPaintData& data);

private:
    int mDuration;
    float mShapeTransformTime;
    int mNonlinearityScale;
    int mGridResolution;

    QHash<EffectWindow*, Timeline*> mTimeLineWindows;
};

#endif

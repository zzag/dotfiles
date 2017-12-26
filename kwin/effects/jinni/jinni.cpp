#include "jinni.h"

#include <kwinconfig.h>
#include <math.h>

using KWin::EffectsHandler;
using KWin::ScreenArea;
using KWin::WindowQuad;
using KWin::WindowQuadList;
using KWin::effects;


enum class IconPosition {
  Unknown,
  Bottom,
  Top,
  Left,
  Right
};

IconPosition
getIconPosition(const QRect icon) {
  if (! icon.isValid()) {
    return IconPosition::Unknown;
  }

  EffectWindow* panel = nullptr;
  foreach (EffectWindow* w, effects->stackingOrder()) {
    if (! w->isDock()) {
      continue;
    }
    if (w->geometry().intersects(icon)) {
      panel = w;
      break;
    }
  }

  if (panel) {
    QRect panelScreen = effects->clientArea(ScreenArea, panel);
    if (panel->width() >= panel->height()) {
      // horizontal panel
      return (panel->y() == panelScreen.y())
        ? IconPosition::Top
        : IconPosition::Bottom;
    }
    // vertical panel
    return (panel->x() == panelScreen.x())
      ? IconPosition::Left
      : IconPosition::Right;
  }

  return IconPosition::Unknown;
}


KWIN_EFFECT_FACTORY_ENABLED(JinniEffectFactory,
                            JinniEffect,
                            "jinni.json",
                            return false;)


JinniEffect::JinniEffect()
      : mDuration(440),
        mShapeTransformTime(0.22),
        mNonlinearityScale(6),
        mGridResolution(100)
{
    connect(effects, &EffectsHandler::windowDeleted,
            this, &JinniEffect::windowDeleted);
    connect(effects, &EffectsHandler::windowMinimized,
            this, &JinniEffect::windowMinimized);
    connect(effects, &EffectsHandler::windowUnminimized,
            this, &JinniEffect::windowUnminimized);
    reconfigure(ReconfigureAll);
}


bool
JinniEffect::supported() {
    return effects->isOpenGLCompositing() && effects->animationsSupported();
}


void
JinniEffect::reconfigure(ReconfigureFlags) {
}


void
JinniEffect::prePaintScreen(ScreenPrePaintData& data, int time) {
    if (mTimeLineWindows.count() > 0) {
        QHash<EffectWindow*, Timeline*>::iterator entry = mTimeLineWindows.begin();
        while (entry != mTimeLineWindows.end()) {
            auto timeline = entry.value();
            timeline->update(time);
            ++entry;
        }
        data.mask |= PAINT_SCREEN_WITH_TRANSFORMED_WINDOWS;
    }

    effects->prePaintScreen(data, time);
}


void
JinniEffect::prePaintWindow(EffectWindow* w, WindowPrePaintData& data, int time) {
    if (mTimeLineWindows.contains(w)) {
      const auto position = getIconPosition(w->iconGeometry());
      if (position == IconPosition::Unknown) {
        effects->prePaintWindow(w, data, time);
        return;
      }

      int xSubdivisions = 0;
      int ySubdivisions = 0;
      switch (position) {
        case IconPosition::Top:
        case IconPosition::Bottom:
          xSubdivisions = 2;
          ySubdivisions = mGridResolution;
          break;
        case IconPosition::Left:
        case IconPosition::Right:
          xSubdivisions = mGridResolution;
          ySubdivisions = 2;
          break;
        default:
          break;
      }

      data.setTransformed();
      data.quads = data.quads.makeRegularGrid(xSubdivisions, ySubdivisions);
      w->enablePainting(EffectWindow::PAINT_DISABLED_BY_MINIMIZE);
    }

    effects->prePaintWindow(w, data, time);
}


void
JinniEffect::minimizeBottom(float progress, QRect geometry,
                            QRect icon, WindowPaintData& data) {
    const float distance = icon.y() + icon.height() - geometry.y();

    WindowQuadList newQuads;
    foreach (WindowQuad q, data.quads) {
        QRect quadGlobal = QRect(QPoint(q.left(), q.top()),
                                 QPoint(q.right(), q.bottom()));
        quadGlobal.translate(geometry.topLeft());

        // Calculate Y coords.

        float farthest = q[1].y(),
              closest = q[3].y();

        if (progress > mShapeTransformTime) {
            const float moveProgress =
                (progress - mShapeTransformTime) / (1 - mShapeTransformTime);
            farthest += moveProgress * distance;
            closest += moveProgress * distance;
        }

        // Calculate X coords.
        const float weightTop = 1 - (distance - farthest) / distance;
        float progressTop = nonlinearity(weightTop);
        const float weightBottom = 1 - (distance - closest) / distance;
        float progressBottom = nonlinearity(weightBottom);

        if (progress < mShapeTransformTime) {
            float shapeProgress = progress / mShapeTransformTime;
            progressTop *= shapeProgress;
            progressBottom *= shapeProgress;
        }

        q[0].setX(q[0].x() + progressTop * ((icon.left() - quadGlobal.left())
                  + icon.width() * q[0].x() / geometry.width()));
        q[3].setX(q[3].x() + progressBottom * ((icon.left() - quadGlobal.left())
                  + icon.width() * q[3].x() / geometry.width()));
        q[1].setX(q[1].x() + progressTop * ((icon.left() - quadGlobal.right())
                  + icon.width() * q[1].x() / geometry.width()));
        q[2].setX(q[2].x() + progressBottom * ((icon.left() - quadGlobal.right())
                  + icon.width() * q[2].x() / geometry.width()));

        q[0].setY(farthest); q[1].setY(farthest);
        q[2].setY(closest); q[3].setY(closest);

        newQuads.append(q);
    }
    data.quads = newQuads;
}


void
JinniEffect::minimizeTop(float progress, QRect geometry,
                         QRect icon, WindowPaintData& data) {
    const float distance = geometry.y() + geometry.height() - icon.y();

    WindowQuadList newQuads;
    foreach (WindowQuad q, data.quads) {
        QRect quadGlobal = QRect(QPoint(q.left(), q.top()),
                                 QPoint(q.right(), q.bottom()));
        quadGlobal.translate(geometry.topLeft());

        // Calculate Y coords.

        float farthest = q[3].y(),
              closest = q[1].y();

        if (progress > mShapeTransformTime) {
            const float moveProgress =
                (progress - mShapeTransformTime) / (1 - mShapeTransformTime);
            farthest -= moveProgress * distance;
            closest -= moveProgress * distance;
        }

        // Calculate X coords.
        const float weightTop = (distance - closest) / distance;
        float progressTop = nonlinearity(weightTop);
        const float weightBottom = (distance - farthest) / distance;
        float progressBottom = nonlinearity(weightBottom);

        if (progress < mShapeTransformTime) {
            float shapeProgress = progress / mShapeTransformTime;
            progressTop *= shapeProgress;
            progressBottom *= shapeProgress;
        }

        q[0].setX(q[0].x() + progressTop * ((icon.left() - quadGlobal.left())
                  + icon.width() * q[0].x() / geometry.width()));
        q[3].setX(q[3].x() + progressBottom * ((icon.left() - quadGlobal.left())
                  + icon.width() * q[3].x() / geometry.width()));
        q[1].setX(q[1].x() + progressTop * ((icon.left() - quadGlobal.right())
                  + icon.width() * q[1].x() / geometry.width()));
        q[2].setX(q[2].x() + progressBottom * ((icon.left() - quadGlobal.right())
                  + icon.width() * q[2].x() / geometry.width()));

        q[0].setY(closest); q[1].setY(closest);
        q[2].setY(farthest); q[3].setY(farthest);

        newQuads.append(q);
    }
    data.quads = newQuads;
}


void
JinniEffect::minimizeLeft(float progress, QRect geometry,
                          QRect icon, WindowPaintData& data) {
    const float distance = geometry.x() + geometry.width() - icon.x();

    WindowQuadList newQuads;
    foreach (WindowQuad q, data.quads) {
        QRect quadGlobal = QRect(QPoint(q.left(), q.top()),
                                 QPoint(q.right(), q.bottom()));
        quadGlobal.translate(geometry.topLeft());

        // Calculate Y coords.

        float farthest = q[1].x(),
              closest = q[0].x();

        if (progress > mShapeTransformTime) {
            const float moveProgress =
                (progress - mShapeTransformTime) / (1 - mShapeTransformTime);
            farthest -= moveProgress * distance;
            closest -= moveProgress * distance;
        }

        // Calculate Y coords.
        const float weightRight = (distance - farthest) / distance;
        float progressRight = nonlinearity(weightRight);
        const float weightLeft = (distance - closest) / distance;
        float progressLeft = nonlinearity(weightLeft);

        if (progress < mShapeTransformTime) {
            float shapeProgress = progress / mShapeTransformTime;
            progressLeft *= shapeProgress;
            progressRight *= shapeProgress;
        }

        q[0].setY(q[0].y() + progressLeft * ((icon.top() - quadGlobal.top())
                  + icon.height() * q[0].y() / geometry.height()));
        q[3].setY(q[3].y() + progressLeft * ((icon.top() - quadGlobal.bottom())
                  + icon.height() * q[3].y() / geometry.height()));
        q[1].setY(q[1].y() + progressRight * ((icon.top() - quadGlobal.top())
                  + icon.height() * q[1].y() / geometry.height()));
        q[2].setY(q[2].y() + progressRight * ((icon.top() - quadGlobal.bottom())
                  + icon.height() * q[2].y() / geometry.height()));

        q[0].setX(closest); q[1].setX(farthest);
        q[2].setX(farthest); q[3].setX(closest);

        newQuads.append(q);
    }
    data.quads = newQuads;
}


void
JinniEffect::minimizeRight(float progress, QRect geometry,
                           QRect icon, WindowPaintData& data) {
    const float distance = icon.x() + icon.width() - geometry.x();

    WindowQuadList newQuads;
    foreach (WindowQuad q, data.quads) {
        QRect quadGlobal = QRect(QPoint(q.left(), q.top()),
                                 QPoint(q.right(), q.bottom()));
        quadGlobal.translate(geometry.topLeft());

        // Calculate Y coords.

        float farthest = q[0].x(),
              closest = q[1].x();

        if (progress > mShapeTransformTime) {
            const float moveProgress =
                (progress - mShapeTransformTime) / (1 - mShapeTransformTime);
            farthest += moveProgress * distance;
            closest += moveProgress * distance;
        }

        // Calculate Y coords.
        const float weightLeft = 1 - (distance - farthest) / distance;
        float progressLeft = nonlinearity(weightLeft);
        const float weightRight = 1 - (distance - closest) / distance;
        float progressRight = nonlinearity(weightRight);

        if (progress < mShapeTransformTime) {
            float shapeProgress = progress / mShapeTransformTime;
            progressLeft *= shapeProgress;
            progressRight *= shapeProgress;
        }

        q[0].setY(q[0].y() + progressLeft * ((icon.top() - quadGlobal.top())
                  + icon.height() * q[0].y() / geometry.height()));
        q[3].setY(q[3].y() + progressLeft * ((icon.top() - quadGlobal.bottom())
                  + icon.height() * q[3].y() / geometry.height()));
        q[1].setY(q[1].y() + progressRight * ((icon.top() - quadGlobal.top())
                  + icon.height() * q[1].y() / geometry.height()));
        q[2].setY(q[2].y() + progressRight * ((icon.top() - quadGlobal.bottom())
                  + icon.height() * q[2].y() / geometry.height()));

        q[0].setX(farthest); q[1].setX(closest);
        q[2].setX(closest); q[3].setX(farthest);

        newQuads.append(q);
    }
    data.quads = newQuads;
}


void JinniEffect::paintWindow(EffectWindow* w, int mask, QRegion region, WindowPaintData& data) {
    if (mTimeLineWindows.contains(w)) {
        const auto geometry = w->geometry();
        const auto icon = w->iconGeometry();

        const auto position = getIconPosition(icon);
        if (position == IconPosition::Unknown) {
          effects->paintWindow(w, mask, region, data);
          return;
        }

        const auto t = mTimeLineWindows[w]->progress();

        switch (position) {
            case IconPosition::Bottom:
                minimizeBottom(t, geometry, icon, data);
                break;
            case IconPosition::Top:
                minimizeTop(t, geometry, icon, data);
                break;
            case IconPosition::Left:
                minimizeLeft(t, geometry, icon, data);
                break;
            case IconPosition::Right:
                minimizeRight(t, geometry, icon, data);
                break;
            default:
                break;
        }
    }

    effects->paintWindow(w, mask, region, data);
}


void
JinniEffect::postPaintScreen() {
    if (mTimeLineWindows.count() > 0) {
        QHash<EffectWindow*, Timeline*>::iterator entry = mTimeLineWindows.begin();
        while (entry != mTimeLineWindows.end()) {
            auto timeline = entry.value();
            if (timeline->done()) {
                delete timeline;
                entry = mTimeLineWindows.erase(entry);
            } else {
                ++entry;
            }
        }
        effects->addRepaintFull();
    }

    effects->postPaintScreen();
}


void
JinniEffect::windowDeleted(EffectWindow* w) {
    if (mTimeLineWindows.contains(w)) {
        delete mTimeLineWindows.take(w);
    }
}


void
JinniEffect::windowMinimized(EffectWindow* w) {
    if (effects->activeFullScreenEffect()) {
        return;
    }

    if (! mTimeLineWindows.contains(w)) {
        mTimeLineWindows[w] = new Timeline(mDuration, Timeline::Forward);
    }
    else {
        mTimeLineWindows[w]->toggleDirection();
    }
}


void
JinniEffect::windowUnminimized(EffectWindow* w) {
    if (effects->activeFullScreenEffect()) {
        return;
    }

    if (! mTimeLineWindows.contains(w)) {
        mTimeLineWindows[w] = new Timeline(mDuration, Timeline::Backward);
    }
    else {
        mTimeLineWindows[w]->toggleDirection();
    }
}


bool
JinniEffect::isActive() const {
    return !mTimeLineWindows.isEmpty();
}


float
JinniEffect::nonlinearity(float x) {
    float z = 2 * mNonlinearityScale * x - mNonlinearityScale;
    return 1.0f / (1.0f + exp(-z));
}

#include "jinni.moc"

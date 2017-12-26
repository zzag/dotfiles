#ifndef JINNI_TIMELINE_H
#define JINNI_TIMELINE_H

class Timeline
{
public:
    enum Direction {
        Forward,
        Backward
    };

public:
    Timeline(int duration, Direction direction = Forward)
        : mDuration(duration),
          mDirection(direction),
          mElapsed(0),
          mFinished(false) {}

    /**
     * progress returns current progress of the timeline.
     */
    float progress() const {
        auto t = static_cast<float>(mElapsed) / mDuration;
        if (mDirection == Backward) {
            t = 1 - t;
        }
        return t;
    }

    void update(int delta) {
        if (mFinished) {
            return;
        }
        mElapsed += delta;
        if (mElapsed >= mDuration) {
            mFinished = true;
            mElapsed = mDuration;
        }
    }

    /**
     * duration returns current duration of the timeline.
     */
    int duration() const { return mDuration; }

    /**
     * setDuration sets new duration of the timeline.
     */
    void setDuration(int d) { mDuration = d; }

    /**
     * direction returns current direction of the timeline.
     */
    Direction direction() const { return mDirection; }

    /**
     * setDirection sets new direction of the timeline.
     */
    void setDirection(Direction d) {
        if (mDirection == d) {
            return;
        }
        if (mElapsed > 0) {
            mElapsed = mDuration - mElapsed;
        }
        mDirection = d;
    }

    /**
     * toggleDirection toggles direction of the timeline.
     */
    void toggleDirection() {
        setDirection(mDirection == Forward ? Backward : Forward);
    }

    /**
     * done returns whether timeline is done.
     */
    bool done() const { return mFinished; }

    /**
     * reset resets current state of the timeline.
     */
    void reset() {
        mElapsed = 0;
        mFinished = false;
    }

private:
    int mDuration;
    Direction mDirection;

    int mElapsed;
    bool mFinished;
};

#endif

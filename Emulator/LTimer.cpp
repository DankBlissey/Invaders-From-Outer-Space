#include "LTimer.h"
#include "SDL3/SDL_timer.h"
#include <SDL3/SDL.h>

void LTimer::start() {
    mStarted = true;
    mPaused = false;

    mStartTicks = SDL_GetTicksNS();
    mPausedTicks = 0;
}

void LTimer::stop() {
    mStarted = false;
    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause() {
    if (mStarted && !mPaused) {
        mPaused = true;

        mPausedTicks = SDL_GetTicksNS() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause() {
    if (mStarted && mPaused) {
        mPaused = false;

        mStartTicks = SDL_GetTicksNS() - mPausedTicks;

        mPausedTicks = 0;
    }
}

std::uint64_t LTimer::getTicksNS() {
    if (!mStarted) {
        return 0;
    }
    if (mPaused) {
        return mPausedTicks;
    }
    return SDL_GetTicksNS() - mStartTicks;
}

bool LTimer::isStarted() {
    return mStarted;
}

bool LTimer::isPaused() {
    return mPaused;
}
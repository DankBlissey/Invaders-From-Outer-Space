#include <cstdint>
class LTimer {
    public:
        void start();
        void stop();
        void pause();
        void unpause();

        std::uint64_t getTicksNS();

        bool isStarted();
        bool isPaused();

    private:
        std::uint64_t mStartTicks {0};
        std::uint64_t mPausedTicks {0};

        bool mPaused {false};
        bool mStarted {false};
};
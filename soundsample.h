#ifndef SOUNDSAMPLE
#define SOUNDSAMPLE

namespace SoundSample
{
    enum class Frequency {
        Hz1000 = 1000,
        Hz2000 = 2000,
        Hz4000 = 4000,
        Hz8000 = 8000,
        Hz125 = 125,
        Hz250 = 250,
        Hz500 = 500
    };

    enum class Direction {
        Left,
        Right
    };

    enum class VolumeType {
        Decybel,
        Percent
    };
}
#endif // SOUNDSAMPLE


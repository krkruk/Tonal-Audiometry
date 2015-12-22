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
        Left = 0,
        Right = 1,
        OneAfterAnother = 2,
        None = 3
    };

    enum class VolumeType {
        Decybel = 0,
        Percent = 1
    };
}
#endif // SOUNDSAMPLE


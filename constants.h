#ifndef CONSTANTS
#define CONSTANTS
#include <QObject>

namespace Consts {
    constexpr qreal PRESSURE_0dB = 0.00002;
    constexpr qreal PRESSURE_0dB_INVERSED = 1/0.00002;

    constexpr qreal PRESSURE_65dB = 0.036;
    constexpr qreal PRESSURE_65dB_INVERSED = 1/0.036;

    constexpr char VOLUME_PERCENT_LEVEL[] = "VolumePercentLevel";
    constexpr char VOLUME_DECIBEL_SPL_LEVEL[] = "VolumeDecibelSoundPressureLevel";
    constexpr char VOLUME_DECIBEL_HEARING_LEVEL[] = "VolumeDecibelHearingLevel";

    const QString intensityLabel = QObject::tr("Intensity [dB HL]");
    const QString frequencyLabel = QObject::tr("Frequency [Hz]");
}
#endif // CONSTANTS


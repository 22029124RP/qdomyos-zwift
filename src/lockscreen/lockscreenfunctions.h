#ifndef LOCKSCREENFUNCTIONS_H
#define LOCKSCREENFUNCTIONS_H

#include <qglobal.h>

#include "lockscreen/qzlockscreenfunctions.h"

/**
 * @brief Logic for lockscreen functionality and Peloton workaround.
 */
class LockscreenFunctions : public QZLockscreenFunctions {
private:
    bool pelotonWorkaroundActive = false;
    QZLockscreen * lockscreen = nullptr;
    configurationType configType = configurationType::NONE;

    void setVirtualBikePelotonWorkaround(bool zwiftMode);
    void setVirtualTreadmillPelotonWorkaround(bool zwiftMode);
    void setVirtualRowerPelotonWorkaround(bool isVirtualDevice);
public:
    explicit LockscreenFunctions();

    QZLockscreen * getLockscreen() const override;

    bool tryConfigurePelotonWorkaround(configurationType configType, bool zwiftMode) override;

    configurationType getConfigurationType() const override;

    bool isPelotonWorkaroundActive() const override;
    bool updateEnergyDistanceHeartRate(metric kcal, metric distance, metric &heart, const int defaultHeartRate) override;
    bool updateHeartRate(metric &heart) override;
    void updateStepCadence(metric& cadence) override;
    void pelotonBikeUpdateCHR(const double crankRevolutions, const uint16_t lastCrankEventTime, const uint8_t heartRate) override;
    void pelotonTreadmillUpdateCHR(const double crankRevolutions, const uint16_t lastCrankEventTime, const uint8_t heartRate) override;
    void pelotonRowerUpdateCHR(const double crankRevolutions, const uint16_t lastCrankEventTime, const uint8_t heartRate) override;
};


#endif // LOCKSCREENFUNCTIONS_H

#pragma once
#include <lockscreen/qzlockscreenfunctions.h>
#include "devicediscoveryinfo.h"

class LockscreenFunctionsTestData
{
    QZLockscreenFunctions::configurationType configType;
    DeviceDiscoveryInfo settings;
    bool pelotonWorkaroundActive = false;
public:
    LockscreenFunctionsTestData(QZLockscreenFunctions::configurationType configType, bool pelotonWorkaroundActive, const DeviceDiscoveryInfo& settings);

    /**
     * @brief Gets the expected configuration type for the lockscreen functions. This can be different to the
     * lockscreen configuration type for the Peloton workaround as sometimes different "virtual device" configs are
     * used for the same actual device.
     * @return
     */
    QZLockscreenFunctions::configurationType get_lockscreenFunctionsConfigType() const;

    /**
     * @brief Gets the virtual device type the lockscreen is expected to be configured for.
     * If the rower/non-zwiftmode lockscreen configuration is requested and applied, the
     * lockscreen's setup for the virtual bike will be called instead.
     * @return
     */
    QZLockscreenFunctions::configurationType get_lockscreenConfigType() const;

    /**
     * @brief The settings required to discover the device and get the expected lockscreen configuration.
     * @return
     */
    DeviceDiscoveryInfo get_settings() const;

    /**
     * @brief Indicates if the Peloton workaround is expected to be active.
     * @return
     */
    bool get_isPelotonActive() const;   
};


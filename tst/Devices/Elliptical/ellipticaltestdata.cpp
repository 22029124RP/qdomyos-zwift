#include "ellipticaltestdata.h"

void EllipticalTestData::configureLockscreenSettings(const DeviceDiscoveryInfo &info,
                                                     std::vector<LockscreenFunctionsTestData> &configurations) const {
    DeviceDiscoveryInfo config(info);
    auto none = QZLockscreenFunctions::configurationType::NONE;

    // Disabling
    for(int i=0; i<4; i++) {
        config.ios_peloton_workaround = i&1;
        config.bike_cadence_sensor = i&2;
        configurations.push_back(LockscreenFunctionsTestData(none, false, config));
    }
}

EllipticalTestData::EllipticalTestData(std::string testName) : BluetoothDeviceTestData(testName) {}


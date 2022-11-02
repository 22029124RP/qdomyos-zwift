﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "technogymmyruntreadmill.h"

class TechnoGymMyRunTreadmillTestData : public BluetoothDeviceTestData {
protected:
    bool configureSettings(devicediscoveryinfo& info, bool enable) const override {
        info.technogym_myrun_treadmill_experimental = !enable;
        return true;
    }
public:
    TechnoGymMyRunTreadmillTestData() {
        this->addDeviceName("MYRUN ", comparison::StartsWithIgnoreCase);
        this->addDeviceName("MERACH-U3", comparison::StartsWithIgnoreCase);
    }

    deviceType get_expectedDeviceType() const override { return deviceType::TechnoGymMyRunTreadmill; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<technogymmyruntreadmill*>(detectedDevice)!=nullptr;
    }
};


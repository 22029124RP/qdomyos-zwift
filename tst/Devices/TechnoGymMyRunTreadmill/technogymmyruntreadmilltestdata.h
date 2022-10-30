﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "technogymmyruntreadmill.h"

class TechnoGymMyRunTreadmillTestData : public BluetoothDeviceTestData {

public:
    TechnoGymMyRunTreadmillTestData() {

    }


    deviceType get_expectedDeviceType() const override { return deviceType::TechnoGymMyRunTreadmill; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<technogymmyruntreadmill*>(detectedDevice)!=nullptr;
    }
};


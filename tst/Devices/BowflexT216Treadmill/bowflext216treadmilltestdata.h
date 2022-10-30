﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "bowflext216treadmill.h"

class BowflexT216TreadmillTestData : public BluetoothDeviceTestData {

public:
    BowflexT216TreadmillTestData() {
        this->addDeviceName("BOWFLEX T216", true, true);
    }

    deviceType get_expectedDeviceType() const override { return deviceType::BowflexT216Treadmill; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<bowflext216treadmill*>(detectedDevice)!=nullptr;
    }
};


﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "bowflextreadmill.h"

class BowflexTreadmillTestData : public BluetoothDeviceTestData {

public:
    BowflexTreadmillTestData() {}

    QStringList get_deviceNames() const override {
        QStringList result;

        return result;
    }

    deviceType get_expectedDeviceType() const override {
        // device not supported
        return deviceType::None;
    }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<bowflextreadmill*>(detectedDevice)!=nullptr;
    }
};


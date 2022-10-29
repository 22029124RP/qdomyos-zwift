﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "nautilustreadmill.h"

class NautilusTreadmillTestData : public BluetoothDeviceTestData {

public:
    NautilusTreadmillTestData() {}

    QStringList get_deviceNames() const override {
        QStringList result;

        return result;
    }

    deviceType get_expectedDeviceType() const override { return deviceType::NautilusTreadmill; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<nautilustreadmill*>(detectedDevice)!=nullptr;
    }
};


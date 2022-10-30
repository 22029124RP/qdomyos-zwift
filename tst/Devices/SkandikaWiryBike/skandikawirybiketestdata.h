﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "skandikawiribike.h"

class SkandikaWiryBikeTestData : public BluetoothDeviceTestData {

public:
    SkandikaWiryBikeTestData() {}

    QStringList get_deviceNames() const override {
        QStringList result;

        return result;
    }

    deviceType get_expectedDeviceType() const override { return deviceType::SkandikawiriBike; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<skandikawiribike*>(detectedDevice)!=nullptr;
    }
};


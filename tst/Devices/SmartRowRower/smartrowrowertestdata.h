﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "smartrowrower.h"

class SmartRowRowerTestData : public BluetoothDeviceTestData {

public:
    SmartRowRowerTestData() {
        this->addDeviceName("SMARTROW", comparison::StartsWith);
    }

    deviceType get_expectedDeviceType() const override { return deviceType::SmartRowRower; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<smartrowrower*>(detectedDevice)!=nullptr;
    }
};


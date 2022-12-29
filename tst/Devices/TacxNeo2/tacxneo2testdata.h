﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "tacxneo2.h"

class TacxNeo2TestData : public BluetoothDeviceTestData {

public:
    TacxNeo2TestData() : BluetoothDeviceTestData("Tacx Neo 2 Bike") {
        this->addDeviceName("TACX NEO", comparison::StartsWithIgnoreCase);
        this->addDeviceName("TACX FLOW", comparison::StartsWithIgnoreCase);
        this->addDeviceName("TACX SMART BIKE", comparison::StartsWithIgnoreCase);
    }

    deviceType get_expectedDeviceType() const override { return deviceType::TacxNeo2; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<tacxneo2*>(detectedDevice)!=nullptr;
    }
};


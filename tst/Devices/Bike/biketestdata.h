﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "bike.h"

class BikeTestData : public BluetoothDeviceTestData {

public:
    BikeTestData() : BluetoothDeviceTestData("Bike (Abstract)") {}

    QStringList get_deviceNames() const override {
        QStringList result;

        return result;
    }

    deviceType get_expectedDeviceType() const override {
        return deviceType::None; // abstract
    }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<bike*>(detectedDevice)!=nullptr;
    }
};


﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "schwinnic4bike.h"

class SchwinnIC4BikeTestData : public BluetoothDeviceTestData {

public:
    SchwinnIC4BikeTestData() {

        this->addDeviceName("IC BIKE", comparison::StartsWithIgnoreCase);
        this->addDeviceName("C7-", comparison::StartsWithIgnoreCase);
        this->addDeviceName("C9/C10", comparison::StartsWithIgnoreCase);

        // 17 characters, beginning with C7-
        this->addInvalidDeviceName("C7-456789ABCDEFG", comparison::IgnoreCase);
    }


    deviceType get_expectedDeviceType() const override { return deviceType::SchwinnIC4Bike; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<schwinnic4bike*>(detectedDevice)!=nullptr;
    }
};


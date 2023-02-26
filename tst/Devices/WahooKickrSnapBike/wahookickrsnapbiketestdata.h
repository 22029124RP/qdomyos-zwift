﻿#pragma once

#include "Devices/Bike/biketestdata.h"


class WahooKickrSnapBikeTestData : public BluetoothDeviceTestData {

public:
    WahooKickrSnapBikeTestData();

    deviceType get_expectedDeviceType() const override;

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override;
};


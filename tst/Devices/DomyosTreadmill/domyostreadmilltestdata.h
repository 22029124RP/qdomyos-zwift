﻿#pragma once

#include "Devices/Treadmill/treadmilltestdata.h"


class DomyosTreadmillTestData : public TreadmillTestData {

public:
    DomyosTreadmillTestData();

    void configureExclusions() override;

    deviceType get_expectedDeviceType() const override;

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override;
};


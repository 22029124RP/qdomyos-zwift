﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "technogymmyruntreadmillrfcomm.h"

class TechnogymMyRunTreadmillRFCommTestData : public BluetoothDeviceTestData {
protected:
    bool configureSettings(devicediscoveryinfo& info, bool enable) const override {
        info.technogym_myrun_treadmill_experimental = enable;
        return true;
    }
public:
    TechnogymMyRunTreadmillRFCommTestData() : BluetoothDeviceTestData("Technogym MyRun Treadmill RF Comm") {
        this->addDeviceName("MYRUN ", comparison::StartsWithIgnoreCase);
        this->addDeviceName("MERACH-U3", comparison::StartsWithIgnoreCase);
    }
    deviceType get_expectedDeviceType() const override { return deviceType::TechnoGymMyRunTreadmillRfComm; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<technogymmyruntreadmillrfcomm*>(detectedDevice)!=nullptr;
    }
};


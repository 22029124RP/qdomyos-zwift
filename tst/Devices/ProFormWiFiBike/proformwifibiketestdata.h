﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "proformwifibike.h"

class ProFormWiFiBikeTestData : public BluetoothDeviceTestData {
protected:
    bool configureSettings(devicediscoveryinfo& info, bool enable) const override {
        info.proformtdf4ip = enable ? this->get_testIP():QString();
        return true;
    }
public:
    ProFormWiFiBikeTestData() {
        // any name
        this->addDeviceName("", comparison::StartsWithIgnoreCase);
    }

    deviceType get_expectedDeviceType() const override { return deviceType::ProformWifiBike; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<proformwifibike*>(detectedDevice)!=nullptr;
    }
};


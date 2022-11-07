﻿#pragma once

#include "Devices/bluetoothdevicetestdata.h"
#include "nordictrackifitadbtreadmill.h"

class NordicTrackIFitADBTreadmillTestData : public BluetoothDeviceTestData {
protected:
    bool configureSettings(devicediscoveryinfo& info, bool enable) const override {
        if(enable)
            info.nordictrack_2950_ip = this->get_testIP();
        else
            info.nordictrack_2950_ip = QString();

        return true;
    }
public:
    NordicTrackIFitADBTreadmillTestData() : BluetoothDeviceTestData("Nordictrack IFit ADB Treadmill") {
        // Allow any name because it's not a bluetooth device
        this->addDeviceName("", comparison::StartsWithIgnoreCase);
    }

    deviceType get_expectedDeviceType() const override { return deviceType::NordicTrackIFitADBTreadmill; }

    bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const override {
        return dynamic_cast<nordictrackifitadbtreadmill*>(detectedDevice)!=nullptr;
    }
};


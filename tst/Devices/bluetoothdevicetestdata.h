#pragma once

#include <bluetoothdevice.h>
#include <QStringList>
#include <vector>
#include <memory>
#include "discovereddevice.h"

class BluetoothDeviceTestData  {
    std::vector<std::shared_ptr<BluetoothDeviceTestData>> exclusions;
    QStringList deviceNames;
    QStringList invalidDeviceNames;
protected:


    void exclude(BluetoothDeviceTestData* testData);

    /**
     * @brief Add a device name that should be identified as this device.
     * @param deviceName
     */
    void addDeviceName(const QString& deviceName, bool multiCase, bool isStartsWith);

    /**
     * @brief Add a device name as a prefix and suffix that should be identified as this device.
     * @param deviceName
     */
    void addDeviceName(const QString& deviceNameStartsWith, const QString& deviceNameEndsWith, bool multiCase);

    /**
     * @brief Add a device name that should NOT be identified as this device.
     * @param deviceName
     */
    void addInvalidDeviceName(const QString& deviceName);

public:
    BluetoothDeviceTestData() {
        // You can do set-up work for each test here.
    }
    virtual ~BluetoothDeviceTestData() {}

    /**
     * @brief A list of bluetooth device names that should be recognised as this device.
     */
    virtual QStringList get_deviceNames() const;

    /**
     * @brief A list of bluetooth device names that should NOT be recognised as this device.
     * By default this is generated by modifying the result of get_deviceNames().
     */
    virtual QStringList get_failingDeviceNames() const;

    /**
     * @brief Gets a list of BluetoothDeviceTestData objects for devices whose
     * prior detection should prevent the detection of this device when an expected device name is found.
     * @return
     */
    virtual std::vector<std::shared_ptr<BluetoothDeviceTestData>> get_exclusions() const;

    virtual deviceType get_expectedDeviceType() const =0;
    virtual bool get_isExpectedDevice(bluetoothdevice * detectedDevice) const =0;

};



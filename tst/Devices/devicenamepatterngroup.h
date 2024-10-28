#pragma once

#include <QStringList>

/**
     * @brief Indicates how bluetooth device names should be compared.
     */
enum DeviceNameComparison : int {
    Exact = 0,
    IgnoreCase = 1,
    StartsWith = 2,
    StartsWithIgnoreCase = DeviceNameComparison::IgnoreCase+DeviceNameComparison::StartsWith
};


/**
 * @brief The DeviceNamePatternGroup class manages groups of device name patterns.
 * Valid names are added, and invalid names are generated.
 */
class DeviceNamePatternGroup
{
private:
    QStringList deviceNames;
    QStringList invalidDeviceNames;

    /**
     * @brief Add the specified device names with different casings.
     * Used to provide device names to test case sensitivity or insensitivity.
     * @param names
     * @param target
     */
    static void addDifferentCasings(const QStringList &names, QStringList &target);
public:
    DeviceNamePatternGroup();

    /**
     * @brief Add a device name that should be identified as this device.
     * If the comparison specified contains "starts with", then samples that start with the
     * specified device name will be added. If a case sensitive comparison is specified,
     * samples with invalid casing will be added to the invalid names list.
     * @param deviceName
     * @param length The expected length. Use 0 for unrestricted.
     */
    DeviceNamePatternGroup * addDeviceName(const QString& deviceName, DeviceNameComparison cmp, uint8_t length=0);

    /**
     * @brief Add a device name as a prefix and suffix that should be identified as this device.
     * If a case sensitive comparison is specified,
     * samples with invalid casing will be added to the invalid names list.
     * @param deviceName
     */
    DeviceNamePatternGroup * addDeviceName(const QString& deviceNameStartsWith, const QString& deviceNameEndsWith, DeviceNameComparison cmp);

    /**
     * @brief Add a device name that should NOT be identified as this device.
     * @param deviceName
     */
    DeviceNamePatternGroup * addInvalidDeviceName(const QString& deviceName, DeviceNameComparison cmp);

    /**
     * @brief A list of bluetooth device names that should be recognised as this device.
     */
    QStringList DeviceNames() const { return this->deviceNames;}


    /**
     * @brief A list of bluetooth device names that should NOT be recognised as this device.
     * By default this is generated by modifying the result of get_deviceNames().
     */
    QStringList InvalidDeviceNames() const { return this->invalidDeviceNames; }

};



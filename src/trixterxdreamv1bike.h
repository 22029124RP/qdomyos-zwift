#pragma once
#include "bike.h"
#include "trixterxdreamv1client.h"
#include "trixterxdreamv1serial.h"
#include "trixterxdreamv1settings.h"
#include "virtualbike.h"
#include <queue>

class trixterxdreamv1bike : public bike
{
    Q_OBJECT

private:
    /**
     * @brief SettingsUpdateTimerIntervalMilliseconds The object will check for a settings update at this interval.
     */
    constexpr static int SettingsUpdateTimerIntervalMilliseconds = 10000;

    /**
     * @brief Mapping from cadence and resistance to power.
     */
    static double powerSurface[260][3];

    /**
     * @brief A queue of states read from the client. Syncronized but unprocessedStatesMutex.
     */
    std::queue<trixterxdreamv1client::state> unprocessedStates[2];

    /**
     * @brief The current unprocessed state queue. Syncronized but unprocessedStatesMutex.
     */
    uint32_t unprocessedStateIndex = 0;

    /**
     * @brief Mutex for accessing the unprocessedStates queue.
     */
    QMutex unprocessedStatesMutex;

    /**
     * @brief An object that processes incoming data to CSCS, heart rate and steering data
     */
    trixterxdreamv1client client;

    /**
     * @brief An object that monitors a serial port to read incoming data, and to write
     * resistance level requests.
     */
    trixterxdreamv1serial * port = nullptr;

    /**
     * @brief The bridge to the client application.
     */
    virtualbike * virtualBike = nullptr;

    /**
     * @brief Indicates if the deice should be sent full resistance instead of the currently requested resistance.
     */
    bool stopping = false;

    /**
     * @brief Indicates if a power boost is being applied.
     */
    bool powerBoost = false;

    /**
     * @brief Sum of brakes 1 and 2 each normalised to 0..125.
     */
    uint8_t brakeLevel = 0;

    /**
     * @brief The id for identifying the resistance timer in void timerEvent(QEvent*).
     */
    int resistanceTimerId = 0;

    /**
     * @brief The id for identifying the settings update timer in void timerEvent(QEVent*).
     */
    int settingsUpdateTimerId = 0;

    /**
     * @brief The id for identifying the timer that updates the metrics from the stored queue of states read from the client,
     * in void timerEvent(QEvent*),
     */
    int metricsUpdateTimerId = 0;

    /**
     * @brief Suppress heart rate readings, QZ level setting.
     */
    bool noHeartService;

    /**
     * @brief Value from app settings combined with QZ's noHeartService value.
     */
    bool noHeartRate;

    /**
     * @brief Suppress virtual device.
     */
    bool noVirtualDevice;

    /**
     * @brief Suppress sending resistance to device.
     */
    bool noWriteResistance;

    /**
     * @brief Suppress steering readings
     */
    bool noSteering;

    /**
     * @brief The last requested resistance level, actual value sent to device.
     */
    resistance_t resistanceLevel = 0;

    /**
     * @brief Option to use resistance levels 0..100 instead of
     * a full range that exceeds an int8_t.
     */
    bool useResistancePercentage = false;

    /**
     * @brief The simulated circumference of the bike's wheels, for converting
     * angular velocity to a speed. Units: kilometers.
     */
    double wheelCircumference;

    /**
     * @brief t0 The start time in milliseconds. Used to reduce te size of time values processed.
     */
    qint64 t0=0;

    /**
     * @brief The number of packets processed.
     */
    uint32_t packetsProcessed=0;

    /**
     * @brief The last time (from getTime()) a packet was processed.
     */
    uint32_t lastPacketProcessedTime=0;

    /**
     * @brief The application settings.
     */
    trixterxdreamv1settings * appSettings = nullptr;

    /**
     * @brief The last app settings version that was used to configure the object.
     */
    uint32_t lastAppSettingsVersion=0;

    /**
     * @brief Stores the mapping between incoming steering values and the steering angles expected by the application.
     */
    std::vector<double> steeringMap;

    /**
     * @brief Used to synchronise updates to this object's members.
     */
    QRecursiveMutex updateMutex;

    /**
     * @brief Processes the state queue
     */
    void update();

    /**
     * @brief Gets the time in miliseconds since this object was created.
     */
    static uint32_t getTime();

    /**
     * @brief Called by the data source (serial port) when a new block of data arrives.
     * Stores the data and triggers an update.
     * @param bytes
     */
    void receiveBytes(const QByteArray &bytes);

    /**
     * @brief Called by the resistanceTimer to send the resistance request to the
     * device.
     */
    void updateResistance();

    /**
     * @brief Calculates the mapping between steering values coming from the device, and
     * the steering angles sent to the application. Uses the values in the appSettings field.
     */
    void calculateSteeringMap();

    /**
     * @brief Set up the bridge to the client application.
     */
    void configureVirtualBike();

    /**
     * @brief Calculate power from cadence RPM and resistance.
     * @param cadenceRPM
     * @param resistance Bike resistance on full, not percentage scale.
     * @return
     */
    double calculatePower(int cadenceRPM, int resistance);

    /**
     * @brief Calculate resistance from current inclination and cadence.
     * @return
     */
    resistance_t calculateResistanceFromInclination();

    /**
     * @brief Adjust the resistance based on whether the
     * object has been configured to use a resistance percentage or the
     * raw value.
     * @param input 0..100 if the object is using a resistance percentage,
     * 0..trixterxdreamclient::MaxResistance otherwise.
     * @param toDevice The direction of the conversion.
     * @return
     */
    resistance_t adjustedResistance(resistance_t input, bool toDevice);

    /**
     * @brief Called to set the resistance level sent to the device.
     * @param resistanceLevel The resistance level to request (0..maximumResistance())
     */
    void set_resistance(resistance_t resistanceLevel);


protected:

    /**
     * @brief Processes timer events, e.g. for resistance.
     * @param event
     */
    void timerEvent(QTimerEvent *event) override;

    /**
     * @brief Disconnect the serial port and resistance timer.
     */
    void disconnectPort();

public:

    /**
     * @brief The maximum supported wheel diameter. Unit: meters
     */
    constexpr static double MaxWheelDiameter = 2.0;

    /**
     * @brief The minimum supported wheel diameter. Unit: meters
     */
    constexpr static double MinWheelDiameter = 0.1;

    /**
     * @brief The default wheel diameter. Unit: meters
     */
    constexpr static double DefaultWheelDiameter = 26*0.0254;

    /**
     * @brief The number of milliseconds of no packets processed required before
     * this object will be considered disconnected from the device.
     */
    constexpr static int32_t DisconnectionTimeout = 50;

    /**
     * @brief The number of milliseconds to collect packets from the device before updating the metrics.
     */
    constexpr static int32_t UpdateMetricsInterval = 100;

    /**
     * @brief Constructor
     * @param noWriteResistance Option to avoid sending resistance to the device.
     * @param noHeartService Option to avoid using the heart rate reading.
     * @param noVirtualDevice Option to avoid using a virtual device.
     */
    trixterxdreamv1bike(bool noWriteResistance, bool noHeartService, bool noVirtualDevice);

    ~trixterxdreamv1bike();

    /**
     * @brief Calculate the power for the requested resistance at the current cadence.
     * @param requestedResistance
     * @return
     */
    uint16_t powerFromResistanceRequest(resistance_t requestedResistance) override;

    /**
     * @brief Calculate the resistance required to produce the requested power at the current cadence.
     * @param power
     * @return
     */
    resistance_t resistanceFromPowerRequest(uint16_t power) override;

    /**
     * @brief Virtual device
     * @return
     */
    void *VirtualDevice() override;

    /**
     * @brief Attempt to connect to the specified port.
     * @param portName The name of the serial port to connect to.
     */
    bool connect(QString portName);

    /**
     * @brief Indicates if a valid packet was received from the device within the DisconnectionTimeout.
     */
    bool connected() override;

    /**
     * @brief Set the simulated wheel diameter to be used for converting angular velocity to speed. Units: meters
     * @param value
     */
    void set_wheelDiameter(double value);

    /**
     * @brief Gets the settings object for this device type.
     */
    const trixterxdreamv1settings * get_appSettings() { return this->appSettings; }

    /**
     * @brief The maximum resistance supported.
     * @return
     */
    resistance_t maxResistance() override { return this->useResistancePercentage ? 100:trixterxdreamv1client::MaxResistance; }

    /**
     * @brief Map Peloton 0 to 100% resistance to the bike's range.
     * @param pelotonResistance The Peloton resistance. Range: 0 to 100.
     * @return The Trixter X-Dream V1 bike resistance. Range 0..250 if !this->useResistancePercentage.
     */
    resistance_t pelotonToBikeResistance(int pelotonResistance) override;

    /**
     * @brief Attempt to create an object to interact with an existing Trixter X-Dream V1 bike on a specific serial port,
     * or if the port is unspecified, any serial port.
     * @param noWriteResistance Option to avoid sending resistance to the device.
     * @param noHeartService Option to avoid using the heart rate reading.
     * @param noVirtualDevice Option to avoid using a virtual device.
     * @param portName (Optional) The specific port to search.
     * @return nullptr if no device is found, an object if a device is found and connected.
     */
    static trixterxdreamv1bike * tryCreate(bool noWriteResistance, bool noHeartService, bool noVirtualDevice, const QString& portName = nullptr);

    /**
     * @brief Attempt to create an object to interact with an existing Trixter X-Dream V1 bike on a specific serial port,
     * or if the port is unspecified, any serial port.
     * @param port (Optional) The specific port to search.
     * @return
     */
    static trixterxdreamv1bike * tryCreate(const QString& portName = nullptr);

};

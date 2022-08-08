#ifndef TREADMILL_H
#define TREADMILL_H
#include "bluetoothdevice.h"
#include <QObject>

class treadmill : public bluetoothdevice {
    Q_OBJECT

  public:
    treadmill();
    void update_metrics(bool watt_calc, const double watts);
    metric lastRequestedSpeed() { return RequestedSpeed; }
    metric lastRequestedInclination() { return RequestedInclination; }
    virtual bool connected() override;
    virtual metric currentInclination() override;
    virtual double requestedSpeed();
    virtual double currentTargetSpeed();
    virtual double requestedInclination();
    virtual double minStepInclination();
    virtual double minStepSpeed();
    metric currentStrideLength() { return InstantaneousStrideLengthCM; }
    metric currentGroundContact() { return GroundContactMS; }
    metric currentVerticalOscillation() { return VerticalOscillationMM; }
    uint16_t watts(double weight);
    bluetoothdevice::BLUETOOTH_TYPE deviceType() override;
    void clearStats() override;
    void setLap() override;
    void setPaused(bool p) override;
    virtual void setLastSpeed(double speed);
    virtual void setLastInclination(double inclination);
    virtual bool autoPauseWhenSpeedIsZero();
    virtual bool autoStartWhenSpeedIsGreaterThenZero();

  public slots:
    virtual void changeSpeed(double speed);
    virtual void changeInclination(double grade, double percentage) override;
    virtual void changeSpeedAndInclination(double speed, double inclination);
    virtual void cadenceSensor(uint8_t cadence) override;
    virtual void powerSensor(uint16_t power) override;
    virtual void speedSensor(double speed) override;
    virtual void instantaneousStrideLengthSensor(double length) override;
    virtual void groundContactSensor(double groundContact) override;
    virtual void verticalOscillationSensor(double verticalOscillation) override;

  signals:
    void tapeStarted();

  protected:
    volatile double requestSpeed = -1;
    double targetSpeed = -1;
    double requestInclination = -100;
    double lastSpeed = 0.0;
    double lastInclination = 0;
    metric RequestedSpeed;
    metric RequestedInclination;
    metric InstantaneousStrideLengthCM;
    metric GroundContactMS;
    metric VerticalOscillationMM;
};

#endif // TREADMILL_H

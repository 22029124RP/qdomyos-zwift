#ifndef PAFERSBIKE_H
#define PAFERSBIKE_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/qlowenergyadvertisingdata.h>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristic.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/qlowenergydescriptordata.h>
#include <QtBluetooth/qlowenergyservice.h>
#include <QtBluetooth/qlowenergyservicedata.h>
#include <QtCore/qbytearray.h>

#ifndef Q_OS_ANDROID
#include <QtCore/qcoreapplication.h>
#else
#include <QtGui/qguiapplication.h>
#endif
#include <QtCore/qlist.h>
#include <QtCore/qmutex.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qtimer.h>

#include <QDateTime>
#include <QObject>
#include <QString>

#include "bike.h"
#include "virtualbike.h"

class pafersbike : public bike {
    Q_OBJECT
  public:
    pafersbike(bool noWriteResistance, bool noHeartService, uint8_t bikeResistanceOffset, double bikeResistanceGain);
    resistance_t pelotonToBikeResistance(int pelotonResistance);
    resistance_t resistanceFromPowerRequest(uint16_t power);
    resistance_t maxResistance() { return max_resistance; }
    bool connected();

    void *VirtualBike();
    void *VirtualDevice();

  private:
    const resistance_t max_resistance = 24;
    double bikeResistanceToPeloton(double resistance);
    double GetDistanceFromPacket(const QByteArray &packet);
    uint16_t wattsFromResistance(double resistance);
    QTime GetElapsedFromPacket(const QByteArray &packet);
    void btinit();
    void writeCharacteristic(uint8_t *data, uint8_t data_len, const QString &info, bool disable_log = false,
                             bool wait_for_response = false);
    void startDiscover();
    void forceResistance(resistance_t requestResistance);
    double GetWattFromPacket(const QByteArray &packet);
    void sendPoll();
    uint16_t watts();

    QTimer *refresh;
    virtualbike *virtualBike = nullptr;

    QLowEnergyService *gattCommunicationChannelService = nullptr;
    QLowEnergyCharacteristic gattWriteCharacteristic;
    QLowEnergyCharacteristic gattNotify1Characteristic;

    uint8_t bikeResistanceOffset = 4;
    double bikeResistanceGain = 1.0;
    uint8_t sec1Update = 0;
    QByteArray lastPacket;
    QDateTime lastRefreshCharacteristicChanged = QDateTime::currentDateTime();
    
    resistance_t lastResistanceBeforeDisconnection = -1;

    bool initDone = false;
    bool initRequest = false;

    bool noWriteResistance = false;
    bool noHeartService = false;

  Q_SIGNALS:
    void disconnected();

  public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

  private slots:

    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);
    void characteristicWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);
    void descriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);
    void stateChanged(QLowEnergyService::ServiceState state);
    void controllerStateChanged(QLowEnergyController::ControllerState state);

    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceScanDone(void);
    void update();
    void error(QLowEnergyController::Error err);
    void errorService(QLowEnergyService::ServiceError);
};

#endif // PAFERSBIKE_H

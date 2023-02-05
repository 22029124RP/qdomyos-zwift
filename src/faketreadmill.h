#ifndef FAKETREADMILL_H
#define FAKETREADMILL_H

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

#include "treadmill.h"
#include "virtualbike.h"
#include "virtualtreadmill.h"

class faketreadmill : public treadmill {
    Q_OBJECT
  public:
    faketreadmill(bool noWriteResistance, bool noHeartService, bool noVirtualDevice);
    bool connected();

    void *VirtualBike();
    void *VirtualTreadmill();
    void *VirtualDevice();

  private:
    QTimer *refresh;
    virtualbike *virtualBike = nullptr;
    virtualtreadmill *virtualTreadmill = nullptr;

    uint8_t sec1Update = 0;
    QByteArray lastPacket;
    QDateTime lastRefreshCharacteristicChanged = QDateTime::currentDateTime();
    QDateTime lastGoodCadence = QDateTime::currentDateTime();

    bool initDone = false;
    bool initRequest = false;

    bool noWriteResistance = false;
    bool noHeartService = false;
    bool noVirtualDevice = false;

  signals:
    void disconnected();
    void debug(QString string);

  private slots:
    void changeInclinationRequested(double grade, double percentage);
    void update();

    void ftmsCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);
};

#endif // FAKETREADMILL_H

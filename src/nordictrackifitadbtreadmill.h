#ifndef NORDICTRACKIFITADBTREADMILL_H
#define NORDICTRACKIFITADBTREADMILL_H

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
#include <QThread>
#include <QUdpSocket>

#include "treadmill.h"
#include "virtualbike.h"
#include "virtualtreadmill.h"

#ifdef Q_OS_IOS
#include "ios/lockscreen.h"
#endif

class nordictrackifitadbtreadmillAdbThread : public QThread {
    Q_OBJECT

  public:
    explicit nordictrackifitadbtreadmillAdbThread(QString s);

    void run();

  signals:
    void onSpeedInclination(double speed, double inclination);

  private:
    QString name;
    struct adbfile {
        QDateTime date;
        QString name;
    };

    static bool dtcomp(adbfile left, adbfile right) { return left.date > right.date; }
    QString runAdbCommand(QString command);
};

class nordictrackifitadbtreadmillLogcatAdbThread : public QThread {
    Q_OBJECT

  public:
    explicit nordictrackifitadbtreadmillLogcatAdbThread(QString s);

    void run();

  signals:
    void onSpeedInclination(double speed, double inclination);

  private:
    QString name;
    struct adbfile {
        QDateTime date;
        QString name;
    };

    static bool dtcomp(adbfile left, adbfile right) { return left.date > right.date; }
    QString runAdbCommand(QString command);
    void runAdbTailCommand(QString command);
};

class nordictrackifitadbtreadmill : public treadmill {
    Q_OBJECT
  public:
    nordictrackifitadbtreadmill(bool noWriteResistance, bool noHeartService);
    bool connected();

    void *VirtualTreadmill();
    void *VirtualDevice();
    virtual bool canStartStop() { return false; }

  private:
    void forceIncline(double incline);
    void forceSpeed(double speed);

    QTimer *refresh;
    virtualtreadmill *virtualTreadmill = nullptr;
    virtualbike *virtualBike = nullptr;

    uint8_t sec1Update = 0;
    QDateTime lastRefreshCharacteristicChanged = QDateTime::currentDateTime();
    uint8_t firstStateChanged = 0;
    uint16_t m_watts = 0;

    bool initDone = false;
    bool initRequest = false;

    bool noWriteResistance = false;
    bool noHeartService = false;

    QUdpSocket *socket = nullptr;
    QHostAddress lastSender;

    nordictrackifitadbtreadmillAdbThread *adbThread = nullptr;
    nordictrackifitadbtreadmillLogcatAdbThread *logcatAdbThread = nullptr;

#ifdef Q_OS_IOS
    lockscreen *h = 0;
#endif

#ifdef Q_OS_ANDROID
    QString lastCommand = "";
#endif

  signals:
    void disconnected();
    void debug(QString string);

  private slots:

    void onSpeedInclination(double speed, double inclination);

    void processPendingDatagrams();
    void changeInclinationRequested(double grade, double percentage);

    void update();
};

#endif // NORDICTRACKIFITADBTREADMILL_H

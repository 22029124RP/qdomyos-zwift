#ifndef PELOTON_H
#define PELOTON_H

#include "bluetooth.h"
#include "powerzonepack.h"
#include "trainprogram.h"
#include <QAbstractOAuth2>
#include <QDesktopServices>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QObject>
#include <QSettings>
#include <QTimer>
#include <QUrlQuery>

class peloton : public QObject {
    Q_OBJECT
  public:
    explicit peloton(bluetooth *bl, QObject *parent = nullptr);
    QList<trainrow> trainrows;

    QString user_id;
    QString current_workout_id = QLatin1String("");
    QString current_workout_name = QLatin1String("");
    QString current_workout_status = QLatin1String("");
    QString current_instructor_id = QLatin1String("");
    QString current_instructor_name = QLatin1String("");
    QString current_ride_id = QLatin1String("");

  private:
    const int peloton_workout_second_resolution = 10;
    bool peloton_credentials_wrong = false;
    QNetworkAccessManager *mgr = nullptr;

    QJsonDocument current_workout;
    QJsonDocument current_workout_summary;
    QJsonDocument workout;
    QJsonDocument instructor;
    QJsonDocument performance;

    QTimer *timer;

    bluetooth *bluetoothManager = nullptr;
    powerzonepack *PZP = nullptr;

    int total_workout;
    void getWorkoutList(int num);
    void getSummary(const QString &workout);
    void getWorkout(const QString &workout);
    void getInstructor(const QString &instructor_id);
    void getPerformance(const QString &workout);

  private slots:
    void login_onfinish(QNetworkReply *reply);
    void workoutlist_onfinish(QNetworkReply *reply);
    void summary_onfinish(QNetworkReply *reply);
    void workout_onfinish(QNetworkReply *reply);
    void performance_onfinish(QNetworkReply *reply);
    void instructor_onfinish(QNetworkReply *reply);
    void pzp_trainrows(QList<trainrow> *list);
    void pzp_loginState(bool ok);

    void startEngine();

  signals:
    void loginState(bool ok);
    void pzpLoginState(bool ok);
    void workoutStarted(QString name, QString instructor);
    void workoutChanged(QString name, QString instructor);
};

#endif // PELOTON_H

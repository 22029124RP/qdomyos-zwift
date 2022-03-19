#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

class lockscreen {
  public:
    void setTimerDisabled();
    void request();
    long heartRate();
    long stepCadence();
    void setKcal(double kcal);
    void setDistance(double distance);

    // virtualbike
    void virtualbike_ios();
    void virtualbike_setHeartRate(unsigned char heartRate);
    void virtualbike_setCadence(unsigned short crankRevolutions, unsigned short lastCrankEventTime);

    void virtualbike_zwift_ios();
    double virtualbike_getCurrentSlope();
    double virtualbike_getPowerRequested();
    bool virtualbike_updateFTMS(unsigned short normalizeSpeed, unsigned char currentResistance,
                                unsigned short currentCadence, unsigned short currentWatt, unsigned short  CrankRevolutions, unsigned short  LastCrankEventTime);
    int virtualbike_getLastFTMSMessage(unsigned char* message);
    
    // virtualtreadmill
    void virtualtreadmill_zwift_ios();
    void virtualtreadmill_setHeartRate(unsigned char heartRate);
    double virtualtreadmill_getCurrentSlope();
    uint64_t virtualtreadmill_lastChangeCurrentSlope();
    double virtualtreadmill_getPowerRequested();
    bool virtualtreadmill_updateFTMS(unsigned short normalizeSpeed, unsigned char currentResistance,
                                unsigned short currentCadence, unsigned short currentWatt);
    
    // volume
    double getVolume();
};

#endif // LOCKSCREEN_H

#pragma once

#include <QMap>
#include <memory>

#include "gtest/gtest.h"
#include "bluetoothdevicetestdata.h"
#include "ActivioTreadmill/activiotreadmilltestdata.h"
#include "BHFitnessElliptical/bhfitnessellipticaltestdata.h"
#include "Bike/biketestdata.h"
#include "BowflexT216Treadmill/bowflext216treadmilltestdata.h"
#include "BowflexTreadmill/bowflextreadmilltestdata.h"
#include "Chronobike/chronobiketestdata.h"
#include "Concept2SkiErg/concept2skiergtestdata.h"
#include "CSCBike/cscbiketestdata.h"
#include "DomyosBike/domyosbiketestdata.h"
#include "DomyosElliptical/domyosellipticaltestdata.h"
#include "DomyosRower/domyosrowertestdata.h"
#include "DomyosTreadmill/domyostreadmilltestdata.h"
#include "EchelonConnectSportBike/echelonconnectsportbiketestdata.h"
#include "EchelonRower/echelonrowertestdata.h"
#include "EchelonStrideTreadmill/echelonstridetreadmilltestdata.h"
#include "Elliptical/ellipticaltestdata.h"
#include "ESLinkerTreadmill/eslinkertreadmilltestdata.h"
#include "FakeBike/fakebiketestdata.h"
#include "FakeElliptical/fakeellipticaltestdata.h"
#include "FakeTreadmill/faketreadmilltestdata.h"
#include "FitPlusBike/fitplusbiketestdata.h"
#include "FitshowTreadmill/fitshowtreadmilltestdata.h"
#include "FlywheelBike/flywheelbiketestdata.h"
#include "FTMSBike/ftmsbiketestdata.h"
#include "FTMSRower/ftmsrowertestdata.h"
#include "HorizonGR7Bike/horizongr7biketestdata.h"
#include "HorizonTreadmill/horizontreadmilltestdata.h"
#include "iConceptBike/iconceptbiketestdata.h"
#include "InspireBike/inspirebiketestdata.h"
#include "KeepBike/keepbiketestdata.h"
#include "KingsmithR1ProTreadmill/kingsmithr1protreadmilltestdata.h"
#include "KingsmithR2Treadmill/kingsmithr2treadmilltestdata.h"
#include "M3IBike/m3ibiketestdata.h"
#include "MCFBike/mcfbiketestdata.h"
#include "MepanelBike/mepanelbiketestdata.h"
#include "NautilusBike/nautilusbiketestdata.h"
#include "NautilusElliptical/nautilusellipticaltestdata.h"
#include "NautilusTreadmill/nautilustreadmilltestdata.h"
#include "NordicTrackElliptical/nordictrackellipticaltestdata.h"
#include "NordicTrackIFitADBTreadmill/nordictrackifitadbtreadmilltestdata.h"
#include "NPECableBike/npecablebiketestdata.h"
#include "OctaneElliptical/octaneellipticaltestdata.h"
#include "OctaneTreadmill/octanetreadmilltestdata.h"
#include "PafersBike/pafersbiketestdata.h"
#include "PafersTreadmill/paferstreadmilltestdata.h"
#include "ProFormBike/proformbiketestdata.h"
#include "ProFormElliptical/proformellipticaltestdata.h"
#include "ProFormEllipticalTrainer/proformellipticaltrainertestdata.h"
#include "ProFormRower/proformrowertestdata.h"
#include "ProFormTreadmill/proformtreadmilltestdata.h"
#include "ProFormWiFiBike/proformwifibiketestdata.h"
#include "ProFormWiFiTreadmill/proformwifitreadmilltestdata.h"
#include "RenphoBike/renphobiketestdata.h"
#include "Rower/rowertestdata.h"
#include "SchwinnIC4Bike/schwinnic4biketestdata.h"
#include "Shuaa5Treadmill/shuaa5treadmilltestdata.h"
#include "SkandikaWiryBike/skandikawirybiketestdata.h"
#include "SmartRowRower/smartrowrowertestdata.h"
#include "SnodeBike/snodebiketestdata.h"
#include "SoleBike/solebiketestdata.h"
#include "SoleElliptical/soleellipticaltestdata.h"
#include "SoleF80Treadmill/solef80treadmilltestdata.h"
#include "SpiritTreadmill/spirittreadmilltestdata.h"
#include "SportsPlusBike/sportsplusbiketestdata.h"
#include "SportsTechBike/sportstechbiketestdata.h"
#include "StagesBike/stagesbiketestdata.h"
#include "StrydeRunPowerSensor/stryderunpowersensortestdata.h"
#include "TacxNeo2/tacxneo2testdata.h"
#include "TechnoGymMyRunTreadmill/technogymmyruntreadmilltestdata.h"
#include "TechnogymMyRunTreadmillRFComm/technogymmyruntreadmillrfcommtestdata.h"
#include "ToorxTreadmill/toorxtreadmilltestdata.h"
#include "Treadmill/treadmilltestdata.h"
#include "TrueTreadmill/truetreadmilltestdata.h"
#include "TrxAppGateUSBBike/trxappgateusbbiketestdata.h"
#include "TrxAppGateUSBTreadmill/trxappgateusbtreadmilltestdata.h"
#include "UltrasportBike/ultrasportbiketestdata.h"
#include "WahooKickrSnapBike/wahookickrsnapbiketestdata.h"
#include "YesoulBike/yesoulbiketestdata.h"



class BluetoothDeviceTestDataFactory
{
    static BluetoothDeviceTestDataVector allTestData;

    BluetoothDeviceTestDataFactory();

    static void registerTestData(BluetoothDeviceTestData * testData)
    {
        BluetoothDeviceTestDataFactory::allTestData.push_back(BluetoothDeviceTestData_ptr(testData));
    }

public:
    static BluetoothDeviceTestDataVector get_allTestData() {
        return BluetoothDeviceTestDataVector(allTestData);
    }

    static void registerTestData();

    using BluetoothDeviceTestDataTypes = ::testing::Types<
    ActivioTreadmillTestData,
    BHFitnessEllipticalTestData,
    BowflexT216TreadmillTestData,
    CSCBikeTestData,
    ChronobikeTestData,
    Concept2SkiErgTestData,
    DomyosBikeTestData,
    DomyosEllipticalTestData,
    DomyosRowerTestData,
    DomyosTreadmillTestData,
    ESLinkerTreadmillTestData,
    EchelonConnectSportBikeTestData,
    EchelonRowerTestData,
    EchelonStrideTreadmillTestData,
    FTMSBike1TestData,
    FTMSBike2TestData,
    FTMSRowerTestData,
    FakeBikeTestData,
    FakeEllipticalTestData,
    FakeTreadmillTestData,
    FitPlusBikeFSTestData,
    FitPlusBikeMRKTestData,
    FitshowTreadmillBFTestData,
    FitshowTreadmillFSTestData,
    FitshowTreadmillSWTestData,
    FlywheelBike1TestData,
    FlywheelBike2TestData,
    HorizonGR7BikeTestData,
    HorizonTreadmillTestData,
    InspireBikeTestData,
    KeepBikeTestData,
    KingsmithR1ProTreadmillTestData,
    KingsmithR2TreadmillTestData,
    M3IBikeTestData,
    MCFBikeTestData,
    MepanelBikeTestData,
    NPECableBike1TestData,
    NPECableBike2TestData,
    NautilusBikeTestData,
    NautilusEllipticalTestData,
    NautilusTreadmillTestData,
    NordicTrackEllipticalTestData,
    NordicTrackIFitADBTreadmillTestData,
    OctaneEllipticalTestData,
    OctaneTreadmillTestData,
    PafersBikeTestData,
    PafersTreadmillTestData,
    ProFormBikeTestData,
    ProFormEllipticalTestData,
    ProFormEllipticalTrainerTestData,
    ProFormRowerTestData,
    ProFormTreadmillTestData,
    ProFormWiFiBikeTestData,
    ProFormWiFiTreadmillTestData,
    RenphoBike1TestData,
    RenphoBike2TestData,
    SchwinnIC4BikeTestData,
    Shuaa5TreadmillTestData,
    SkandikaWiryBikeTestData,
    SmartRowRowerTestData,
    SnodeBike1TestData,
    SnodeBike2TestData,
    SoleBikeTestData,
    SoleEllipticalTestData,
    SoleF80TreadmillTestData,
    SpiritTreadmillTestData,
    SportsPlusBikeTestData,
    SportsTechBikeTestData,
    StagesBike1TestData,
    StagesBike2TestData,
    StrydeRunPowerSensorTestData,
    TacxNeo2TestData,
    TechnoGymMyRunTreadmillTestData,
    TechnogymMyRunTreadmillRFCommTestData,
    ToorxTreadmillTestData,
    TrueTreadmillTestData,
    TrxAppGateUSBBike1TestData,
    TrxAppGateUSBBike2TestData,
    TrxAppGateUSBTreadmillTestData,
    UltrasportBikeTestData,
    WahooKickrSnapBikeTestData,
    YesoulBikeTestData,
    iConceptBikeTestData>;
};





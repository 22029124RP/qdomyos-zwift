#import <CoreBluetooth/CoreBluetooth.h>
#import "ios_echelonconnectsport.h"

@implementation ios_echelonconnectsport

- (instancetype)init:(NSString *)deviceName {
    self = [super init];
    if (self) {
        _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
        _targetDeviceName = deviceName;
    }
    return self;
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    if (central.state == CBManagerStatePoweredOn) {
        [self.centralManager scanForPeripheralsWithServices:nil options:nil];
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *,id> *)advertisementData RSSI:(NSNumber *)RSSI {
    if ([peripheral.name isEqualToString:self.targetDeviceName]) {
        self.connectedPeripheral = peripheral;
        [self.centralManager stopScan];
        [self.centralManager connectPeripheral:peripheral options:nil];
    }
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    peripheral.delegate = self;
    [peripheral discoverServices:nil];
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"Peripheral disconnected: %@. Error: %@", peripheral, error);
    if ([peripheral.name isEqualToString:self.targetDeviceName]) {
        NSLog(@"Attempting to reconnect to %@", self.targetDeviceName);
        [self.centralManager connectPeripheral:peripheral options:nil];
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
    for (CBService *service in peripheral.services) {
        [peripheral discoverCharacteristics:nil forService:service];
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (characteristic.value) {
        // Extract the data from characteristic.value and process it as needed
        NSData *receivedData = characteristic.value;
        NSLog(@"UUID: %@ Received data: %@", characteristic.UUID, receivedData);
        // Your processing logic here
        characteristicChanged(characteristic.UUID, receivedData)
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {
    CBUUID *write = [CBUUID UUIDWithString:@"0bf669f2-45f2-11e7-9598-0800200c9a66"];
    CBUUID *uuid1 = [CBUUID UUIDWithString:@"0bf669f3-45f2-11e7-9598-0800200c9a66"];
    CBUUID *uuid2 = [CBUUID UUIDWithString:@"0bf669f4-45f2-11e7-9598-0800200c9a66"];

    for (CBCharacteristic *characteristic in service.characteristics) {
        if ([characteristic.UUID isEqual:uuid1]) {
            self.gattNotify1Characteristic = characteristic;
            [peripheral setNotifyValue:YES forCharacteristic:self.gattNotify1Characteristic];
        } else if ([characteristic.UUID isEqual:uuid2]) {
            self.gattNotify2Characteristic = characteristic;
            [peripheral setNotifyValue:YES forCharacteristic:self.gattNotify2Characteristic];
        } else if ([characteristic.UUID isEqual:write]) {
            self.gattWriteCharacteristic = characteristic;
        }
    }

    // Verifica se entrambe le caratteristiche sono state trovate
    if (self.gattNotify1Characteristic && self.gattNotify2Characteristic && self.gattWriteCharacteristic) {
        stateChanged();
    }
}

- (void)writeCharacteristc:(uint8_t *)data length:(NSUInteger)length {
    if (self.connectedPeripheral.state != CBPeripheralStateConnected) {
        NSLog(@"Cannot send fan speed request: Peripheral is not connected.");
        return;
    }
    
    NSData *dataToSend = [NSData dataWithBytes:data length:length];
    
    if (self.characteristicUUID2) {
        [self.connectedPeripheral writeValue:dataToSend forCharacteristic:self.characteristicUUID2 type:CBCharacteristicWriteWithResponse];
    }
}

@end
//libraries
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>



//define UUIDs

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_X                          "C8F88594-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_Y                          "C8F88123-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_Z                          "C8F88321-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_RESET                      "C8F12345-2217-0CA6-8F06-A4270B675D69"

//set characteristics

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEFloatCharacteristic xCharacteristic( BLE_UUID_X, BLERead | BLENotify );
BLEFloatCharacteristic yCharacteristic( BLE_UUID_Y, BLERead | BLENotify );
BLEFloatCharacteristic zCharacteristic( BLE_UUID_Z, BLERead | BLENotify );
BLEFloatCharacteristic resetCharacteristic( BLE_UUID_RESET, BLERead | BLENotify );

//rotation variable
float x=0.0;
float y=0.0;
float z=0.0;
// fing switch (unused)
float resetState=0.0;
const int buttonPin = 2;

void setup()
{
  BLE.begin();

  // set advertised local name and service
  BLE.setDeviceName( "Arduino Nano 33 BLE" );
  BLE.setLocalName( "Arduino Nano 33 BLE" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( xCharacteristic );
  testService.addCharacteristic( yCharacteristic );
  testService.addCharacteristic( zCharacteristic );
  testService.addCharacteristic( resetCharacteristic );


  // add service
  BLE.addService( testService );

  // set the initial value for characeristics
  xCharacteristic.writeValue( x );
  yCharacteristic.writeValue( y );
  zCharacteristic.writeValue( z );
  resetCharacteristic.writeValue( resetState );

  BLE.advertise();

  // accelrometer check
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //finger switch setup
  pinMode(buttonPin, INPUT);

  
}

void loop()
{
  
  BLEDevice central = BLE.central();

//  if (IMU.accelerationAvailable()) {
//        IMU.readAcceleration(x, y, z);
//  }

// if connected to central, grab acceleromtere data and write to characteristic
  if ( central )
  {
    while ( central.connected() )
    {

      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
      }
      xCharacteristic.writeValue( x );
      yCharacteristic.writeValue( y );
      zCharacteristic.writeValue( z );

      resetState = digitalRead(buttonPin);
      resetCharacteristic.writeValue( resetState );
      
        }
      }

  
}

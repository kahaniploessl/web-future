#include <ArduinoBLE.h>

//define UUIDs

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_X                          "C8F88594-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_Y                          "C8F88123-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_Z                          "C8F88321-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_RESET                      "C8F12345-2217-0CA6-8F06-A4270B675D69"

//rotation variables
float x,y,z;

// finger switch button (unused)
float resetState;

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

  BLE.begin();

  BLE.scanForUuid( BLE_UUID_TEST_SERVICE );
}


void loop()
{

  //scan for peripheral
  BLEDevice peripheral = BLE.available();

  if ( peripheral )
  {
    if ( peripheral.localName() != "Arduino Nano 33 BLE" )
    {
      return;
    }

    BLE.stopScan();

    connectPeripheral( peripheral );

    BLE.scanForUuid( BLE_UUID_TEST_SERVICE );
  }
}


bool connectPeripheral( BLEDevice peripheral )
{
  
  //if connected checks
  if ( !peripheral.connect() )
  {
    return false;
  }

  if ( !peripheral.discoverAttributes() )
  {
    peripheral.disconnect();
    return false;
  }

  //set connection to peripheral characteristics
  BLECharacteristic xCharacteristic = peripheral.characteristic( BLE_UUID_X );
  BLECharacteristic yCharacteristic = peripheral.characteristic( BLE_UUID_Y );
  BLECharacteristic zCharacteristic = peripheral.characteristic( BLE_UUID_Z );
  BLECharacteristic resetCharacteristic = peripheral.characteristic( BLE_UUID_RESET );

  //xchecks
  if ( !xCharacteristic)
  {
    peripheral.disconnect();
    return false;
  }
  if ( !xCharacteristic.canSubscribe() )
  {
    peripheral.disconnect();
    return false;
  }
  if ( !xCharacteristic.subscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  //ychecks

  if ( !yCharacteristic)
  {
    peripheral.disconnect();
    return false;
  }
  if ( !yCharacteristic.canSubscribe() )
  {
    peripheral.disconnect();
    return false;
  }
  if ( !yCharacteristic.subscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  //zchecks
  
  if ( !zCharacteristic)
  {
    peripheral.disconnect();
    return false;
  }
  if ( !zCharacteristic.canSubscribe() )
  {
    peripheral.disconnect();
    return false;
  }
  if ( !zCharacteristic.subscribe() )
  {
    peripheral.disconnect();
    return false;
  }

    // reset checks
  
  if ( !resetCharacteristic)
  {
    peripheral.disconnect();
    return false;
  }
  if ( !resetCharacteristic.canSubscribe() )
  {
    peripheral.disconnect();
    return false;
  }
  if ( !resetCharacteristic.subscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  //while connected update x,y,z values
  while ( peripheral.connected() )
  {
    if ( xCharacteristic.valueUpdated() )
    {
      xCharacteristic.readValue( &x, 4 );
    }

    if ( yCharacteristic.valueUpdated() )
    {
      yCharacteristic.readValue( &y, 4 );
    }
    
    if ( zCharacteristic.valueUpdated() )
    {
      zCharacteristic.readValue( &z, 4 );
    }

    if ( resetCharacteristic.valueUpdated() )
    {
      resetCharacteristic.readValue( &resetState, 4 );
    }

//      Serial.print("Reset:");
//      Serial.print(resetState);

//serial print in order for touchdesigner to grab variables

      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(",");
      Serial.println(z);

      delay(50);



  }

  peripheral.disconnect();
  return true;
}

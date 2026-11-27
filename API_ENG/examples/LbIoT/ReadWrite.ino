
#include "Leanbot.h"
#include "LeanbotIoT.h"


// using F() macro to lower Arduino SRAM usage
#define  WIFI_SSID      F( "Nam-Laptop" )
#define  WIFI_PASSWORD  F( "21345678" )

#define  IOT_ASSET_ID   F( "33khwmcNaidrPJ2oZpcgde" )
#define  IOT_REALM      F( "master" )
//  Asset      https://iot-qa.pythaverse.space/manager/#/assets/false/33khwmcNaidrPJ2oZpcgde
//  Dashboard  https://iot-qa.pythaverse.space/manager/#/insight/false/???


auto iotSoilTension = LbIoT.Asset.attributeInt(F( "soilTensionMeasured" ));

long MY_TIME_ZONE   = 7*SECS_PER_HOUR + 0*SECS_PER_MIN;  // GMT+7:00
auto iotDateTime    = LbIoT.Asset.attributeTime(F( "time" ));


void setup() {
  Leanbot.begin();
}


void loop() {
  if ( LbIoT.Wifi.status() != WL_CONNECTED ) {
    if ( LbIoT.Wifi.begin( WIFI_SSID, WIFI_PASSWORD ) < 0 ) {
      return;  // retry
    }
  }


  if ( LbIoT.Asset.status() != ESTABLISHED ) {
    if ( LbIoT.Asset.begin( IOT_ASSET_ID, IOT_REALM ) < 0 ) {
      return;  // retry
    }

    // print Asset info
    Serial.print( "Asset: " );
    LbIoT.Asset.printNotes();
  }


  // read sensor value
  int sensorValue = analogRead(A0);
  Serial.print( "\n\n\n" "######   Send the \"sensorValue = " );
  Serial.print(sensorValue);
  Serial.print( "\" to server   ######" "\n\n" );
  iotSoilTension.write( sensorValue );
  delay(2500);                           // small delay to wait for response from server
  printDecodedResponseStatus();


  Serial.print( "\n\n\n" "######   Read the value from server   ######" "\n\n" );
  int value = iotSoilTension.read();
  printDecodedResponseStatus();
  Serial.print( iotSoilTension.name() );
  Serial.print(" = ");
  Serial.println(value);
  delay(2500);


  Serial.print( "\n\n\n" "######   Read server and local time   ######" "\n\n" );
  time_t serverTime = iotDateTime.read() + MY_TIME_ZONE;
  printDecodedResponseStatus();
  Serial.print( "Server time: " );
  printTime(serverTime);

  time_t localTime = WiFi.getTime() + MY_TIME_ZONE;
  Serial.print( "Local time:  " );
  printTime(localTime);
  delay(2500);
}


// yyyy-mm-dd hh:mm:ss
void printTime(time_t t) {
  Serial.print( year(t) );
  Serial.print("-");
  print2Digit( month(t) );
  Serial.print("-");
  print2Digit( day(t) );

  Serial.print(" ");

  print2Digit( hour(t) );
  Serial.print(":");
  print2Digit( minute(t) );
  Serial.print(":");
  print2Digit( second(t) );
  Serial.print("\n");
}


void print2Digit(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}


/*
  https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
  - Informational responses (100 – 199)
  - Successful responses    (200 – 299)
  - Redirection messages    (300 – 399)
  - Client error responses  (400 – 499)
  - Server error responses  (500 – 599)

  Common status:
  - HTTP/1.1 200 OK       : The request succeeded
  - HTTP/1.1 403 Forbidden: Incorrect "attributeId" or "Access public write" is not configured
*/
void printDecodedResponseStatus() {
  LbIoT.Asset.printDecodedResponseStatus();
}

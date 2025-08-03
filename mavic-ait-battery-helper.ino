#include <Arduino.h>
#include <Wire.h>
#include "bqcmd.h"
// Mavic air battery adress
#define BQ_ADDR 0x0B
// Set to true if you want to apply pacth, else it will just print battery data
#define UNLOCK_ACTIVETED false

void setup() {
  Serial.begin(9600);      // Start serial communication for debug output
  Wire.begin();            // Initialize I2C bus
  delay(1000);             // Wait for bus and device to stabilize
  Serial.println();

  if(!UNLOCK_ACTIVETED) {
    Serial.println(F("⚠️ Battery unlock desactived by default (to let you test the connexion & read values first) ⚠️"));
    Serial.println(F("⚠️ If want to unlock battery, change parameter UNLOCK_ACTIVETED to true ⚠️"));
    Serial.println();
  }

  Serial.println(F("Starting communication with battery..."));
  Serial.println(F("⚠️ If you receive NACK : ⚠️"));
  Serial.println(F("  - This code is specific to Mavic Air 1 + Arduino MEGA 2560, some parameters may change if you have another configuration"));
  Serial.println(F("  - Your battery is maybe not well connected to arduino"));
  Serial.println(F("      - Verify connexion, you can find screenshot of how to connect in github repo. https://github.com/gvnt/mavic-air-battery-helper"));
  Serial.println(F("  - Your battery is maybe completely discharge and cannot communicate, you need to open it and charge if a bit manually"));
  Serial.println();

  Serial.println(F("Testing to print FirmwareVersion (Should look like 0x02 0x00 0x43 0x07 0x01 0x01 0x00 0x27 0x00 0x03 0x85 0x02 0x00)"));
  runMBACommand(BQ_ADDR, "FirmwareVersion");

  Serial.println(F("Printing battery state ..."));
  runMBACommand(BQ_ADDR, "OperationStatus");
  runMBACommand(BQ_ADDR, "SafetyAlert");
  runMBACommand(BQ_ADDR, "SafetyStatus");
  runMBACommand(BQ_ADDR, "PFAlert");
  runMBACommand(BQ_ADDR, "PFStatus");
  runMBACommand(BQ_ADDR, "ManufacturingStatus");

  if(UNLOCK_ACTIVETED) {
    Serial.println(F("Unlocking battery..."));
    runMBACommand(BQ_ADDR, "UnsealKey1");
    runMBACommand(BQ_ADDR, "UnsealKey2");

    Serial.println(F("Temporary disabling PermanentFailure ..."));
    runMBACommand(BQ_ADDR, "PermanentFailure");
    runMBACommand(BQ_ADDR, "ManufacturingStatus");

    Serial.println(F("Reseting PermanentFailure data ..."));
    runMBACommand(BQ_ADDR, "PermanentFailureDataReset");

    Serial.println(F("Printing battery state ..."));
    runMBACommand(BQ_ADDR, "OperationStatus");

    Serial.println(F("Printing register custom DJI PermanentFailure ..."));
    runMBACommand(BQ_ADDR, "PF2RegisterRead");

    Serial.println(F("Clearing custom DJI PermanentFailure ..."));
    runMBACommand(BQ_ADDR, "ClearPF2");

    Serial.println(F("Printing register custom DJI PermanentFailure ..."));
    runMBACommand(BQ_ADDR, "PF2RegisterRead");

    Serial.println(F("Reactivating PermanentFailure mode ..."));
    runMBACommand(BQ_ADDR, "PermanentFailure");

    Serial.println(F("Waiting for device reset (wait some seconds) ..."));
    runMBACommand(BQ_ADDR, "DeviceReset");
    delay(10000);

    Serial.println(F("Printing final battery state ..."));
    runMBACommand(BQ_ADDR, "OperationStatus");
    runMBACommand(BQ_ADDR, "SafetyAlert");
    runMBACommand(BQ_ADDR, "SafetyStatus");
    runMBACommand(BQ_ADDR, "PFAlert");
    runMBACommand(BQ_ADDR, "PFStatus");
    runMBACommand(BQ_ADDR, "ManufacturingStatus");

    Serial.println(F("You can disconnect and test your battery now."));
  } 
}

void loop() {}

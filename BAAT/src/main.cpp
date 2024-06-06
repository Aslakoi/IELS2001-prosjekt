#include <inits.h>
#include <servos.h>
#include <blutoot.h>
#include <motor.h>
//  #include <ubidots.h>

#define RXD2 16
#define TXD2 17

void setup()
{
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //serial 2 for kommunikasjon til den andre espen
  motorSetup();
  servoSetup();
  setupBLE();
  //ubiSetup();  
}

void sendData(uint8_t servoFoilValL, uint8_t maxMotorDuty) {
  String message = "foils:" + String(servoFoilValL) + ";motor:" + String(maxMotorDuty);
  //Serial.println(message);
  Serial2.println(message);
  
}

void loop() {
  unsigned long currentTime = millis();

  // BLE communication and control commands
  if (currentTime - previousTime >= 40)
  {
    if (deviceConnected) 
    {
      if (!rxString.isEmpty()) 
      {
        convertControlpad();  // Process the command received via BLE
        rxString = "";  // Clear the received string to prepare for new data
      }
      controlFoil(direction);
      moveFoilsToNeutral();
      controlRudder(direction);
      moveToNeutral();
      motorControll();
      sendData(servoFoilValL, maxMotorDuty);
    }
    previousTime = currentTime;  // Reset timing for the next command cycle
  }
}


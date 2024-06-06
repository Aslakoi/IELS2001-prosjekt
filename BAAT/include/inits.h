#include <Arduino.h>
#include <ESP32Servo.h>
#include <BLEDevice.h>
//#include <BLEServer.h>
//#include <BLEUtils.h>
#include <BLE2902.h>

//BLE variabler
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool convert = false;
String rxString = "";
std::string rxValue; // rxValue gathers input data
long previousTime = 0;

// UART service UUID data
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//motor driver
#define PWM 32
#define AI2 25
#define AI1 33

uint8_t maxMotorDuty = 255;
uint8_t reverseDuty = 80;
const uint8_t motorChannel = 10;
uint8_t motorMode = 0;

Servo servoFoilR;
Servo servoFoilL;
Servo servoRudder; 

//foil variabler
const uint8_t servoFoilPinR = 26;
const uint8_t servoFoilPinL = 27;
uint8_t servoFoilValR = 11;
uint8_t servoFoilValL = 16;
bool foilFlag = false;
const uint8_t foilStepTime = 20;
bool neutralFoils = false;

//ror variabler
const uint8_t servoRudderPin = 14;
uint8_t servoRudderVal = 150;
uint8_t rudderStepTime = 15;
unsigned long lastButtonPressTime = 0;
bool rudderFlag = false;
bool neutralRudder = false;
String direction = "";


///////////
//Ubidots
//////////

//#include <UbidotsEsp32MqttLibs.h>
//#include <PubSubClientLib.h>

/****************************************
 * Define Constants
 ****************************************/
/*
const char *UBIDOTS_TOKEN = "BBUS-bYrshUmnZmwNwmCE0f0XeukiD59GRb"; 
const char *WIFI_SSID = "Aslak";
const char *WIFI_PASS = "Tallin36";
const char *DEVICE_LABEL = "test";
const char *VARIABLE_LABEL = "noe";
const char *VARIABLE_LABEL2 = "noe-annet";
const char *VARIABLE_LABEL3 = "servo";

const uint8_t LED = 32; // Pin used to write data based on 1's and 0's coming from Ubidots
const uint8_t analogPin = 24;
const uint8_t servoPin = 25;

const uint16_t PUBLISH_FREQUENCY = 2000;
unsigned long timer = 0;

Ubidots ubidots(UBIDOTS_TOKEN);*/
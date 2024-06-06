#include <UbidotsEsp32MqttLibs.h>
#include <distance.h>
#include <MPU-6050.h>

#define RXD2 16
#define TXD2 17

String incoming;

//deklarer innhentet data på den andre ESP'en
uint8_t maxMotorDuty;  // label - maxmotorduty
uint8_t foil_output; // label -foils

const char *UBIDOTS_TOKEN = "BBUS-bYrshUmnZmwNwmCE0f0XeukiD59GRb";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Aslak";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "Tallin36";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "baat2";   // Put here your Device label to which data will be published
const char *VARIABLE_LABEL = "maxmotorduty"; // Put here your Variable label to which data will be published
const char *VARIABLE_LABEL2 = "foil";
const char *VARIABLE_LABEL3 = "pitch";
const char *VARIABLE_LABEL4 = "acceleration";

const int PUBLISH_FREQUENCY = 1000; // Update rate in milliseconds

unsigned long timer;

Ubidots ubidots(UBIDOTS_TOKEN);

void receivedData() {
  if (Serial2.available()) {
    incoming = Serial2.readStringUntil('\n'); // Read until newline character
    //Serial.println(incoming);

    if (incoming.indexOf("foils:") != -1 && incoming.indexOf("motor:") != -1) {
      int startIndex = incoming.indexOf("foils:") + 6;
      int endIndex = incoming.indexOf(";", startIndex);
      foil_output = incoming.substring(startIndex, endIndex).toInt();

      startIndex = incoming.indexOf("motor:") + 6;
      endIndex = incoming.indexOf(";", startIndex);
      maxMotorDuty = incoming.substring(startIndex, endIndex).toInt();  // Use global maxMotorDuty

      /*Serial.print("Foils: ");
      Serial.println(foil_output);
      Serial.print("Max Motor Duty: ");
      Serial.println(maxMotorDuty);*/

      // Add your code to handle foil_output and maxMotorDuty here
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void ubiSetup() {
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Started the receiver");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  //ultrasonicSetup();
  MPUsetup();
  ubiSetup();
}


//roll = pitch fordi MPU er satt opp rotert 90 grader, altså x=y
void loop() {
  receivedData();
  //ultrasonic();
  calculateAccPitchRoll();

  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    ubidots.add(VARIABLE_LABEL, maxMotorDuty); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, foil_output);
    ubidots.add(VARIABLE_LABEL3, angle_roll_output);
    ubidots.add(VARIABLE_LABEL4, acc_x_ms2_output);
    ubidots.publish(DEVICE_LABEL);
    //Serial.println(maxMotorDuty);
    //Serial.println(foil_output);
    //Serial.println(angle_pitch_output);
    Serial.println(angle_roll_output);
    Serial.println(acc_x_ms2_output);
    timer = millis();
  }
  ubidots.loop();
}

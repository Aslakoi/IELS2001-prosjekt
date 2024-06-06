//onConnect og onDisconnect er a allerede funksjoner i BLE biblioteket, denne derived classen overskriver disse 
//for min bruk. ESP32 firmware vet om klient connecter eller disconnecter
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("client connected");
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("client disconnected, restarting advertising");
    BLEDevice::startAdvertising();
  }
};

//forstå disse, ta objektorientert for faen
class MyCallbacks: public BLECharacteristicCallbacks { 
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      Serial.println(" ");
      Serial.print("Received data: ");
      for (int i = 0; i < rxValue.length(); i++) { 
        Serial.print(rxValue[i]);
        rxString += rxValue[i]; // build string from received data 
      } 
      Serial.println();
      rxString.trim();  //fjern newlines og sånt
    } 
  } 
}; 

void setupBLE()
{
  Serial.begin(115200);

  BLEDevice::init("Aslak ESP32 UART"); // give the BLE device a name
  
  BLEServer *pServer = BLEDevice::createServer(); // create BLE server
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic for TX
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic( 
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start(); // start the service
  pServer->getAdvertising()->start(); // start advertising
  Serial.println("Waiting a client connection to notify...");
  Serial.println(" ");
}

void convertControlpad() {
  convert = false;
  Serial.print("      ");
  if (rxString == "!B11:") {
    Serial.println("********** Start Action 1");
    maxMotorDuty += 25;
    if (maxMotorDuty >= 255) {
      maxMotorDuty = 255;
    }
  }
  else if (rxString == "!B219") 
  {
    Serial.println("********** Start Action 2");
    motorMode = 1;
  }
  else if (rxString == "!B318") {
    Serial.println("********** Start Action 3");
    maxMotorDuty -= 25;
    if (maxMotorDuty <= 100) {
      maxMotorDuty = 100;
    }
  }
  else if (rxString == "!B417") {
    Serial.println("********** Start Action 4");
    motorMode = 2;
  }
  else if (rxString == "!B516") {
    Serial.println("********** Start Action UP");
    foilFlag = true;
    direction = "UP";
  }
  else if (rxString == "!B615") {
    Serial.println("********** Start Action DOWN");
    foilFlag = true;
    direction = "DOWN";
  }
  else if (rxString == "!B714") {
    Serial.println("********** Start Action LEFT");
    rudderFlag = true;
    direction = "LEFT";
  }
  else if (rxString == "!B813") {
    Serial.println("********** Start Action RIGHT");
    rudderFlag = true;
    direction = "RIGHT";
  }
  else if (rxString == "!B10;") {
    Serial.println("********** Stop Action 1");
  }
  else if (rxString == "!B20:") {
    Serial.println("********** Stop Action 2");
    motorMode = 0;
  }
  else if (rxString == "!B309") {
    Serial.println("********** Stop Action 3");
  }
  else if (rxString == "!B408") {
    Serial.println("********** Stop Action 4");
    motorMode = 0;
  }
  else if (rxString == "!B507") {
    Serial.println("********** Stop Action UP, servoFoilVal is: ");
    foilFlag = false;
    Serial.println(servoFoilValR);
    Serial.println(servoFoilValL);
  }
  else if (rxString == "!B606") {
    Serial.println("********** Stop Action DOWN, servoFoilVal is: ");
    foilFlag = false;
    Serial.println(servoFoilValR);
    Serial.println(servoFoilValL);
  }
  else if (rxString == "!B705") {
    Serial.println("********** Stop Action LEFT, servoRudderVal is: ");
    rudderFlag = false;
    neutralRudder = true;
    Serial2.print(servoRudderVal);
  }
  else if (rxString == "!B804") {
    Serial.println("********** Stop Action RIGHT, servoRudderVal is: ");  
    rudderFlag = false;
    neutralRudder = true;
    Serial2.print(servoRudderVal);

  }
  else if (rxString == "rudderVal") {
    std::string rudderVal = std::to_string(servoRudderVal);
    pCharacteristic->setValue(rudderVal);
    pCharacteristic->notify();
  }
  else if (rxString == "foilValL") {
    std::string foilVal = std::to_string(servoFoilValL);
    pCharacteristic->setValue(foilVal);
    pCharacteristic->notify();
  }
  else if (rxString == "foilValR") {
    std::string foilVal = std::to_string(servoFoilValL);
    pCharacteristic->setValue(foilVal);
    pCharacteristic->notify();
  }
  else if (rxString == "motorDuty") {
    std::string motorDuty = std::to_string(maxMotorDuty);
    pCharacteristic->setValue(motorDuty);
    pCharacteristic->notify();
  }
  else if (rxString == "neutralFoils") {
    Serial.println("hallaaaaaaaa");
    neutralFoils = true;
  }
  rxString = "";
}

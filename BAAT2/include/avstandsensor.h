const int trigPin = 32; // Definerer trigger pin som nummer 9
const int echoPin = 33; // Defines echo pin som nummer 10

int distance; //https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

unsigned long previousTime = 0;

void ultrasonicSetup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void ultrasonic() 
{
  unsigned long currentTime = millis();
  uint8_t interval = 500;

  if (currentTime - previousTime >= interval) {
    digitalWrite(trigPin, LOW); // Resetter trigpin
    delayMicroseconds(2); // mikrosekund går så raskt at det ikke blokkerer annen kode

    digitalWrite(trigPin, HIGH); // sender en 10 mikrosekund lang puls
    delayMicroseconds(10); // 
    digitalWrite(trigPin, LOW); // 

    int duration = pulseIn(echoPin, HIGH); // registrerer når echo pin fanger opp pulsen og finner ut hvor lang tid det tok

    distance = duration * 0.034 / 2; // regner ut avstand med lydens hastighet fram og tilbake
    }
}
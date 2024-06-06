
uint8_t upperBoundR = 21;
uint8_t upperBoundL = 26;
uint8_t lowerBoundR = 1;
uint8_t lowerBoundL = 6;

void servoSetup()
{
//kalibrer disse
  servoFoilR.attach(servoFoilPinR, 0, 5000);
  servoFoilL.attach(servoFoilPinL, 0, 5000);
  servoRudder.attach (servoRudderPin, 0, 6000);
  
  //stiller inn servoer til midten
  servoFoilR.write(servoFoilValR);
  servoFoilL.write(servoFoilValL);
  servoRudder.write(servoRudderVal);
}

void controlRudder(String direction)
{
  if (!rudderFlag) return;
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPressTime >= rudderStepTime) 
  {
    if (direction == "LEFT") 
    {
      servoRudderVal +=5;
      if (servoRudderVal >= 180) {
        servoRudderVal = 180;
      }
    } else if (direction == "RIGHT") 
    {
      servoRudderVal -=8;
      if (servoRudderVal <= 110) {
        servoRudderVal = 110;
      }
    }
    servoRudder.write(servoRudderVal);
    lastButtonPressTime = currentTime; // Update last adjustment time for incremental adjustments
  }
}

void moveToNeutral()
{
  if (!neutralRudder) return;
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPressTime >= rudderStepTime) 
  {
    if (servoRudderVal < 150) {
      servoRudderVal += 5;
      if (servoRudderVal > 150) {
        servoRudderVal = 150;
      }
    } 
    else if (servoRudderVal > 150) {
      servoRudderVal -= 5;
      if (servoRudderVal < 150) {
        servoRudderVal = 150;
      }
    }
    servoRudder.write(servoRudderVal);
    lastButtonPressTime = currentTime; // Update last adjustment time
  }
}

void controlFoil(String direction) {

  if (!foilFlag) return;
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPressTime >= foilStepTime) 
  {
    if (direction == "DOWN") {
      // Decrease R, Increase L
      servoFoilValR -= 1;
      servoFoilValL += 1;
    } else if (direction == "UP") {
      // Increase R, Decrease L
      servoFoilValR += 1;
      servoFoilValL -= 1;
    }
    
    // Ensure servoFoilValR stays within 5 to 30
    if (servoFoilValR < lowerBoundR) {
      servoFoilValR = lowerBoundR;
    } else if (servoFoilValR > upperBoundR) {
      servoFoilValR = upperBoundR;
    }
    
    // Ensure servoFoilValL stays within 5 to 30
    if (servoFoilValL < lowerBoundL) {
      servoFoilValL = lowerBoundL;
    } else if (servoFoilValL > upperBoundL) {
      servoFoilValL = upperBoundL;
    }
        // Write the new values to the servos
    servoFoilR.write(servoFoilValR);
    servoFoilL.write(servoFoilValL);

    lastButtonPressTime = currentTime; // Update last adjustment time
  }
}

void moveFoilsToNeutral() {
  if (!neutralFoils) return;
  
  servoFoilL.write(16);
  servoFoilR.write(11);
}


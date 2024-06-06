void motorSetup()
{
  pinMode(AI1, OUTPUT);
  pinMode(AI2, OUTPUT);
  ledcSetup(motorChannel, 500, 8);
  ledcAttachPin(PWM, motorChannel);
}

void motorControll()
{
  //fremover
  if (motorMode == 1)
  {
    digitalWrite(AI1, HIGH);
    digitalWrite(AI2, LOW);
    ledcWrite(motorChannel, maxMotorDuty);
  }
  //bakover
  else if (motorMode == 2) {
    digitalWrite(AI1, LOW);
    digitalWrite(AI2, HIGH);
    ledcWrite(motorChannel, reverseDuty);
  }
  else {
    digitalWrite(AI1, LOW);
    digitalWrite(AI2, LOW);
    ledcWrite(motorChannel, 0);
  }
}
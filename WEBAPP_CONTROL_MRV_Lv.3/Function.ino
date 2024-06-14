void rotateMotor(int motorNumber, int motorDirection) {
  if (motorDirection == FORWARD) {
    analogWrite(L_ENA, 200);
    analogWrite(L_ENB, 200);
    analogWrite(R_ENA, 200);
    analogWrite(R_ENB, 200);
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
  } else if (motorDirection == BACKWARD) {
    analogWrite(L_ENA, 200);
    analogWrite(L_ENB, 200);
    analogWrite(R_ENA, 200);
    analogWrite(R_ENB, 200);
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  } else {
    analogWrite(L_ENA, 200);
    analogWrite(L_ENB, 200);
    analogWrite(R_ENA, 200);
    analogWrite(R_ENB, 200);
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
}

//------------------------------------------------------------------AGV Control Function -----------------------------------------------------------------------------//
void processAGVMovement(String inputValue) {
  //Serial.printf("Got value as %s %d\n", inputValue.c_str(), inputValue.toInt());
  switch (inputValue.toInt()) {
    case UP:
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      break;
    case DOWN:
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      break;
    case UP_LEFT:
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      break;
    case UP_RIGHT:
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      break;
    case DOWN_LEFT:
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      break;
    case DOWN_RIGHT:
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      break;
    case TURN_LEFT:
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      break;
    case TURN_RIGHT:
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      break;
    case ON_BUZZER:
      beep(1);
      break;
    case OFF_BUZZER:
      beep(0);
      break;
    case STOP:
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      break;
    default:
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      break;
  }
}
// ------------------------------------------------------------------------------
void beep(bool x) {
  if (x == 1) {
    digitalWrite(18, HIGH);
    lcd_mode = 1;
  } else {
    digitalWrite(18, LOW);
    lcd_mode = 0;
  }
}
// -------------------------------------------------------------------------------
void sensor_fn() {

////// รับค่า  Sensor DHT
  humi = dht.readHumidity();     //รับค่าความชื้น
  temp = dht.readTemperature();  //รับค่าอุณหภูมิ
  ////// รับค่า Sensor แสง
  LDR_input = analogRead(LDR_Pin);
  ldr = map(LDR_input, 0, 4095, 0, 500);

  if (temp >= temp_alarm) {
    alarm_beep();
  }
  if (ldr >= light_alarm) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
  
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  Ultra = microsecondsToCentimeters(duration);
  // Serial.print("Distance:");
  // Serial.println(Ultra);
}
// --------------------------------------------------------------------------
void lcd_fn() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= lcd_refresh) {
    previousMillis1 = currentMillis;
    lcd.clear();

    ////// แสดงค่า  Sensor DHT
    if (lcd_mode == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Temp : " + String(temp, 1) + " C");  //แสดงค่าอุณหภูมิ
      lcd.setCursor(0, 1);
      lcd.print("Humidity: " + String(humi, 1) + " %");  //แสดงค่าความซื้น
    } else {
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print(LCDtext0);
      lcd.setCursor(0, 1);
      lcd.print(LCDtext1);
    }
  }
}
// --------------------------------------------------------------------------
void alarm_beep() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= 500) {
    previousMillis2 = currentMillis;
    if (alarm_state == 0) {
      alarm_state = 1;
    } else {
      alarm_state = 0;
    }
    digitalWrite(Buzzer_Pin, alarm_state);
  }
}
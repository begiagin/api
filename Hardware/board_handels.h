

void sensor_data()
{
  int a = analogRead(A0);
  int temp = a / 4.35;
  temp = millis() / 100;
  String sensor_value = String(temp);
  server.send(200, "text/plain", sensor_value);
}

void Motor_control() {
  String act_state = server.arg("state");

  isUP = act_state[0] == '1';
  isDOWN = act_state[1] == '1';

  //  digitalWrite(NPN1, isUP/* && !digitalRead(ProxyUp)*/);
  //  digitalWrite(NPN2, isDOWN/* && !digitalRead(ProxyDown)*/);


  act_state = isUP;
  act_state += isDOWN;


  server.send(200, "text/plane", act_state);
  Serial.print("act_state=");
  Serial.println(act_state);
}

void Test12v() {
  String RmtNoCheck;
  char Rmt[6];
  RmtNoCheck = server.arg("RmtNoCheck");

  for (byte i = 0; i < RmtNoCheck.length(); i++) {
    Rmt[i] = RmtNoCheck[i];
  }

  set_comm(Rmt);

  if (SPI_Array[10] | SPI_Array[11]) {
    digitalWrite(PNP1, HIGH);
    Serial.println("12v turned OFF. (set ProgGroup=0 to turn it ON)");
  }
  else {
    digitalWrite(PNP1, LOW);
    Serial.println("+++12v is switched ON.+++");
  }


}

void RemoteNoFUN() {
  String RmtNoValue;
  char Rmt[6];
  RmtNoValue = server.arg("RmtNoValue");

  for (byte i = 0; i < RmtNoValue.length(); i++) {
    Rmt[i] = RmtNoValue[i];
  }
  //Serial.println(RmtNoValue);
  set_comm(Rmt);
  digitalWrite(PNP1, HIGH);
  Serial.println("---12v is OFF---.");
}

void set_comm(char Rmt[6]) {

  long k = atoi(Rmt);
  Remote1_20 = k / 0x10000;
  k &= 0xffff;
  Key1_4 = k / 0x100;
  ProgGroup1_10 = k & 255;

  for (uint8_t i = 0; i < sizeof(SPI_Array); i++)
    SPI_Array[i] = 0;

  uint8_t rk, rm;
  rm = (Remote1_20 - 1) % 2;
  rm *= 8;
  rm += Key1_4;
  switch (rm) {
    case 1:
      rk = 2;
      break;
    case 2:
      rk = 128;
      break;
    case 3:
      rk = 4;
      break;
    case 4:
      rk = 8;
      break;
    case 9:
      rk = 64;
      break;
    case 10:
      rk = 1;
      break;
    case 11:
      rk = 32;
      break;
    case 12:
      rk = 16;
      break;
  }
  //  if ((Remote1_20 % 2) == 1) rk *= 16;
  rm = (Remote1_20 - 1) / 2;
  SPI_Array[rm] = rk;

  switch (ProgGroup1_10) {
    case 1:
      SPI_Array[11] = 16;
      break;
    case 2:
      SPI_Array[11] = 32;
      break;
    case 3:
      SPI_Array[11] = 64;
      break;
    case 4:
      SPI_Array[11] = 128;
      break;
    case 5:
      SPI_Array[11] = 8;
      break;
    case 6:
      SPI_Array[10] = 1;
      break;
    case 7:
      SPI_Array[10] = 2;
      break;
    case 8:
      SPI_Array[10] = 4;
      break;
    case 9:
      SPI_Array[10] = 8;
      break;
    case 10:
      SPI_Array[10] = 128;
      break;
  }
  SPIx80(ProgGroup1_10 > 0);

  Serial.print("\r\n\r\nRemote1_20=");
  Serial.println(Remote1_20);
  Serial.print("Key1_4=");
  Serial.println(Key1_4);
  Serial.print("ProgGroup1_10=");
  Serial.println(ProgGroup1_10);
}

void checkFUN() {
  if (checkDOWN) {
    server.send(200, "text/plane", "down");
    checkDOWN = false;
  }
  if (checkUP) {
    server.send(200, "text/plane", "up");
    checkUP = false;
  }
}

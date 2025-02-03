float y = 0, e, u, Kp, Ki, Kd, P, I = 0, D, eprev = 0, dt;
volatile bool A;
volatile bool B;
volatile float counter = 0;
// unsigned long lastPrintTime = 0;  // For controlling the print rate

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), ISRA, RISING);
  pinMode(2, INPUT);  // enc A
  pinMode(5, INPUT);  // enc B
  pinMode(6, OUTPUT); // motor PWM
  pinMode(7, OUTPUT); // motor rotA
  pinMode(8, OUTPUT); // motor rotB
}

void loop() {
  // Print the encoder count every 500 ms
  // if (millis() - lastPrintTime >= 500) {
  //   Serial.print("Counter: ");
  //   Serial.println(counter);
  //   Serial.print("e: ");
  //   Serial.println(e);
  //   lastPrintTime = millis();
  // }

  // Input scaling 0.36
  e = -360.0 / 1000.0 * counter;

  // PID constants:
  Kp = 7;
  Ki = 3;
  Kd = 0.3;
  dt = 10.0;

  // PID values
  P = Kp * e;
  I += Ki * e * dt * 0.001;
  D = Kd * (e - eprev) / (dt * 0.001);

  // I component saturation
  if (I > 80) {
    I = 80;
  }
  if (I < -80) {
    I = -80;
  }

  // Control signal
  float u = P + I + D;

  // Control signal saturation
  if (u > 255) {
    u = 255;
  }
  if (u < -255) {
    u = -255;
  }

  // Output to motor:
  if (u < 0) {
    analogWrite(6, -u);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
  }
  if (u >= 0) {
    analogWrite(6, u);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
  }

  eprev = e;  // Error time diff

  delay(dt);
}


void ISRA() {
  if (!digitalRead(5)) {
    counter++;
  } else {
    counter--;
  }
}

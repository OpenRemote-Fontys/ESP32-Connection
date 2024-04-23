const int max9814OutputPin = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int startTime = micros();
  int readings[100000];

  for (int i = 0; i < 1000000; i++) {
    readings[i] = analogRead(max9814OutputPin);
  }

  int computeTime = micros() - startTime;

  float timePer = computeTime / 1000000.0;
  Serial.println(timePer);
  float Hz = 1 / (timePer / 10000000);
  Serial.println(Hz);

  unsigned long sum = 0;
  for (int i = 0; i < 100000; i ++ ) {
    sum = sum + readings[i];
  }
  float average = sum / 100000;
  Serial.println(average);

  

  delay(1000000);
}


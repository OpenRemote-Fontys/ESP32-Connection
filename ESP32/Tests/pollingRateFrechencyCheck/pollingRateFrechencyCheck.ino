#define audioPin 26

void setup() {
  Serial.begin(115200);

}

void loop() {
  int millisecondsStart =  millis();
  int reading;

  for (int i = 0; i < 100000 ;i++) {
    reading = analogRead(audioPin);
  }
  int time = millis() - millisecondsStart;

  Serial.println(time);

  float timePer = (float(time) / 100000) / 1000;
  float Hz = 1 / timePer;
  Serial.println(Hz);

  delay(10000);

}

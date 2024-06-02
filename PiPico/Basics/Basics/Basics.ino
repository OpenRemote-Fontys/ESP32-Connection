#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  WiFi.begin("iotroam", "hndehi24WX");

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
}

void loop() {
  // put your main code here, to run repeatedly:

}

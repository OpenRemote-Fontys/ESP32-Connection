#include "secret.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <stdlib.h>

#define AudioPin 34

//Objects
WiFiClientSecure askClient;  //SSL Client
//WiFiClient askClient; //Non-SSL Client, also remove the comments for askClient.setCACert(local_root_ca);

PubSubClient client(askClient);

void setup() {
  Serial.begin(115200);
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());
  askClient.setCACert(local_root_ca);  //If you use non SSL then comment out
  Serial.println(client.setServer(mqtt_server, mqtt_port).connect(ClientID, username, mqttpass));
}

void loop() {
  int counter = 0;
  long sum = 0;
  int maximum = 0;

  if (!client.connected()) {
    reconnect();
  }

  int StartTime = millis();
  while ((millis() - StartTime) < 20000) {
    int reading = analogRead(AudioPin);
    sum = sum + (1360 + abs(1360 - reading));
    counter++;
    if (maximum < reading){
      maximum = reading;
    }
    Serial.println(reading);
  }

  //Caluclate Average
  float average = float(sum) / float(counter);
  float voltage = (((float(average) / 2720.0) * 3.3) - 1.65);
  Serial.println(voltage);
  Serial.println(average);

  //Calculate maxVoltage
  float maxvoltage = (((float(maximum) / 2720.0) * 3.3) - 1.65);
  Serial.println(maximum);
  Serial.println(maxvoltage);

  client.publish(topicAverageVolume, String(voltage).c_str());
  client.publish(topicMaximumVolume, String(maxvoltage).c_str());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(ClientID, username, mqttpass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

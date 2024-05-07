#include <stdio.h>
#include <string.h>
#include "secret.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

int FFTValues[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

WiFiClientSecure askClient;
PubSubClient client(askClient);

void setup() {
  Serial.begin(9600);  // Initialize Serial at 9600 baud rate

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Attempting to connect... ");
  }

  askClient.setCACert(local_root_ca);  //If you use non SSL then comment out
  client.setServer(mqtt_server, mqtt_port);
  client.connect(ClientID, username, mqttpass);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (Serial.available() > 0) {
    String receivedDataString = Serial.readStringUntil('\n');
    Serial.println(receivedDataString);

    DynamicJsonDocument doc1(256);  // Adjust size as needed
    DynamicJsonDocument doc2(256);  // Adjust size as needed

    char receivedData[128];
    receivedDataString.toCharArray(receivedData, sizeof(receivedData));

    char *value = strtok(receivedData, ",");
    const char *names[16] = { "80 Hz", "115 Hz", "160 Hz", "225 Hz", "320 Hz", "450 Hz", "630 Hz", "890 Hz", "1250 Hz", "1770 Hz", "2500 Hz", "3550 Hz", "5000 Hz", "7000 Hz", "10000 Hz", "14000 Hz" };

    JsonObject obj1 = doc1.to<JsonObject>();  // Using JsonObject instead of JsonArray
    JsonObject obj2 = doc2.to<JsonObject>();  // Using JsonObject instead of JsonArray

    int i = 0;
    while (value != NULL && i < 8) {
      obj1[names[i]] = atoi(value);  // Assign value to unique name
      value = strtok(NULL, ",");
      i++;
    }

    while (value != NULL && i < 16) {
      obj2[names[i]] = atoi(value);  // Assign value to unique name
      value = strtok(NULL, ",");
      i++;
    }


    char jsonBuffer1[256];
    serializeJson(obj1, jsonBuffer1);

    char jsonBuffer2[256];
    serializeJson(obj2, jsonBuffer2);

    Serial.println(jsonBuffer1);
    Serial.println(jsonBuffer2);

    client.publish(topicSoundReadings1, jsonBuffer1);
    client.publish(topicSoundReadings2, jsonBuffer2);
  }


  client.loop();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(ClientID, username, mqttpass)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, retrying in 5 seconds...");
      delay(5000);
    }
    client.loop();  // Maintain the MQTT connection
  }
}
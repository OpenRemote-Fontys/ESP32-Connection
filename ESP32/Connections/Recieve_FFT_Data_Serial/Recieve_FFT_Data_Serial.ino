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

    DynamicJsonDocument doc(256);  // Adjust size as needed

    char receivedData[128];
    receivedDataString.toCharArray(receivedData, sizeof(receivedData));

    char *value = strtok(receivedData, ",");
    const char *names[16] = {"80Hz", "115Hz", "160Hz", "225Hz", "320Hz", "450Hz", "630Hz", "890Hz", "1250Hz", "1770Hz", "2500Hz", "3550Hz", "5000Hz", "7000Hz", "10000Hz", "14000Hz"};

    JsonObject obj1 = doc.to<JsonObject>(); // Using JsonObject instead of JsonArray
    JsonObject obj2 = doc.to<JsonObject>(); // Using JsonObject instead of JsonArray

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
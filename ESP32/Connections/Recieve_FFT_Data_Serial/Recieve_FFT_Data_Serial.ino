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

    int i = 0;
    while (value != NULL && i < 16) {
      FFTValues[i] = atoi(value);  // Convert string to integer
      value = strtok(NULL, ",");
      i++;
    }

    // Populate the JSON document with FFTValues
    JsonArray array = doc.to<JsonArray>();
    for (int i = 0; i < 16; i++) {
        array.add(FFTValues[i]);
    }

    // Serialize the JSON document into a char array
    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    Serial.println(jsonBuffer);
    client.publish(topicSoundReadings, jsonBuffer);
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
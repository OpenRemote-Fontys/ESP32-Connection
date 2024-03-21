#include "EspMQTTClient.h"

EspMQTTClient client(
  "iotroam",
  "EbSkb8Fxdy",
  "robinvanhoof.tech",  // MQTT Broker server ip // Robinip: 84.29.91.205
  "master:mqttuser",   // Can be omitted if not needed
  "16wV2b8Ov1LRPzALB2wvTAqKVKSzrdgw",   // Can be omitted if not needed
  "esp32-1" ,     // Client name that uniquely identify your device
  8883
);

void setup()
{
  Serial.begin(115200);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  //client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("master/esp32-1/writeattributevalue/writeAttribute/6pWmdLBaJXgwjvYi4V3BEg", [](const String & payload) {
    Serial.println(payload);
  });

  // Publish a message to "mytopic/test"
  client.publish("master/esp32-1/writeattributevalue/writeAttribute/6pWmdLBaJXgwjvYi4V3BEg", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  });
}

void loop()
{
  client.loop();
}
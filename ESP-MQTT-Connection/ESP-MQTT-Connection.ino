#include "secret.h"
//#include <ESP8266WiFi.h> // remove comment for ESP8266, and add comment at #include <WiFi.h> 
#include <WiFi.h>          
#include <PubSubClient.h>

//Objects
//WiFiClientSecure askClient; //SSL Client
WiFiClient askClient; //Non-SSL Client, also remove the comments for askClient.setCACert(local_root_ca);

PubSubClient client(askClient);

void setup() {
  Serial.begin(115200);
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

  }


  Serial.println(WiFi.localIP());
  //askClient.setCACert(local_root_ca); //If you use non SSL then comment out
  Serial.println(client.setServer(mqtt_server, mqtt_port).connect(ClientID, username, mqttpass)) ;
  //client.setCallback(callback);

}

void loop() {

    client.publish(topic, "{ \"msg\":\"1234Message\" }");
  //To publish Strings:
  delay(10000);

}

// //MQTT callback
// void callback(char* topic, byte * payload, unsigned int length) {

//   for (int i = 0; i < length; i++) {
//     Serial.println(topic);
//     Serial.print(" has send ");
//     Serial.print((char)payload[i]);
//   }

// }

// //MQTT reconnect
// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("********** Attempting MQTT connection...");
//     // Attempt to connect
//     if (client.connect(ClientID, username, mqttpass, lastwill, 1, 1, lastwillmsg)) {
//       Serial.println("-> MQTT client connected");
//       client.subscribe(topic);
//       Serial.print("Subscribed to: ");
//       Serial.println(topic);
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println("-> try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

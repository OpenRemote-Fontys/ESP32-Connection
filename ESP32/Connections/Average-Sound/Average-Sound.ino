#include "secret.h"
#include <WiFi.h> 
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>

#define AudioPin 36

//Objects
WiFiClientSecure askClient; //SSL Client
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
  askClient.setCACert(local_root_ca); //If you use non SSL then comment out
  Serial.println(client.setServer(mqtt_server, mqtt_port).connect(ClientID, username, mqttpass)) ;
  //client.setCallback(callback);

}

void loop() {
  int StartTime = micros();
  
  int counter = 0;
    //List or array here

    while (micros() - StartTime < 20000) {

    analogRead(AudioPin);
    counter++;
   }
    //Caluclate Average
    //Send average
    

  delay();

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

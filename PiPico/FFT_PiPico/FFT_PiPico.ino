// Imports for WiFi and MQTT
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include "secret.h"
#include <stdio.h>
#include <string.h>
#include <arduinoFFT.h>

// Variables to be used across project
#define SAMPLES 2048       // Number of samples taken at a time, has to be a power of 2
#define SAMPLE_FREQ 40000  // Interested in frequencies up till 20kHz
#define AUDIO_PIN 26       // Adjust based on your setup
#define NUMBER_BANDS 16    // The number of distinct frequency bands we want to record
#define NOISE 600          // Noise filter, values below this number are filtered
#define AVG_INTERVAL 15000

// Setting up sampling and FFT variables
unsigned int sampling_period_us;
byte peak[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int bandValues[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int averageValuesSum[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long periodStartTime = 0;
const char *names[16] = { "80 Hz", "115 Hz", "160 Hz", "225 Hz", "320 Hz", "450 Hz", "630 Hz", "890 Hz", "1250 Hz", "1770 Hz", "2500 Hz", "3550 Hz", "5000 Hz", "7000 Hz", "10000 Hz", "14000 Hz" };
double vReal[SAMPLES];
double vImaginary[SAMPLES];
unsigned long newTime;
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImaginary, SAMPLES, SAMPLE_FREQ);

const char* local_root_ca = \
                          "-----BEGIN CERTIFICATE-----\n" \
                          "MIIEQjCCAyqgAwIBAgISBBGKU3XIWjEpy64fJ96QuQQuMA0GCSqGSIb3DQEBCwUA\n" \
                          "MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
                          "EwJSMzAeFw0yNDA1MTgwMTA1MjNaFw0yNDA4MTYwMTA1MjJaMBwxGjAYBgNVBAMT\n" \
                          "EXJvYmludmFuaG9vZi50ZWNoMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAEUnO8kBp0\n" \
                          "r4bCos63+k8MuK5JmGc91w7gqpXtEJ56w8nWTK7xEp0eDrBRrxm9uearJKtfQQI4\n" \
                          "HKd2rvzyeHu7ZfAveUbDrfmMmj8dvSQ34wVzGdqAOcq3xoeUHEbdaLREo4ICFDCC\n" \
                          "AhAwDgYDVR0PAQH/BAQDAgeAMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcD\n" \
                          "AjAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBQAVJSPQyotPv0h8QpwkEqnyuHxRjAf\n" \
                          "BgNVHSMEGDAWgBQULrMXt1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcw\n" \
                          "IQYIKwYBBQUHMAGGFWh0dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYW\n" \
                          "aHR0cDovL3IzLmkubGVuY3Iub3JnLzAcBgNVHREEFTATghFyb2JpbnZhbmhvb2Yu\n" \
                          "dGVjaDATBgNVHSAEDDAKMAgGBmeBDAECATCCAQUGCisGAQQB1nkCBAIEgfYEgfMA\n" \
                          "8QB3AD8XS0/XIkdYlB1lHIS+DRLtkDd/H4Vq68G/KIXs+GRuAAABj4lyV9IAAAQD\n" \
                          "AEgwRgIhAL3CHptGgCh0Kt+CgvjWK1j1u50IzFns3o44wrrqkH1xAiEAoqTH5ate\n" \
                          "iwNk5RDIocsIm4NiP5RpVzQKICSmz0qzV/AAdgBIsONr2qZHNA/lagL6nTDrHFIB\n" \
                          "y1bdLIHZu7+rOdiEcwAAAY+Jclr7AAAEAwBHMEUCIAkkN+zi1Aobr/xtZFqMXhUf\n" \
                          "N0P1RXHjuFF0A9MFuVf9AiEAqY7Cn8xvlcvZ/9kZIRBMjffcIFZ7UojU6LRCC1zj\n" \
                          "axAwDQYJKoZIhvcNAQELBQADggEBACHqlfeTo51Lu0i4D+g9mC9yzfLsjTADbCuK\n" \
                          "dUYMG+5COO+3RQbJN0asnaSw1+nzNsQXRENyWLIIT0X4fnXySJxCyUuP9sX9O2EM\n" \
                          "sAu5yPn67C6sycC8JodjsEreDGDSvA2cbUXDB8+5yzSIrViw67b0bvPwJ9kYYvu/\n" \
                          "eBbSFFo/99EFLvDRS3XkaFB6yHMIASMTcPq+e5tmaZS5G4lAznUUXLFunGpgKRlT\n" \
                          "ko1DPNEqkeInhAeaQqh1PRuxUg5aKhzmSV+cj/08Tk+e/Cgrz1R5LSKqHMMoQg/0\n" \
                          "6ZrpXIk2RywPs5JT3AugTkdlLhSh+zfTQcgHy4b3fJfAVhlH+3M=" \
                          "-----END CERTIFICATE-----";


// WiFi and MQTT variables
WiFiClientSecure askClient;
PubSubClient client(askClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin("iotroam", "hndehi24WX");
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  askClient.setCACert(local_root_ca);  //If you use non SSL then comment out

  client.setServer("robinvanhoof.tech", 8883);

  client.connect("picow", "master:mqttuser", "16wV2b8Ov1LRPzALB2wvTAqKVKSzrdgw");

  sampling_period_us = round(1000000 * (1.0 / SAMPLE_FREQ));  // Calculating the time per sample
}

void loop() {
  // Reset band values to 0
  for (int i = 0; i < NUMBER_BANDS; i++) {
    bandValues[i] = 0;
  }

  // Sample on audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_PIN);
    vImaginary[i] = 0;
    while ((micros() - newTime) < sampling_period_us) {
      // Do nothing till the timer per sample has elapsed
    }
  }

  // Now we do the interesting stuff where we compute FFT
  FFT.dcRemoval();
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  // Look at FFT results
  for (int i = 2; i < (SAMPLES / 2); i++) {
    if (vReal[i] > NOISE) {
      if (i > 2 && i <= 5) bandValues[0] += (int)vReal[i];
      if (i > 5 && i <= 7) bandValues[1] += (int)vReal[i];
      if (i > 7 && i <= 10) bandValues[2] += (int)vReal[i];
      if (i > 10 && i <= 14) bandValues[3] += (int)vReal[i];
      if (i > 14 && i <= 20) bandValues[4] += (int)vReal[i];
      if (i > 20 && i <= 28) bandValues[5] += (int)vReal[i];
      if (i > 28 && i <= 39) bandValues[6] += (int)vReal[i];
      if (i > 39 && i <= 55) bandValues[7] += (int)vReal[i];
      if (i > 55 && i <= 78) bandValues[8] += (int)vReal[i];
      if (i > 78 && i <= 109) bandValues[9] += (int)vReal[i];
      if (i > 109 && i <= 154) bandValues[10] += (int)vReal[i];
      if (i > 154 && i <= 218) bandValues[11] += (int)vReal[i];
      if (i > 218 && i <= 308) bandValues[12] += (int)vReal[i];
      if (i > 308 && i <= 434) bandValues[13] += (int)vReal[i];
      if (i > 434 && i <= 612) bandValues[14] += (int)vReal[i];
      if (i > 612) bandValues[15] += (int)vReal[i];
    }
  }

  DynamicJsonDocument doc1(256);  // Adjust size as needed
  DynamicJsonDocument doc2(256);

  JsonObject obj1 = doc1.to<JsonObject>();  // Using JsonObject instead of JsonArray
  JsonObject obj2 = doc2.to<JsonObject>();  // Using JsonObject instead of JsonArray

  for (int i = 0; i < 8; i++) {
    obj1[names[i]] = bandValues[i];  // Assign value to unique name
  }

  for (int i = 8; i < 16; i++) {
    obj2[names[i]] = bandValues[i];  // Assign value to unique name
  }

  char jsonBuffer1[256];
  serializeJson(obj1, jsonBuffer1);

  char jsonBuffer2[256];
  serializeJson(obj2, jsonBuffer2);

  Serial.println(jsonBuffer1);
  Serial.println(jsonBuffer2);

  if (!client.connected()) {
    reconnect();
  }

 // client.publish(topicSoundReadings1, jsonBuffer1);
  //client.publish(topicSoundReadings2, jsonBuffer2);

  while (millis() - periodStartTime < 5000) {
  }

  periodStartTime = millis();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("picow", "master:mqttuser", "16wV2b8Ov1LRPzALB2wvTAqKVKSzrdgw")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, retrying in 5 seconds...");
      delay(5000);
    }
    client.loop();  // Maintain the MQTT connection
  }
}
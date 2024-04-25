#include <stdio.h>
#include <string.h>

int FFTValues[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(9600);  // Initialize Serial at 9600 baud rate
}

void loop() {
  if (Serial.available() > 0) {

    for (int i = 0; i < 16; i++) {
      FFTValues[i] = 0;
    }

    String receivedDataString = Serial.readStringUntil('\n');  // Read data until newline character
    Serial.println(receivedDataString);
    char receivedData[128];

    // Copy the contents of receivedDataString into receivedData buffer
    receivedDataString.toCharArray(receivedData, sizeof(receivedData));

    // Tokenize the string using strtok
    char *value = strtok(receivedData, ",");

    int i = 0;

    // Iterate through the tokens
    while (value != NULL) {
      FFTValues[i] = atoi(value);
      Serial.println(FFTValues[i]);
      value = strtok(NULL, ",");  // Get the next token
      i++  ;                           
    }

  }
}
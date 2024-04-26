//Imports
#include <arduinoFFT.h>

//Variables to be used accross project
#define SAMPLES 2048       //Number of samples take at a time, has to be a power of 2
#define SAMPLE_FREQ 40000  //Interested in frequencuies up till 20kHz
#define AMPLITUDE 1000
#define AUDIO_PIN A0
#define NUMBER_BANDS 16  //The number of destinct frequency bands we want to record
#define NOISE 500        //Noise Filter, values below this number are filtered
#define AVG_INTERVAL 15000

// Setting up sampling and FFT variables
unsigned int sampling_period_us;
byte peak[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int bandValues[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int averageValuesSum[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned int avgCount = 0;
unsigned long averagePeriodStartTime = 0;

double vReal[SAMPLES];
double vImaginary[SAMPLES];
unsigned long newTime;
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImaginary, SAMPLES, SAMPLE_FREQ);


void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  sampling_period_us = round(1000000 * (1.0 / SAMPLE_FREQ));  //Cacluating the time per sample
}

void loop() {

  //Reset Band Values to 0
  for (int i = 0; i < NUMBER_BANDS; i++) {
    bandValues[i] = 0;
  }

  //Sample on audio Pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_PIN);
    vImaginary[i] = 0;
    while ((micros() - newTime) < sampling_period_us) {
      //Do nothing till the timer per sample has elapsed
    }
  }

  //Now we do the interesting stuff where we compute fft
  FFT.dcRemoval();
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  //Loot at FFT results
  for (int i = 2; i < (SAMPLES / 2); i++) {
    if (vReal[i] > NOISE) {
      if (i > 8 && i <= 10) bandValues[0] += (int)vReal[i];
      if (i > 10 && i <= 13) bandValues[1] += (int)vReal[i];
      if (i > 13 && i <= 17) bandValues[2] += (int)vReal[i];
      if (i > 17 && i <= 24) bandValues[3] += (int)vReal[i];
      if (i > 24 && i <= 32) bandValues[4] += (int)vReal[i];
      if (i > 32 && i <= 43) bandValues[5] += (int)vReal[i];
      if (i > 43 && i <= 58) bandValues[6] += (int)vReal[i];
      if (i > 58 && i <= 77) bandValues[7] += (int)vReal[i];
      if (i > 77 && i <= 104) bandValues[8] += (int)vReal[i];
      if (i > 104 && i <= 141) bandValues[9] += (int)vReal[i];
      if (i > 141 && i <= 189) bandValues[10] += (int)vReal[i];
      if (i > 189 && i <= 255) bandValues[11] += (int)vReal[i];
      if (i > 255 && i <= 344) bandValues[12] += (int)vReal[i];
      if (i > 344 && i <= 463) bandValues[13] += (int)vReal[i];
      if (i > 463 && i <= 624) bandValues[14] += (int)vReal[i];
      if (i > 624) bandValues[15] += (int)vReal[i];
    }
  }

  for (int j = 0; j < NUMBER_BANDS; j++) {
    averageValuesSum[j] += bandValues[j];
  }

  avgCount++;

  if (millis() - averagePeriodStartTime > 20000) {
    for (int i = 0; i < NUMBER_BANDS; i++) {
      Serial1.print(averageValuesSum[i]/avgCount);
      Serial.print(averageValuesSum[i]/avgCount);
      if (i < NUMBER_BANDS - 1) {
        Serial1.print(",");
        Serial.print(",");
      }
    }
    Serial1.println();
    Serial.println();
    avgCount = 0;

  for (int j = 0; j < NUMBER_BANDS; j++) {
      averageValuesSum[j] = 0;
    }

    averagePeriodStartTime = millis();
  }


  delay(10000);
}

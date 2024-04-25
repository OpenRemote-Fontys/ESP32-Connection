//Imports
#include <arduinoFFT.h>

//Variables to be used accross project
#define SAMPLES 2048       //Number of samples take at a time, has to be a power of 2
#define SAMPLE_FREQ 40000  //Interested in frequencuies up till 20kHz
#define AMPLITUDE 1000
#define AUDIO_PIN A0
#define NUMBER_BANDS 16  //The number of destinct frequency bands we want to record
#define NOISE 500        //Noise Filter, values below this number are filtered

// Setting up sampling and FFT variables
unsigned int sampling_period_us;
byte peak[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int bandValues[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
      if (i > 2 && i <= 5) bandValues[0] += (int)vReal[i];
      else if (i > 5 && i <= 7) bandValues[1] += (int)vReal[i];
      else if (i > 7 && i <= 10) bandValues[2] += (int)vReal[i];
      else if (i > 10 && i <= 14) bandValues[3] += (int)vReal[i];
      else if (i > 14 && i <= 20) bandValues[4] += (int)vReal[i];
      else if (i > 20 && i <= 28) bandValues[5] += (int)vReal[i];
      else if (i > 28 && i <= 39) bandValues[6] += (int)vReal[i];
      else if (i > 39 && i <= 55) bandValues[7] += (int)vReal[i];
      else if (i > 55 && i <= 78) bandValues[8] += (int)vReal[i];
      else if (i > 78 && i <= 109) bandValues[9] += (int)vReal[i];
      else if (i > 109 && i <= 154) bandValues[10] += (int)vReal[i];
      else if (i > 154 && i <= 218) bandValues[11] += (int)vReal[i];
      else if (i > 218 && i <= 308) bandValues[12] += (int)vReal[i];
      else if (i > 308 && i <= 434) bandValues[13] += (int)vReal[i];
      else if (i > 434 && i <= 612) bandValues[14] += (int)vReal[i];
      else if (i > 612) bandValues[15] += (int)vReal[i];
    }
  }

  for (int i = 0; i < NUMBER_BANDS; i++) {
    Serial1.print(bandValues[i]);
    Serial.print(bandValues[i]);
    if (i < NUMBER_BANDS - 1) {
      Serial1.print(",");
      Serial.print(",");
    }
  }
  Serial1.println();
  Serial.println();


  delay(10000);
}

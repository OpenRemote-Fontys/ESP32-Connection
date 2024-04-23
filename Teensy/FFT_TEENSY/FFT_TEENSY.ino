//Imports
#include <arduinoFFT.h>

//Variables to be used accross project
#define SAMPLES 1024       //Number of samples take at a time, has to be a power of 2
#define SAMPLE_FREQ 32000  //Interested in frequencuies up till 16kHz
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
      switch (i) {
        case 0 ... 2:
          bandValues[0] += (int)vReal[i];
          break;
        case 3 ... 3:
          bandValues[1] += (int)vReal[i];
          break;
        case 4 ... 5:
          bandValues[2] += (int)vReal[i];
          break;
        case 6 ... 7:
          bandValues[3] += (int)vReal[i];
          break;
        case 8 ... 9:
          bandValues[4] += (int)vReal[i];
          break;
        case 10 ... 13:
          bandValues[5] += (int)vReal[i];
          break;
        case 14 ... 18:
          bandValues[6] += (int)vReal[i];
          break;
        case 19 ... 25:
          bandValues[7] += (int)vReal[i];
          break;
        case 26 ... 36:
          bandValues[8] += (int)vReal[i];
          break;
        case 37 ... 50:
          bandValues[9] += (int)vReal[i];
          break;
        case 51 ... 69:
          bandValues[10] += (int)vReal[i];
          break;
        case 70 ... 97:
          bandValues[11] += (int)vReal[i];
          break;
        case 98 ... 135:
          bandValues[12] += (int)vReal[i];
          break;
        case 136 ... 189:
          bandValues[13] += (int)vReal[i];
          break;
        case 190 ... 264:
          bandValues[14] += (int)vReal[i];
          break;
        default:
          bandValues[15] += (int)vReal[i];
          break;
      }
    }
  }

  for (int i = 0; i < NUMBER_BANDS; i++) {
    Serial.print("Band: ");
    Serial.print(i);
    Serial.print(" Value: ");
    Serial.println(bandValues[i]);
  }

  delay(10000);
}

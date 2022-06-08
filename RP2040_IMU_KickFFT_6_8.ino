/*
  This example reads audio data from the on-board PDM microphones, and prints
  out the samples to the Serial console. The Serial Plotter built into the
  Arduino IDE can be used to plot the audio data (Tools -> Serial Plotter)

  Circuit:
  - Arduino Nano RP2040 connect board
  This example code is in the public domain.
*/


#include "KickFFT.h"
#include <Arduino_LSM6DSOX.h>

const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2  => 총 128의 가속도 값을 샘플링함
const double frequency = 16000;   // this frequency(16Khz) makes 64 samples from PDM module 
const float Fs = frequency;  // FFT 결과의 Full Scale => 16Khz 까지의 주파수 성분 검출 가능 그러나 최종적으로는 8Khz까지의 정보만 볼 수 있음
uint32_t mag[samples] = {0};  // mag[] 어레이에는 총 128개의 FFT 결과가 저장됨
uint16_t startIndex = 0;
uint16_t endIndex = 0;

// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[samples];   // 참조형 global 변수 => 여기에 fft 결과값이 저장됨  

// Number of IMU samples read
volatile int samplesRead=0;    //   

void setup() {
  Serial.begin(9600);
  
  while (!Serial);
  if (!IMU.begin()) {  // IMU 센서 사용하도록 begin()
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float x[samples], y[samples], z[samples];  // 3축 가속도 센서값을 128개 저장하기 위한 어레이 선언
  
  if (IMU.accelerationAvailable()) {
    if (samplesRead < samples) {   // samplesRead 값이 samples(128)보다 작다면 본 if block를 수행함 
      IMU.readAcceleration(x[samplesRead], y[samplesRead], z[samplesRead]);  //  가속도 센서값을 x,y,z 변수에 저장함
//    Serial.print(x);
//    Serial.print('\t');
//    Serial.print(y);
//    Serial.print('\t');
//    Serial.println(z);
      samplesRead ++;
      //Serial.println(samplesRead);    // loop() 함수가 실행될 때 마다 계속해서 x[samplesRead] 값을 저장함
  }
 
   // x[samples] 값이 다 저장되면 하기의 명령이 수행됨 만일 samplesRead값이 128이 아니면 수행안됨
  if (samplesRead==128) {   // 측정된 x 변수에 저장된 값을 sampleBuffer[] 변수에 저장함
     for (int i=0; i<samples; i++)
     {
       sampleBuffer[i] = x[i];
     }
   
    KickFFT<int16_t>::fft(Fs, 0, Fs/2, samples, sampleBuffer, mag, startIndex, endIndex);  // sampleBuffer 변수는 참조형 변수임

  //Print to Serial Monitor and copy and paste into a .csv file to display in Excel
    Serial.println("Freq(Hz),Magnitude"); //Use SerialUSB for SparkFun SAMD21 boards
    for(uint16_t i = startIndex; i < endIndex; i++)
    {
    //Peak should be around 1.3 Hz or so
    
//     Serial.print(i*Fs/samples); //Use SerialUSB for SparkFun SAMD21 boards
//     Serial.print(","); //Use SerialUSB for SparkFun SAMD21 boards
     Serial.print(mag[i]); //Use SerialUSB for SparkFun SAMD21 boards
     Serial.println(); //Use SerialUSB for SparkFun SAMD21 boards
    }
    samplesRead = 0;
    //while(1);  // 한번 수행수행
  }
}
}

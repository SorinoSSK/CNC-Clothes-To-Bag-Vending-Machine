// SPDX-FileCopyrightText: 2011 Limor Fried/ladyada for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit
#include <AutoPID.h>
// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 87988.16      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 28.1  
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000

#define OUTPUT_MIN 0
#define OUTPUT_MAX 255
#define KP 23.5
#define KI 1.566
#define KD 72.8

int samples[NUMSAMPLES];

double temperature, setPoint, outputVal;

AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

void setup(void) {
  Serial.begin(115200);
  pinMode(A1, OUTPUT);
  digitalWrite(A1,LOW);
  //if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  myPID.setBangBang(4);
  //set PID update interval to 4000ms
  myPID.setTimeStep(4000);
//  pinMode(A1, OUTPUT);
//  analogReference(EXTERNAL);
}

void loop(void) 
{
  float average;

  // take N samples in a row, with a slight delay
  for (int i = 0; i < NUMSAMPLES; i++)
  {
    if (i == NUMSAMPLES - 1)
    {
      samples[i] = analogRead(THERMISTORPIN);
    }
    else
    {
      samples[i] = samples[i+1];
    }
    
  }  
  // average all the samples out
  average = 0;
  for (int i=0; i< NUMSAMPLES; i++) 
  {
     average += samples[i];
  }
  average /= NUMSAMPLES;

//  Serial.print("Average analog reading "); 
//  Serial.println(average);
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
//  Serial.print("Thermistor resistance "); 
//  Serial.println(average);
  
  temperature = average / THERMISTORNOMINAL;     // (R/Ro)
  temperature = log(temperature);                  // ln(R/Ro)
  temperature /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  temperature += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  temperature = 1.0 / temperature;                 // Invert
  temperature -= 273.15;                         // convert absolute temp to C

  setPoint = 132;
  myPID.run(); //call every loop, updates automatically at certain time interval
  analogWrite(A1, outputVal);
//  digitalWrite(LED_PIN, myPID.atSetPoint(1)); //light up LED when we're at setpoint +-1 degree
  Serial.print("Temperature: "); 
  Serial.print(temperature);
  Serial.println(" *C");
}

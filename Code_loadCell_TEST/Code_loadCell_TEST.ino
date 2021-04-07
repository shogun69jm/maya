/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.

 This example code uses bogde's excellent library:"https://github.com/bogde/HX711"
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.
 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

//#define calibration_factor 7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN1  3
#define LOADCELL_SCK_PIN1  2
#define calibration_factor1 7050.0 
#define LOADCELL_DOUT_PIN2  5
#define LOADCELL_SCK_PIN2  4
#define calibration_factor2 7050.0 
#define LOADCELL_DOUT_PIN3  7
#define LOADCELL_SCK_PIN3  6
#define calibration_factor3 7050.0 
#define LOADCELL_DOUT_PIN4  9
#define LOADCELL_SCK_PIN4  8
#define calibration_factor4 7050.0 

HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("HX711 scale demo. Calibration factors  ");
  Serial.print("Calibration factor 1 :");
  Serial.println(calibration_factor1);
  Serial.print("Calibration factor 2 :");
  Serial.println(calibration_factor2);
  Serial.print("Calibration factor 3 :");
  Serial.println(calibration_factor3);
  Serial.print("Calibration factor 4 :");
  Serial.println(calibration_factor4);

  initScale(1);
  initScale(2);
  initScale(3);
  initScale(4);

  /*
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  */
  Serial.println("Readings:");
}

void loop() {
  
  Serial.print("Reading: S1= ");
  Serial.print(scale1.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" S2=");
  Serial.print(scale2.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" S3=");
  Serial.print(scale3.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" S4=");
  Serial.print(scale4.get_units(), 1); //scale.get_units() returns a float
  
 // Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  delay(2000);
}

void initScale(int loadcell){

  switch (loadcell){
  case 1:
    scale1.begin(LOADCELL_DOUT_PIN1, LOADCELL_SCK_PIN1);
    scale1.set_scale(calibration_factor1);
    scale1.tare();
    break;
  case 2:
    scale2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2);
    scale2.set_scale(calibration_factor2);
    scale2.tare(); 
  case 3:
    scale3.begin(LOADCELL_DOUT_PIN3, LOADCELL_SCK_PIN3);
    scale3.set_scale(calibration_factor3);
    scale3.tare();
    break;
  case 4:
    scale4.begin(LOADCELL_DOUT_PIN4, LOADCELL_SCK_PIN4);
    scale4.set_scale(calibration_factor4); 
    scale4.tare(); 
    break;
  default:
    // statements
    break;
}


}

#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;


HX711 scale;


void setup()
{
  Serial.begin(9600);
  Serial.println("Beginning");

  scale.power_up();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(2000);                    // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();                            // reset the scale to 0
  Serial.println("Now put upper plate"); // my upper plate is about 6 kg ->6000 g
  delay(10000);
  float f= scale.get_units(10);
  Serial.println(f);                      //here program prints values we need to divide with 6000g
  scale.power_down();     
 //
}
void loop()
{

}

/*
void setup()
{
  Serial.begin(9600);
  Serial.print("Beginning");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(39.8);    // instead of Calibration put a number you calculated on last program   

}
void loop()
{
  float ff=scale.get_units(10);
  Serial.println(ff);
  scale.power_down();                 // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
}
*/
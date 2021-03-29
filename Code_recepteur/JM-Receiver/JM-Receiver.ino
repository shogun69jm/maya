/*
  433 MHz RF Module Receiver Demonstration 2
  RF-Rcv-Demo-2.ino
  Demonstrates 433 MHz RF Receiver Module with DHT-22 Sensor
  Use with Transmitter Demonstration 2
 
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h>
// Includ LiquidCrystal by Arduino, Adafruit
#include <LiquidCrystal.h>

// Functions
//String displayLCD(String);
 
// Define output strings
 
String str_humid;
String str_temp;
String str_out;
int i =0;

 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// Create LCD Board
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    // Setup Serial Monitor
    Serial.begin(9600);

    // LCD board init
    lcd.begin(16,2); // Max size
    lcd.setCursor(0,0);

    Serial.print("RECEIVER Setup");
}
 
void loop()
{
    // Set buffer to size of expected message
    uint8_t buf[11];
    uint8_t buflen = sizeof(buf);
    
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {
      Serial.println("RECEIVING...");

      // Message received with valid checksum
      // Get values from string
      
      // Convert received data into string
      str_out = String((char*)buf);
      
      // Split string into two values
      for (int i = 0; i < str_out.length(); i++) {
		  if (str_out.substring(i, i+1) == ",") {
			str_humid = str_out.substring(0, i);
			str_temp = str_out.substring(i+1);
			break;
		  }
		}
      
      // Print values to Serial Monitor
      Serial.print("Humidity: ");
      Serial.print(str_humid);
      Serial.print("  - Temperature: ");
      Serial.println(str_temp);
   
      lcd.clear();
      lcd.println("OK");
      delay(2000);
      lcd.clear();
    
    }
}


/*
PROJECT ESILV PIX 2021 : Connected hive
............................. REVEIVER
AUTHOR : Théo PATARD
VERSION : 
Libraries :
 VirtualWire http://www.airspayce.com/mikem/arduino/VirtualWire/index.html
 LiquidCrystal LiquidCrystal by Arduino, Adafruit
*/


//#include <VirtualWire.h> // On inclut la librairie VirtualWire
//#include <VirtualWire_Config.h>
//#include <LiquidCrystal.h>
#include <RH_ASK.h>
#include <SPI.h>


//const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
RH_ASK rf_driver;

//uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
//uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
    
void setup()
{
    
    lcd.begin(16,2); // Définit la taille du message max, ici donc la taille de l'écran
    lcd.setCursor(0,0); // Définit la position du curseur au début 
    
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Début communication");
    
    vw_set_rx_pin(11);
    vw_setup(2000); // initialisation de la librairie VirtualWire à 2000 bauds
    vw_rx_start();  // Activation de la partie réception de la librairie VirtualWire

}
 
void loop()
{
   String val = "";
   char c;    
  
    vw_wait_rx(); // Attend qu'un message soit reçu
  
 
  
      if (vw_get_message(buf, &buflen)) // On copie le message, qu'il soit corrompu ou non
      {
          Serial.print("On recoit : ");
          
          Serial.println((char*)buf);

          lcd.print((char*)buf);// Affiche le message sur l'écran

          delay(1000);
          lcd.clear();
          
        }
   
}




/*
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  // set up the LCD's number of columns and rows:

  lcd.begin(16, 2);
}

void loop() {

  // set the cursor to (0,0):

  lcd.setCursor(0, 0);

  // print from 0 to 9:

  for (int thisChar = 0; thisChar < 10; thisChar++) {

    lcd.print(thisChar);

    delay(500);

  }

  // set the cursor to (16,1):

  lcd.setCursor(16, 1);

  // set the display to automatically scroll:

  lcd.autoscroll();

  // print from 0 to 9:

  for (int thisChar = 0; thisChar < 10; thisChar++) {

    lcd.print(thisChar);

    delay(500);

  }

  // turn off automatic scrolling

  lcd.noAutoscroll();

  // clear screen for the next loop:

  lcd.clear();
}
*/
#include <Adafruit_LiquidCrystal.h>

#include <VirtualWire.h>
#include <VirtualWire_Config.h>

#include <VirtualWire.h>
#include <VirtualWire_Config.h>

#include <VirtualWire.h> // On inclut la librairie VirtualWire
#include <VirtualWire_Config.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd (7,6,5,4,3,2); // Définit la fonction des différentes pins

uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
    
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

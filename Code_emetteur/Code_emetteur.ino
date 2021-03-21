/*
PROJECT ESILV PIX 2021 : Connected hive
............................. TRANSMITTER
VERSION : 
Libraries : VirtualWire http://www.airspayce.com/mikem/arduino/VirtualWire/index.html
*/


#include <VirtualWire.h>
#include <VirtualWire_Config.h>
#include <DHT.h>

#define DHTPIN 8 // Definit la broche de l'Arduino sur laquelle la broche DATA du capteur est reliee 
#define DHTTYPE DHT11 // Definit le type de capteur utilise

DHT dht(DHTPIN, DHTTYPE); // Declare un objet de type DHT
                            
void setup()
{
    Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Début communication"); // Petit message pour tester que l'interface sérial se lance bien

    dht.begin();
     
    vw_set_tx_pin(12);                                        
    vw_setup(2000);     // initialisation de la librairie VirtualWire à 2000 bauds


}
                                             
void loop()
{   
    String t_str = "T = " + String(dht.readTemperature());
    String h_str = " H = " + String(dht.readHumidity());
    char tab[27];
    
    for(int i=0; i< 8; i++)
    {
     tab[i] = t_str[i];
    }

    int x = 0;
    for(int i=8; i< 27; i++)
    {    
     tab[i] = h_str[x];
     x++;
    }

    char *msg = tab;
    
    Serial.print("Sending..."); // On signale le début de l'envoi
    vw_send((uint8_t*)msg, strlen(msg)); // On envoie le message 
    vw_wait_tx(); // On attend la fin de l'envoi
    Serial.println("Done !"); // On signale la fin de l'envoi
    delay(2000); // Et on attend 1s pour éviter que deux messages se superpose
}

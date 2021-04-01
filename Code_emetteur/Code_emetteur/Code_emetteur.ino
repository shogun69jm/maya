/*Todo
Créer une structure de trame réseau
cryto d'identification
dip ? comment l'utiliser pour le code ?
la version à transférer pour l'init
la température C
la température F
l'humidité
les 4 poids

Regarder comment fonctionnent les fonctions de send/receive
Mettre des diodes pour signaler la réception et l'emission
Comment on enregistre les données ?
Chargeur solaire
Recherche sur des algo abeilles
*/

/*
ESILV PIX 2021
Author      : Théo Patard
Date        : 27/03/2021
Project     : Connected Hive
Subproject  : Transmitter & Sensors (Temperature, humididy and load)
*/

#include <VirtualWire.h>
#include <VirtualWire_Config.h>
#include <DHT.h>

#define _DHT_PIN 8 // Definit la broche de l'Arduino sur laquelle la broche DATA du capteur est reliee 
#define _DHTTYPE DHT11 // Definit le type de capteur utilise
#define _TRANSMITTER_PIN 12 //Pin 12
#define _LED_PIN 11 //Pin 11
#define _TRANSMITTER_SPEED 2400 //2000 bauds
#define _SERIALMONITOR_SPEED 9600
#define _BUFFER_MAXLEN 128

#define _DIP1_PIN 2
#define _DIP2_PIN 3
#define _DIP3_PIN 4


#define _RUNNING_DELAY 2000 //2 seconds

// Hymidity & Temperature sensor
DHT myDHT(_DHT_PIN,_DHTTYPE); // Declare un objet de type DHT

//---------------------------------------------------------------- SETUP    
void setup()
{

// Init du debug monitor   
Serial.write(27);       // ESC command
Serial.print("[2J");    // clear screen command
Serial.write(27);  
Serial.begin(_SERIALMONITOR_SPEED); // Initialisation du port série pour avoir un retour sur le serial monitor
Serial.println("Début transmission"); // Petit message pour tester que l'interface sérial se lance bien

// Init des PIN Arduino utilisés
pinMode(_TRANSMITTER_PIN,OUTPUT);   //Emetteur
pinMode(_DHT_PIN, INPUT);           //Capteur Humidité et température
pinMode(_DIP1_PIN, INPUT_PULLUP);   //DIP PIN 1 activation de la résistance interne de type Pull Up
pinMode(_DIP2_PIN, INPUT_PULLUP);   //DIP PIN 2 activation de la résistance interne de type Pull Up
pinMode(_DIP3_PIN, INPUT_PULLUP);   //DIP PIN 3 activation de la résistance interne de type Pull Up

// Init du détecteur DHT   
myDHT.begin();

// Init de l'émetteur    
vw_set_tx_pin(_TRANSMITTER_PIN);                                        
vw_setup(_TRANSMITTER_SPEED);     // initialisation de la librairie VirtualWire à 2000 bauds

// Init de la LED
digitalWrite(_LED_PIN, HIGH);

}

//---------------------------------------------------------------- LOOP                                       
void loop()
{   
static char outstr1[16], outstr2[16];
static String myDIP;

// Idenfication du numéro de l'émetteur
myDIP = String(readDIP());

// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = myDHT.readHumidity();
// Read temperature as Celsius
float tc = myDHT.readTemperature();
// Read temperature as Fahrenheit
float tf = myDHT.readTemperature(true);
// Formatage Température sur 3 chiffres dont 1 décimale
dtostrf(tc,3,1,outstr1);
// Formatage Humidité sur 2 chiffres dont O décimale
dtostrf(h,2,0,outstr2);

String t_str = String(outstr1)+"C";
String h_str = String(outstr2)+"%";
String myMail = "R"+ myDIP + " " + t_str + " " + h_str;

//Serial.println(myMail);

    if(digitalRead(_TRANSMITTER_PIN) == LOW)
    {
      //char *msg = tab;
      char * msg ;
      //convertit ma chaine en un buffer de char
      msg = string2Buffer(myMail);
    
      String toto ;
      toto = (char*)msg;
      Serial.print("Send >> "); // On signale le début de l'envoi
      Serial.println(toto);

      digitalWrite(_LED_PIN, true); // Flash a light to show transmitting
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(_LED_PIN, false);

      delay(_RUNNING_DELAY); // Et on attend 2s pour éviter que deux messages se superpose

      //Ne pas oublier de libérer l'allocation !
      free(msg);
    }
}

//---------------------------------------------------------------- LECTURE DU DIP
unsigned short readDIP() {

 unsigned short dmxVal = 0;

 if(digitalRead(_DIP3_PIN) == LOW) dmxVal += 1;
 if(digitalRead(_DIP2_PIN) == LOW) dmxVal += 2;
 if(digitalRead(_DIP1_PIN) == LOW) dmxVal += 4;
 
 
 return dmxVal;
}

//---------------------------------------------------------------- CONVERSION String vers un tab de char
char* string2Buffer(String myMail){
  int len = myMail.length();
  char * buffer = malloc(len * sizeof(char));
  for (int i = 0 ; i <len; i++){
    buffer[i] = myMail.charAt(i);
  }
  return  buffer;
}

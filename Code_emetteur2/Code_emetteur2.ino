/*Todo
Créer une structure de trame réseau
cryto d'identification
Comment vider proprement le serial monitor
Regarder comment fonctionnent les fonctions de send/receive
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

#define TRANSMITTER_PROG_VERSION 0.01

#include <VirtualWire.h>
#include <VirtualWire_Config.h>
#include <DHT.h>
#include <HX711.h>

//CRYPTO et ID
#define _CRYPTO "XC0dMrCNzKxqmX4J"
#define _ID "1"

//SENSOR
#define _DHT_PIN 13 // Definit la broche de l'Arduino sur laquelle la broche DATA du capteur est reliee 
#define _DHTTYPE DHT11 // Definit le type de capteur utilise

//TRANSMITTER
#define _TRANSMITTER_PIN 12 //Pin 12
#define _TRANSMITTER_SPEED 2400 //2000 bauds
#define _LED_PIN 11 //Pin 11

//LOADCELL
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

//MISC
#define _SERIALMONITOR_SPEED 9600
#define _BUFFER_MAXLEN 128
#define _DIP1_PIN 10
#define _RUN_PAUSE 2000 //2 seconds


// Hymidity & Temperature sensor
DHT myDHT(_DHT_PIN,_DHTTYPE); // Declare un objet de type DHT

//LoadCell & HX711
HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

//2 compteurs pour debug
int loopcount = 0;
int looptransmit = 0;

//---------------------------------------------------------------- SETUP    
void setup()
{
// Init du debug monitor   
Serial.begin(_SERIALMONITOR_SPEED); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("TRANSMITTER BOARD SETUP..........");
    Serial.print("Version : ");
    Serial.println(TRANSMITTER_PROG_VERSION);
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.print(" - ");
    Serial.println(__TIME__);
    Serial.println("Transmission is starting...");
//bannerDisplay();

// Init des PIN Arduino utilisés
pinMode(_TRANSMITTER_PIN,INPUT);   //Emetteur
pinMode(_DHT_PIN, INPUT);          //Capteur Humidité et température
pinMode(_LED_PIN, INPUT);          //Led
pinMode(_DIP1_PIN, INPUT_PULLUP);   //DIP PIN 1 activation de la résistance interne de type Pull Up
//pinMode(_DIP2_PIN, INPUT_PULLUP);   //DIP PIN 2 activation de la résistance interne de type Pull Up
//pinMode(_DIP3_PIN, INPUT_PULLUP);   //DIP PIN 3 activation de la résistance interne de type Pull Up

// Init du détecteur DHT   
myDHT.begin();

// Init de l'émetteur    
vw_set_tx_pin(_TRANSMITTER_PIN);                                        
vw_setup(_TRANSMITTER_SPEED);     // initialisation de la librairie VirtualWire à 2000 bauds

// Init de la LED
digitalWrite(_LED_PIN, HIGH);

// Init des LoadCell
initScale(1);
initScale(2);
initScale(3);
initScale(4);
}

//---------------------------------------------------------------- LOOP                                       
void loop(){     
//Initialisation des chaines et tableau
String myDIP = "0";
char outstrT[7]={'0','0','.','0','\0',' '};
char outstrH[4]={'0','0','\0',' '};
char outstrL1[8] = {0};
char outstrL2[8] = {0};
char outstrL3[8] = {0};
char outstrL4[8] = {0};
char myBuffer[_BUFFER_MAXLEN]={0};
char* myTemplate = "";
char* id = _ID ;
char* hi = _CRYPTO ;

//DBG Je compte le nb de loop
loopcount++;

//............................................................ D I P
//Idenfication du numéro de l'émetteur avec le DIP
myDIP = String(readDIP());

//............................................................ TP & HU
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = myDHT.readHumidity();
// Read temperature as Celsius
float tc = myDHT.readTemperature();
// Read temperature as Fahrenheit
//float tf = myDHT.readTemperature(true);

// Formatage Température sur 3 chiffres dont 1 décimale
dtostrf(tc,4,1,outstrT);
// Formatage Humidité sur 2 chiffres dont O décimale
dtostrf(h,2,0,outstrH);

//............................................................ LOAD CELLs
// Mesure des 4 points de charge
float l1 = scale1.get_units();
float l2 = scale2.get_units();
float l3 = scale3.get_units();
float l4 = scale4.get_units();
// Formatage float vers string
dtostrf(l1,2,1,outstrL1);
dtostrf(l2,2,1,outstrL2);
dtostrf(l3,2,1,outstrL3);
dtostrf(l4,2,1,outstrL4);

//............................................................ MESSAGE FORMAT
//myTemplate = "_ST_ID%s_HI%s_TP%s_HU%s_L1%s_L2%s_L3%s_L4%s_ED_";
myTemplate = "%s;%s;%s;%s;%s;%s;%s;%s";
sprintf(myBuffer,myTemplate,hi,id,outstrT,outstrH, outstrL1,outstrL2,outstrL3,outstrL4);

//............................................................ DEBUG
Serial.print("DBG MESSG=");
String myPacket = "";
myPacket = (char*)myBuffer;
myPacket += "\0";
Serial.println(myPacket);

  if(digitalRead(_TRANSMITTER_PIN) == LOW)
  {
    
    //Je compte le nb de transmission
    looptransmit++;

    digitalWrite(_LED_PIN, true); // Flash a light to show transmitting

    //Envoi du message
    vw_send((uint8_t *)myBuffer, strlen(myBuffer));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(_LED_PIN, false);

    //DEBUG
    Serial.print("DBG: OK=");
    Serial.print(looptransmit);
    Serial.print("/");
    Serial.print(loopcount);
    Serial.print(" buflen=");
    Serial.print(String(strlen(myBuffer)));
    Serial.print(" freq=");
    Serial.print(_RUN_PAUSE);
    Serial.println("ms");

    //Ne pas oublier de libérer l'allocation si utiliser la fontion string2Buffer
    //free(msg);
  }
  else {
    Serial.println("No signal transmitted...");
  }
  delay(_RUN_PAUSE); // Et on attend 2s pour éviter que deux messages se superpose  
}

//---------------------------------------------------------------- LECTURE DU DIP
unsigned short readDIP() {

 unsigned short dmxVal = 0;

 if(digitalRead(_DIP1_PIN) == LOW) dmxVal += 1;
 //if(digitalRead(_DIP2_PIN) == LOW) dmxVal += 2;
 //if(digitalRead(_DIP3_PIN) == LOW) dmxVal += 4;
 
 return dmxVal;
}

// INUTILE # préféré sprintf ---------------------------------------------------------------- CONVERSION String vers un tab de char
char* string2Buffer(String mess){
  int len = mess.length();
  //char *buffer_p = (char*)malloc(sizeof(char)*bufferSize);
  char *buffer = (char*)malloc(sizeof(char)*len);
  //char* buffer = malloc(len * sizeof(char));
  for (int i = 0 ; i <len; i++){
    buffer[i] = "";
    buffer[i] = mess.charAt(i);
  }
  return  buffer;
}

//---------------------------------------------------------------- Banner
void bannerDisplay(){
  //delay(2000);  
  if (Serial.available()){
    Serial.println("TRANSMITTER BOARD SETUP..........");
    Serial.print("Version : ");
    Serial.println(TRANSMITTER_PROG_VERSION);
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.print(" - ");
    Serial.println(__TIME__);
    Serial.println("Transmission is starting...");
    delay(2000);
  }
}

//---------------------------------------------------------------- Init LoaCell scale
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




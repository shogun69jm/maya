/*
ESILV PIX 2021
Author      : Théo Patard
Date        : 27/03/2021
Project     : Connected Hive
Subproject  : Receiver & Display & Save
*/

#define RECEIVER_PROG_VERSION 0.01

#include <VirtualWire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
//#include <SD.h>
//#include <SPI.h>

// Arduino UNO Pin
#define _RECEIVER_PIN 9 //Pin 11

#define _LED_PIN 8 //10

#define _LCD_RS 6
#define _LCD_EN 7
#define _LCD_D4 5
#define _LCD_D5 4
#define _LCD_D6 3
#define _LCD_D7 2

#define _microSD_READER_MOSI 11
#define _microSD_READER_MISO 12
#define _microSD_READER_SCK  13
#define _microSD_READER_CS   10

#define _LCD_MAXLINE 2
#define _LCD_MAXCHAR 16

#define _RECEIVER_SPEED 2400 //2000 bauds

#define _RUNNING_DELAY 2000 //5 seconds de pause
#define _TIMEOUT 5000 //Timeout de 5 seconds pour le récepteur, si aucune réception

//Mode DEBUG
const bool DEBUG = true;

// Components declaration
RTC_DS3231 myRTC; // DS3231 RTC module

// LCD LiquidCrystal lcd(rs, enable, d4, d5, d6, d7) 
LiquidCrystal myLCD(_LCD_RS,_LCD_EN,_LCD_D4,_LCD_D5,_LCD_D6,_LCD_D7);

// BUFFER 
uint8_t buf[VW_MAX_MESSAGE_LEN] = {0}; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
uint8_t loopcount = 0;

const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/*
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;
*/

//File myFile;
String fileBuffer;
String filename = "test";


//---------------------------------------------------------------- SETUP    
void setup()
{
  Serial.begin(9600); // Initialisation du serial monitor
  if (DEBUG) bannerDisplay();


  //RTC setup
  myRTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  

  if (! myRTC.begin()) {
    if (DEBUG) Serial.println("Couldn't find RTC");
    while (1);
  }

  //if (SD.begin(_microSD_READER_CS)&&(DEBUG)) Serial.println("Initializing SD CARD...");
  
  
  if (myRTC.lostPower()) {
    Serial.println("RTC lost power, lets set the time!"); //La batterie est à plat !
	
	// Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    myRTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));

  digitalWrite(_LED_PIN,HIGH); //Init de la LED réception
  }

  //LCD Setup
  myLCD.noDisplay();
  myLCD.begin(_LCD_MAXCHAR,_LCD_MAXLINE); // Définit la taille du message max, ici donc la taille de l'écran
  myLCD.setCursor(0,0); // Définit la position du curseur au début
  myLCD.print("PIX 2021");
  myLCD.print("Connected Hive");   
  myLCD.display();

  //RECEIVER Setup  
  vw_set_rx_pin(_RECEIVER_PIN);
  vw_setup(_RECEIVER_SPEED); // initialisation de la librairie VirtualWire à 2000 bauds
  vw_rx_start();  // Activation de la partie réception de la librairie VirtualWire
}

//---------------------------------------------------------------- LOOP 
void loop()
{
  String val = "";
  loopcount++;

  DateTime now = myRTC.now(); 
  
  // Boucle de réception HF
  if (vw_wait_rx_max(_TIMEOUT)){
    if (vw_get_message(buf, &buflen)) // On copie le message, qu'il soit corrompu ou non
    {
      // Flash la led pour montrer la réception
      digitalWrite(_LED_PIN,true);
    
      //Serial Monitor
      if (DEBUG){
        Serial.print("RCV >> ");
        Serial.print(getDateTime());
        Serial.print(" > ");
        Serial.print(loopcount);
        Serial.print(" ");
        Serial.println((char*)buf);
      }      

      //Buffer parsing vers un tableau formaté de 8 positions(mesure)
      char* token ;
      String mesure[8][2] = {"",""};
      int j=0;
      token = strtok((char*)buf,";");
      mesure[0][1]="Code"; mesure[1][1]="Id"; mesure[2][1]="T"; mesure[3][1]="H"; mesure[4][1]="L1"; mesure[5][1]="L2"; mesure[6][1]="L3"; mesure[7][1]="L4";
      mesure[j][0] = String(token);
      while(token)
      {
        //Serial.println(token);
        token = strtok(NULL,";");
        j++;
        mesure[j][0] = String(token);

      }

      // Debug : Affichage sur le serial Monitor
      if (DEBUG){
        for (int k=0;k<8;k++){
          Serial.print(mesure[k][1]);
          Serial.print("=");
          Serial.println(mesure[k][0]);      
        }
      }

      myLCD.clear();
      myLCD.setCursor(0,0);
      myLCD.print(getDateTime());
      myLCD.setCursor(0,1);
      myLCD.display();

      //Afficheur LCD
      String myT = mesure[2][0];
      String myH = mesure[3][0];
      String myL = mesure[4][0];
      myLCD.print(myT);
      myLCD.write((char)223);
      myLCD.print("C "+myH+"% "+myL+"g");

      //LED flash
      digitalWrite(_LED_PIN,false);
      
      //Pause
      delay(_RUNNING_DELAY); //Vitesse inférieur à celle de l'emetteur, on prend son temps !
      free(token);
    }
}
else
{
  if (DEBUG) Serial.println("No signal...");
  myLCD.clear();
  myLCD.setCursor(0,0);
  myLCD.print("No signal...");
}
}

//---------------------------------------------------------------- Fonction donnant la date / heure
String getDateTime()
{
  String myYear, myMonth, myDay, myDayOfTheWeek, myHour, myMinute, mySecond;
  String myDateTime;
  DateTime now = myRTC.now();   
  myYear = now.year();
  myMonth = now.month();
  myDay = now.day();
  myDayOfTheWeek = daysOfTheWeek[now.dayOfTheWeek()];
  myHour = now.hour();
  myMinute = now.minute();
  mySecond = now.second();
   
  return myHour+":"+myMinute+":"+mySecond+" "+myDay+"."+myMonth+"."+myYear;
}

//---------------------------------------------------------------- Banner
void bannerDisplay(){
  delay(2000);
  Serial.println("RECEIVER BOARD SETUP..........");
  Serial.print("Version : ");
  Serial.println(RECEIVER_PROG_VERSION);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.print(" - ");
  Serial.println(__TIME__);
  Serial.println("Reception is starting...");
}


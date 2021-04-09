/*
ESILV PIX 2021
Author      : Théo Patard
Date        : 27/03/2021
Project     : Connected Hive
Subproject  : Receiver & Display
*/

#define RECEIVER_PROG_VERSION 0.01

#include <VirtualWire.h> // On inclut la librairie VirtualWire
#include <VirtualWire_Config.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>

// Arduino UNO Pin
#define _RECEIVER_PIN 11 //Pin 11
#define _LED_PIN 10
#define _LCD_RS 6
#define _LCD_EN 7
#define _LCD_D4 5
#define _LCD_D5 4
#define _LCD_D6 3
#define _LCD_D7 2

#define _RECEIVER_SPEED 2400 //2000 bauds
#define _LCD_MAXLINE 2
#define _LCD_MAXCHAR 16
#define _RUNNING_DELAY 2000 //5 seconds de pause
#define _TIMEOUT 5000 //Timeout de 5 seconds pour le récepteur, si aucune réception

RTC_DS3231 myRTC; // DS3231 RTC module
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// LCD LiquidCrystal lcd(rs, enable, d4, d5, d6, d7)
LiquidCrystal myLCD(_LCD_RS,_LCD_EN,_LCD_D4,_LCD_D5,_LCD_D6,_LCD_D7);

// BUFFER 
uint8_t buf[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message reçu (de taille maximum VW_MAX_MESSAGE_LEN)
uint8_t buflen = VW_MAX_MESSAGE_LEN; // Taille maximum de notre tableau
uint8_t counter = 0;

//---------------------------------------------------------------- SETUP    
void setup()
{
  Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
  bannerDisplay();

  //RTC setup
  myRTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  

  if (! myRTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (myRTC.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
	
	// Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    myRTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));

  digitalWrite(_LED_PIN,HIGH);    
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
  char c; 
  counter++;

  DateTime now = myRTC.now();
  
 // vw_wait_rx(); // Attend qu'un message soit reçu
  
  if (vw_wait_rx_max(_TIMEOUT)){
    if (vw_get_message(buf, &buflen)) // On copie le message, qu'il soit corrompu ou non
    {
      // Flahs la led pour montrer la réception
      digitalWrite(_LED_PIN,true);
    
      //Serial Monitor
      Serial.print("RCV >> ");
      Serial.print(getDateTime());
      Serial.print(" > ");
      Serial.print(counter);
      Serial.print(" ");
      Serial.println((char*)buf);

      myLCD.clear();
      myLCD.setCursor(0,0);
      myLCD.print(getDateTime());
      myLCD.setCursor(0,1);
      myLCD.display();

      char* carac = malloc(buflen * sizeof(char));
      carac = (char *)buf;
      String myT = String(carac[0])+String(carac[1])+String(carac[2])+String(carac[3]); // 4 premiers caractères pour la température
      String myH = String(carac[4])+String(carac[5]); // 2 caractères suivants pour l'humidité
      //float T = myT.toFloat();
      //float H = myH.toFloat();
      myLCD.print(myT);
      myLCD.write((char)223);
      myLCD.print("C "+myH+"%");

      digitalWrite(_LED_PIN,false);
      
      delay(_RUNNING_DELAY); //Vitesse inférieur à celle de l'emetteur, on prend son temps !
    }
}
else
{
  Serial.println("No signal...");
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
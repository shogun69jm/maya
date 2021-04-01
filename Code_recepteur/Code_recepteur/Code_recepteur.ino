/*
ESILV PIX 2021
Author      : Théo Patard
Date        : 27/03/2021
Project     : Connected Hive
Subproject  : Receiver & Display
*/

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
#define _RUNNING_DELAY 5000 //5 seconds de pause

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
  //Setup banner
  Serial.begin(9600); // Initialisation du port série pour avoir un retour sur le serial monitor
  Serial.println("RECEIVER BOARD SETUP..........");

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.print(" - ");
  Serial.println(__TIME__);

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
  myLCD.begin(_LCD_MAXCHAR,_LCD_MAXLINE); // Définit la taille du message max, ici donc la taille de l'écran
  myLCD.setCursor(0,0); // Définit la position du curseur au début   
  //Serial.println("Receiving...");

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
  
  vw_wait_rx(); // Attend qu'un message soit reçu
  
  if (vw_get_message(buf, &buflen)) // On copie le message, qu'il soit corrompu ou non
  {
    // Flahs la led pour montrer la réception
    digitalWrite(_LED_PIN,true);
  
    //Serial Monitor
    Serial.print("Receive >> ");
    Serial.print(getDateTime());
    Serial.print(" > ");
    Serial.print(counter);
    Serial.print(" ");
    Serial.println((char*)buf);

    //LCD Display
    myLCD.clear();
    myLCD.setCursor(0,0);
    myLCD.print(getDateTime());
    myLCD.setCursor(0,1);
    myLCD.print((char*)buf);// Affiche le message sur l'écran

    digitalWrite(_LED_PIN,false);
    
    delay(_RUNNING_DELAY); //Vitesse inférieur à celle de l'emetteur, on prend son temps !
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
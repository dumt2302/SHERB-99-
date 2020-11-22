#include <Arduino.h>
#include "LiquidCrystal.h"
#include <SPI.h>
#include <Wire.h>
#include "LibRobus.h"

int TEMPS_WARMUP = 900;
unsigned long time;
int val;

LiquidCrystal lcd(7,8,9,10,11,12);
int analogPin = 7;

void setup()   {                

 lcd.begin(16,2);
 lcd.setCursor(0,1);
 lcd.print("Twistiti :)");
}


void loop() {  
  
  delay(100);

  val = readAlcohol();
  lcd.clear();
  printWarming();

  time = millis()/1000;
  
  if(time<=TEMPS_WARMUP)
  {
    time = map(time, 0, TEMPS_WARMUP, 0, 100);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Ready to use!");
  }else
  {
    lcd.clear();
    printValeur(val);
    printTexteIndicateur(val);
  }
  lcd.display();

}

void printWarming() 
{
  lcd.setCursor(0,1);
  lcd.print("Twisititi");
  lcd.setCursor(0,2);
  lcd.print("Warming Up!");
}

void printValeur(int value)
{
  lcd.setCursor(0,1);
  lcd.print(val);
}

void printTexteIndicateur(int value)
{
  lcd.setCursor(0,2);
  
  if(value<200)
  {
      lcd.print("Tu es sobre.");
  }
  if (value>=200 && value<280)
  {
      lcd.print("Une beer!");
  }
  if (value>=280 && value<350)
  {
      lcd.print("T'es tipsy!");
  }
  if (value>=350 && value <450)
  {
      lcd.print("Conduit pas!");
  }
  if(value>450)
  {
     lcd.print("Puke to restart!");
  }
 }
 
 int readAlcohol()
 {
  int val = 0;
  int val1;
  int val2;
  int val3; 

  lcd.clear();
  val1 = analogRead(analogPin); 
  delay(10);
  val2 = analogRead(analogPin); 
  delay(10);
  val3 = analogRead(analogPin);
  
  val = (val1+val2+val3)/3;
  return val;
 }



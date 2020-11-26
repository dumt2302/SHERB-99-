#include <Arduino.h>
#include <LiquidCrystal.h>
#include <libRobus.h>
#include <Wire.h>
#include <SPI.h>

int TEMPS_WARMUP = 900;
unsigned long time;
int val;

int analogPin = 0;
int boutonStart = 2;
int boutonStop = 1;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

bool BoutonStart()
{
  bool interaction = false;
  if(digitalRead(boutonStart) == LOW)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Rechauffement");
    interaction = true;
  }
  else
  if(digitalRead(boutonStop) == LOW)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Arret");
    interaction = false;
  }
  
  return interaction;
}

/*void printWarming() 
{
  lcd.setCursor(0,1);
  lcd.print("Twisititi");
  lcd.setCursor(0,2);
  lcd.print("Warming Up!");
}*/

/*void printValeur(int value)
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
 }*/
 
 int readAlcohol()
 {
  int val = 0;
  int val1;
  int val2;
  int val3; 

  //lcd.clear();
  val1 = analogRead(analogPin); 
  Serial.print("/");
  Serial.print(val1);
  delay(10);
  val2 = analogRead(analogPin); 
  Serial.print("/");
  Serial.print(val2);
  delay(10);
  val3 = analogRead(analogPin);
  Serial.print("/");
  Serial.print(val3);
  Serial.print("/");
  
  

  val = (val1+val2+val3)/3;
  Serial.print(val);
  Serial.print("\n");
  return val;
 }

void setup()   {                

 BoardInit();
 SERVO_Enable(0);
 pinMode(boutonStart, INPUT_PULLUP);
 pinMode(boutonStop, INPUT_PULLUP);
 /*lcd.begin(16,2);
 lcd.setCursor(0,1);
 lcd.print("Twistiti :)");
 pinMode(A0, INPUT);*/
}

void loop()
{
  bool arret = false;
  if(BoutonStart() == true)
  {
    do
    {
      delay(1000);
      lcd.print(readAlcohol());
      if(digitalRead(boutonStop) == LOW)
      {
        arret = true;
      }
    } while(arret == false);
  }
}
/*void loop() {  
  
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
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
  }else
  {
    lcd.print("Ready to use!");
    lcd.clear();
    printValeur(val);
    printTexteIndicateur(val);
  }
  lcd.display();

}*/
#include <Arduino.h>
#include <librobus.h>





#define LED_BLUE 2 
#define LED_WHITE 3 
#define LED_RED 4

#define LED_GREEN1 5
#define LED_GREEN2 6
#define LED_GREEN3 7

#define nbrEclairage 4 //Détermine le nombre de fois où les LEDs alternent

int buzzer = 36;
int out; /* la sortie du breatheanalyzer*/

int melody[] = {523, 587, 659, 698, 784, 880, 988, 1046};
int duration = 500;  

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_WHITE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(buzzer,OUTPUT);
    pinMode(LED_GREEN1, OUTPUT);
    pinMode(LED_GREEN2, OUTPUT);
    pinMode(LED_GREEN3, OUTPUT);
}

void loop()
{
    if ( out >= 0.08) // le 0.008 représente la taux d'alcool dans le sang interdit(80mg/100ml)
    {
        digitalWrite(LED_BLUE,HIGH);
        delay(500);
        digitalWrite(LED_BLUE,LOW);
        digitalWrite(LED_WHITE,HIGH);
        delay(500);
        digitalWrite(LED_WHITE,LOW);
        digitalWrite(LED_RED,HIGH);
        delay(500);
        digitalWrite(LED_RED,LOW);
        digitalWrite(LED_BLUE,HIGH);
        delay(500);
        digitalWrite(LED_BLUE,LOW);
        digitalWrite(LED_WHITE,HIGH);
        delay(500);
        digitalWrite(LED_WHITE,LOW);
        digitalWrite(LED_RED,HIGH);
        delay(500);
        digitalWrite(LED_RED,LOW);
        digitalWrite(LED_BLUE,HIGH);
        delay(500);
        digitalWrite(LED_BLUE,LOW);
        digitalWrite(LED_WHITE,HIGH);
        delay(500);
        digitalWrite(LED_WHITE,LOW);
        digitalWrite(LED_RED,HIGH);
        delay(500);
        digitalWrite(LED_RED,LOW);
        delay(500);

        digitalWrite(buzzer,300); /* 300 etant la valeur de hz du buzzer */
        delay(10);
        digitalWrite(buzzer,800);
        delay(10);
    }
    else /* LA VALEUR BOUL EST 0 (DONC AUCUN ALCOOL DANS LE SANG)*/
    {
        digitalWrite(LED_GREEN1,HIGH);
        delay(500);
        digitalWrite(LED_GREEN2,HIGH);
        delay(500);
        digitalWrite(LED_GREEN3,HIGH);
        delay(500);
        digitalWrite(LED_GREEN1,LOW);
        delay(500);
        digitalWrite(LED_GREEN2,LOW);
        delay(500);
        digitalWrite(LED_GREEN3,LOW);
        delay(500);
    
    
        for (int thisNote = 0; thisNote < 8; thisNote++) 
        {
            // pin8 output the voice, every scale is 0.5 sencond
            tone(8, melody[thisNote], duration);
            delay(1000);
        }
    }  
}

void LumierePolice()
{
    
    for(int i = 0; i<nbrEclairage; i++)
    {
        digitalWrite(LED_BLUE,HIGH);
        delay(500);
        digitalWrite(LED_BLUE,LOW);
        digitalWrite(LED_WHITE,HIGH);
        delay(500);
        digitalWrite(LED_WHITE,LOW);
        digitalWrite(LED_RED,HIGH);
        delay(500);
        digitalWrite(LED_RED,LOW);
     }
        digitalWrite(buzzer,300); /* 300 etant la valeur de hz du buzzer */
        delay(10);
        digitalWrite(buzzer,800);
        delay(10);
        digitalWrite(buzzer, 0);
}

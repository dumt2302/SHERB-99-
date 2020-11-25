#include <Arduino.h>
#include <librobus.h>
#include <Wire.h>
#include "LiquidCrystal.h"
#include <SPI.h>

#define redpin A3
#define greenpin A5
#define bluepin A6 
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

#define LED_BLUE 2 
#define LED_WHITE 3 
#define LED_RED 4

#define LED_GREEN1 5
#define LED_GREEN2 6
#define LED_GREEN3 7

#define nbrEclairage 4 //Détermine le nombre de fois où les LEDs alternent


int buzzer = 36;

int melody[] = {523, 587, 659, 698, 784, 880, 988, 1046};
int duration = 500;  


int TEMPS_WARMUP = 900;
unsigned long time;
int val;
LiquidCrystal lcd(7,8,9,10,11,12);
int analogPin = 7;

int32_t EncodeurDroite = 0;
int32_t EncodeurGauche = 0;
int32_t sumDiff = 0;
bool A = 0;
bool B = 1;
int VirageDegre;
bool Bouton = 0;
bool Pression = 0;
bool TropChaud = 1;
bool TropPlate = 0; 
bool TestResult; 

void MasterSlave(float MasterMOTOR, int32_t MasterENCODEUR, int32_t SlaveENCODEUR) //Prends en entrée la valeur du moteur maitre et les valeurs d'encodeur
{
  float SlaveMOTOR;                                       //valeur du moteur slave
  int32_t Diff_ENCODEUR = MasterENCODEUR - SlaveENCODEUR; //différence de pas entre les deux moteurs
  sumDiff += Diff_ENCODEUR;
  float kp = 0.001;                                             //valeur de la constante P kp = 0.001
  float ki = 0.00003;                                           //valeur de la constante I ki = 0.00003                            
  SlaveMOTOR = MasterMOTOR + kp * Diff_ENCODEUR + ki * sumDiff; //équation de la valeur du moteur slave
  MOTOR_SetSpeed(1, SlaveMOTOR);
}

void Avancer(int DistanceCm)
{


  ENCODER_ReadReset(0); //reset des encodeurs
  ENCODER_ReadReset(1);
  float vitesse;
  float Vmax = 0.7; //vitesse max
  float Vmin = 0.1; //vitesse min
  float a = 0.03;   //acceleration (chiffre magique)
  float distance = 0;
  int compteur = 1;


  float y1 = a * distance + Vmin; //fonction d'acceleration
  float y2 = Vmax;  //fonction de vitesse max
  float y3 = a * (DistanceCm - distance) + Vmin;  //fonction de decceleration
  #if MOTOR_REVERSED  //car un encodeur est inversé
  compteur = -1;

#endif
   while (DistanceCm > distance)
  {
    y1 = a * distance + Vmin;
    y2 = Vmax;
    y3 = a * (DistanceCm - distance) + Vmin;
    if(y1 < y2 && y1 < y3)  //l'equation utilisee pour trouver la vitesse est toujour celle donnant le plus petit resultat dans le temps
    {
      distance = ENCODER_Read(0) * 0.0074809175 * compteur;
      y1 = a * distance + Vmin;
      vitesse = y1;
      MOTOR_SetSpeed(0, vitesse);
      MasterSlave(vitesse, ENCODER_Read(0), ENCODER_Read(1));
      delay(10);

    }
    else if(y2 < y1 && y2 < y3)
    {
      distance = ENCODER_Read(0) * 0.0074809175 * compteur;
      y2 = Vmax;
      vitesse = y2;
      MOTOR_SetSpeed(0, vitesse);
      MasterSlave(vitesse, ENCODER_Read(0), ENCODER_Read(1));
      delay(10);
    }
    else
    {
      distance = ENCODER_Read(0) * 0.0074809175 * compteur;
      y3 = a * (DistanceCm - distance) + Vmin;
      vitesse = y3;
      MOTOR_SetSpeed(0, vitesse);
      MasterSlave(vitesse, ENCODER_Read(0), ENCODER_Read(1));
      delay(10);
    }
  }

  delay(10);
}

void virage(int VirageDegre,bool robot)
{
  ENCODER_ReadReset(1);
  ENCODER_ReadReset(0);
  // code
  //la circonférence d'une roue est 23.93894cm
  //la circonférence d'un tour du robot est de 59.6902cm
  //1 degré est égal à 24.934 pulse, donc 25 pulse.

  //définition des variables
    
    int Arondir;
    float UnPulse;
    if (robot==0){
     UnPulse= 0.0469;
      }
   else{
     UnPulse = 0.0469;
     }

    float ValeurPulse;
    //trouve le nombre de pulse nécessaire au virage
    ValeurPulse = abs(VirageDegre/UnPulse);
    Arondir = round(ValeurPulse);
    
   //réinitialise le compteur de pule de l'encodeur
   ENCODER_Reset(0);
   ENCODER_Reset(1);
  //Permet d'arrêter le virage à la bonne direction. 
  
    //Défini si le virage sera vers la gauche ou vers la droite. Positif est en sense horaire et négatif est anti-horaire. 
      if (VirageDegre>=0)
      {
        ENCODER_Reset(0);
        ENCODER_Reset(1);
        while (ENCODER_Read(0)<= Arondir)
        {
          if (ENCODER_Read(0) <= (Arondir/3)) 
          {
            MOTOR_SetSpeed(0,0.4);
            MOTOR_SetSpeed(1,-0.4);
          }
          else if (ENCODER_Read(0) <= (2*Arondir/3) )
          {
            MOTOR_SetSpeed(0,0.3);
            MOTOR_SetSpeed(1,-0.3);
          }
          else
          {
            MOTOR_SetSpeed(0,0.2);
            MOTOR_SetSpeed(1,-0.2);
          }
          
        }
        
      }
      else if(VirageDegre<=0)
      {
        while (ENCODER_Read(1)<= Arondir)
        {
          if (ENCODER_Read(1)<= (Arondir/3))
          {
            MOTOR_SetSpeed(0,-0.4);
            MOTOR_SetSpeed(1,0.4);
          }
          
          else if (ENCODER_Read(1) <= (2*Arondir/3))
          {
            MOTOR_SetSpeed(0,-0.3);
            MOTOR_SetSpeed(1,0.3);
          }
          else 
          {
            MOTOR_SetSpeed(0,-0.2);
            MOTOR_SetSpeed(1,0.2);
          }
        }
        
        
      }
  
  //Arrête la rotation
  MOTOR_SetSpeed(0,0);
  MOTOR_SetSpeed(1,0);
  //réinitialise le compteur de pule de l'encodeur
   ENCODER_Reset(0);
   ENCODER_Reset(1);

   
}

void TiroirOuver()
{
  SERVO_SetAngle(0, 180);//toiroire s'ouvre
  //SERVO_SetAngle(1, 180);
  delay(2000);
}

void Toiroirefermer()
{
  SERVO_SetAngle(0, 0); //moteur force pour le barre
 // SERVO_SetAngle(1, 0); 
  delay(2000);
}

void PressurePlate()
{
  // tout depend de la sotie de la pressure plate avec variable globale qui change
  
}

void Activationdubouton()
{
  //il faut savoir la sortie du bouton avec variable globale qui se modifie
}

void setup()
{
   lcd.begin(16,2);
 lcd.setCursor(0,1);
 lcd.print("Twistiti :)");

 Serial.begin(9600);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_WHITE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(buzzer,OUTPUT);
    pinMode(LED_GREEN1, OUTPUT);
    pinMode(LED_GREEN2, OUTPUT);
    pinMode(LED_GREEN3, OUTPUT);
}

//SHIT DE BREATHALYZER
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
      TestResult=TropPlate;
  }
  if (value>=200 && value<280)
  {
      lcd.print("Une beer!");
      TestResult=TropPlate;
  }
  if (value>=280 && value<350)
  {
      lcd.print("T'es tipsy!");
      TestResult=TropPlate;
  }
  if (value>=350 && value <450)
  {
      lcd.print("Conduit pas!");
      TestResult=TropChaud;
  }
  if(value>450)
  {
     lcd.print("Puke to restart!");
     TestResult=TropChaud;
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

//Si la personne ne pette pas la balloon
void TestPass()
{
  //LumiereVert();
  TiroirOuver();
}

//pete la bine et la balloon
void TestFail()
{
  //Affichier a l'ecran (Vous avez trop bu, appelez un Uber)
 // LumierePolice();
  //virage(720,A);
  delay(1000);
  //virage(-720,A);
  delay(10000);
}

//lumiere rouge ou bleu et rouge lorsque l'alcoo test est échoué
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

//lorsque l'alco-test est réussi
void LumiereVert()
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

//fonction alcoo-test
void Breathalyzer()
{
  //si 0 alchool return 0
  //si trop chaud return 1
  //si erreur return -1
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

void loop() //fonction qui permet a l'utilisateur de prendre son taux d'alchool
{
  SERVO_Enable(0);//active les servomoteurs 
  SERVO_Enable(1);

  //Affichier a l'ecran (Bonsoir peser sur le bouton lors que vous etre pret)

  Toiroirefermer();
  delay(2000);
  TiroirOuver();
  Activationdubouton();

  //Affichier a l'ecran(Veiller deposer vos cles)

  if (Bouton !=0)// lorsque l'utilisateur pese sur le bouton activation de la fonction
  {
    TiroirOuver();
  
    PressurePlate();
    if(Pression != 0)
    {
      delay(5000);
      //Affichier a l'ecran(Merci)
      Toiroirefermer();
    }
    
    delay(5000);
    
    Breathalyzer();
    
    

    if (TestResult=TropPlate)
    {
      TestPass();
    }
    
    else if(TestResult=TropChaud)
    {
      TestFail();
    }

    TiroirOuver();
    SERVO_Disable(1);
    SERVO_Disable(0);
  }

}


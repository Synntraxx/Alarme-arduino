/*       
 *       Matthis Sanier
 *       Alarme-1_v-3   Alarme avec code et porte et fenêtre + écran lcd
 *       
 *       buzzer, bouton poussoir, carte arduino, écran lcd
 *       
 *       01/10/2024 Alarme-2_v-1
*/

#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

const int ledPin = 6;
const int Fenetre = 10;
const int Porte = 8;
const int Buzzer = 12;

byte ledState = 0;  
byte AlarmState = 0;
int buttonStatePorte = 0;
int buttonStateFenetre = 0;

char CodeTableau[4] = {0x31, 0x32, 0x33, 0x34}; // Code initial
char Code[4]; // Code entré par l'utilisateur
bool CodeBon; // Variable pour vérifier si le code est correct
bool buzzerActif = false; // Variable pour gérer le buzzer

void SaisieCode() {
  // Compare le code entré avec le code défini dans le programme
  CodeBon = true;
  for (byte i = 0; i < 4; i++) {
    if (Code[i] != CodeTableau[i]) {
      CodeBon = false;
      break;
    }
  }
}

void Menu() {
  lcd.println("Menu");
  delay(150);
  if (AlarmState == 0) {
    lcd.println("1 : Activ alarm");
  }
  if (AlarmState == 1) {
    lcd.println("2 : Desac alarm");
  }
  if (AlarmState == 0) {
    lcd.println("3.Verif code");
    lcd.println("4.Modif code");
    lcd.println("R.R");
  }
  lcd.println("");
}

void Activer(){
  Serial.flush();
  CodeVerifs();
  
  if (CodeBon) {
    AlarmState = 1;
    ledState = 1;
    lcd.println("Alarm Activ");
    lcd.clear();
    digitalWrite(ledPin, ledState);
    delay(1000);
    Menu();
  } else {
    lcd.println("R menu");
    Menu();
  }
}

void Desactiver(){
  CodeVerifs();
  if (CodeBon) {
    AlarmState = 0;
    ledState = 0;
    buzzerActif = false;
    lcd.println("Alarm Desac");
    lcd.clear();
    digitalWrite(ledPin, ledState);
    noTone(Buzzer);  // Désactive le buzzer
    delay(1000);
    Menu();
  } else {
    lcd.println("R menu");
    Menu();
  }
}

void CodeVerifs() {
  lcd.clear();
  lcd.println("Entrer code");
  while (Serial.available() < 4) {
    // Attend que l'utilisateur entre 4 chiffres
  }
  
  delay(100);
  
  // Lit les 4 caractères du code entré
  for (byte i = 0; i < 4; i++) {
    Code[i] = Serial.read();
  }
  
  for (byte i = 0; i < 4; i++) {
    lcd.print(Code[i]);
  }
  
  lcd.println("");
  SaisieCode();
  delay(1000);

  if (CodeBon) {
    lcd.println("Bon");
  } else {
    lcd.println("incorrect");
  }
}

void CodeModifs() {
  lcd.println("Nouveau code");
  while (Serial.available() < 4) {
    // Attend que l'utilisateur entre 4 chiffres
  }
  
  delay(100);

  // Lit les 4 caractères du nouveau code
  for (byte i = 0; i < 4; i++) {
    CodeTableau[i] = Serial.read();
  }

  lcd.print("Nouveau code");
  
  for (byte i = 0; i < 4; i++) {
    lcd.print(CodeTableau[i]);
  }
  
  lcd.println();
}

void FenetreOuverte() {
  if (buttonStateFenetre == HIGH && AlarmState == 1) {
    lcd.println("Fenêt ouvert alarm on");
    buzzerActif = true;  // Active le buzzer tant que l'alarme n'est pas désactivée
  }
}

void PorteOuverte() {
  if (buttonStatePorte == HIGH && AlarmState == 1) {
    lcd.println("Port ouvert 15 sec !");
    for (int i = 15; i > 0; i--) {
      lcd.print("Il reste ");
      lcd.print(i);
      lcd.println(" secondes.");
      delay(1000);
      if (AlarmState == 0) return;  // Si l'alarme est désactivée pendant les 15 secondes, arrête
    }
    buzzerActif = true;  // Active le buzzer après 15 secondes
  }
}

void BuzzerHandler() {
  if (buzzerActif) {
    tone(Buzzer, 1000);  // Active le buzzer
  } else {
    noTone(Buzzer);  // Désactive le buzzer
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(ledPin, OUTPUT);
  pinMode(Porte, INPUT);
  pinMode(Fenetre, INPUT);
  pinMode(Buzzer, OUTPUT);
  Menu();  // Affiche le menu principal au démarrage
}

void loop() {
  buttonStatePorte = digitalRead(Porte);
  buttonStateFenetre = digitalRead(Fenetre);

  // Gérer la porte et la fenêtre
  FenetreOuverte();
  PorteOuverte();
  BuzzerHandler();  // Gérer le buzzer

  if (Serial.available() > 0) {
    char command = Serial.read();  // Lit la commande

    switch (command) {
      case '1':
        if (AlarmState == 0) {
          Activer();
        }
        break;
              
      case '2':
        Desactiver();
        break;

      case '3':
        if (AlarmState == 0) {
          lcd.println("R menu principal");
          lcd.clear();
          CodeVerifs();
          Menu();
        }
        break;

      case '4':
        if (AlarmState == 0) {
          CodeModifs();
          Menu();
        }
        break;

      case 'R':
        if (AlarmState == 0) {
          lcd.println("R menu principal");
          lcd.clear();
          Menu();
        }
        break;

      default:
        lcd.println("non reconnue");
        break;
    }
    
    // Réinitialise le buffer Serial
    while (Serial.available() > 0) {
      Serial.read();
    }

    delay(1000);
  }
}

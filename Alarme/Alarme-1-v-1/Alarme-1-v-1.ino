const int ledPin = 6;
byte ledState = 0;  
byte AlarmState = 0;

char CodeTableau[4] = {0x31, 0x32, 0x33, 0x34}; // Code initial
char Code[4]; // Code entré par l'utilisateur
bool CodeBon; // Variable pour vérifier si le code est correct

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
  Serial.println("Menu:");
  Serial.println("1 : Activer");
  Serial.println("2 : Désactiver");
  Serial.println("3 : Vérification");
  Serial.println("4 : Modifier le code");
  Serial.println("5 : Exit");

}

void Activer(){
  AlarmState = 1;
  ledState = 1;
  Serial.println("Activer");
  digitalWrite(ledPin, ledState);
  delay(3000);
  Menu();
  
  
}

void Desactiver(){
  AlarmState = 0;
  ledState = 0;
  Serial.println("Desactiver");
  digitalWrite(ledPin, ledState);
  delay(3000);
  Menu();
  
}


void CodeVerifs() {

  Serial.println("Entrez le code (4 chiffres) : ");
  while (Serial.available() < 4) {
    // Attend que l'utilisateur entre 4 chiffres
  }
  delay(100);

  // Lit les 4 caractères du code entré
  for (byte i = 0; i < 4; i++) {
    Code[i] = Serial.read();
  }


  Serial.print("Le code : ");
  Serial.println(CodeTableau);
  for (byte i = 0; i < 4; i++) {
    Serial.print(Code[i]);
  }
  
  Serial.println("");

  SaisieCode();

  delay(3000);

  if (CodeBon) {
    Serial.println("Code Bon");
  } else {
    Serial.println("Code incorrect");
  }
}

void CodeModifs() {
  Serial.println("Entrez le nouveau code (4 chiffres) : ");
  while (Serial.available() < 4) {
    // Attend que l'utilisateur entre 4 chiffres
  }
  delay(100);

  // Lit les 4 caractères du nouveau code
  for (byte i = 0; i < 4; i++) {
    CodeTableau[i] = Serial.read();
  }

  Serial.print("Nouveau code: ");
  for (byte i = 0; i < 4; i++) {
    Serial.print(CodeTableau[i]);
  }
  Serial.println();
}
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  CodeVerifs();
  Menu(); // Affiche le menu principal au démarrage
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read(); // Lit la commande

    switch (command) {
      case '1':
        Activer(); // Appelle la fonction pour vérifier le code
        break;
        
      case '2':
        Desactiver(); // Appelle la fonction pour modifier le code
        break;

      case '3':
        Serial.println("Retour au menu principal...");
        CodeVerifs();
        Menu();// Affiche le menu principal
        break;

      case '4':
        CodeModifs();
        Menu();// Affiche le menu principal
        break;

      case '5':
        Serial.println("Retour au menu principal...");
        Menu(); // Affiche le menu principal
        break;


      default:
        Serial.println("Commande non reconnue.");
        break;
    }
    
    // Réinitialise le buffer Serial
    while (Serial.available() > 0) {
      Serial.read();
    }

    delay(2000);
  }
}

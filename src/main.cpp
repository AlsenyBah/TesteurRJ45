/*
Titre:Titre du programme 
Auteur:Alseny Bah
Date:04/03/2025
Description:Une description du programme
Version:0.0.1 */



// #include <Arduino.h>
//  // Broches de sortie pour l'émetteur
// int outputPins[] = {0, 1, 2, 3, 4, 5, 6, 7}; // D0 à D7

// void setup() {
//   Serial.begin(9600);
//   while (!Serial); // Attend que le moniteur série soit prêt
//   // Configure les broches comme sorties
//   for (int i = 0; i < 8; i++) {
//     pinMode(outputPins[i], OUTPUT);
//     digitalWrite(outputPins[i], LOW); // Initialise à LOW
//   }
//   Serial.println("Émetteur prêt. Début des tests...");
// }

// void loop() {
//   // Teste chaque fil en envoyant HIGH
//   for (int i = 0; i < 8; i++) {
//     digitalWrite(outputPins[i], HIGH);
//     Serial.print("Envoi HIGH sur fil ");
//     Serial.println(i + 1);
//     delay(1000); // Attend 1 seconde
//     digitalWrite(outputPins[i], LOW);
//     delay(500); // Pause entre les tests
//   }
//   Serial.println("Cycle terminé. Reprise...");
//   delay(2000); // Pause avant de recommencer
// }  


//--------------------------------------------------------------------------------------------------------

// Ici vous avez le second code du microcontroleur MKR 1010 recepteur veuillez le commenté ou le décomenté pour seulement exécuté soit le code de l'émetteur ou du recepteur sur le bon microntroleur 

// #include <Arduino.h>

// // Broches utilisées pour le récepteur

// int broches[] = {0, 1, 2, 3, 4, 5, 6, 7}; // D0 à D7 pour les 8 fils du RJ45
// int ordreRecu[8]; // Stocke l'ordre reçu

// void setup() {
//   Serial.begin(9600);
//   while (!Serial); // Attend que le moniteur série soit prêt
  
//   // Configure toutes les broches comme entrées avec pull-up
//   for (int i = 0; i < 8; i++) {
//     pinMode(broches[i], INPUT_PULLUP);
//   }
  
//   Serial.println("Mode Récepteur activé. En attente de signaux...");
// }

// void loop() {
//   bool signalDetecte = false;
  
//   // Vérifie chaque broche pour détecter un signal
//   for (int i = 0; i < 8; i++) {
//     if (digitalRead(broches[i]) == HIGH) {
//       ordreRecu[i] = 1;
//       signalDetecte = true;
//       Serial.print("Signal détecté sur broche ");
//       Serial.println(i + 1);
//       while (digitalRead(broches[i]) == HIGH); // Attend que le signal redescende
//     } else {
//       ordreRecu[i] = 0;
//     }
//   }
  
//   // Analyse les résultats si un signal a été détecté
//   if (signalDetecte) {
//     analyserCable();
//     delay(1000); // Pause avant le prochain cycle
//   }
// }

// void analyserCable() {
//   // Ordre attendu pour câble droit (T568B)
//   int ordreDroit[8] = {1, 2, 3, 4, 5, 6, 7, 8};
//   // Ordre attendu pour câble croisé (T568B vers T568A)
//   int ordreCroise[8] = {3, 6, 1, 4, 5, 2, 7, 8};
  
//   bool estDroit = true;
//   bool estCroise = true;
//   int filsDetectes = 0;
  
//   // Compte les fils détectés
//   for (int i = 0; i < 8; i++) {
//     if (ordreRecu[i] == 1) filsDetectes++;
//   }

//   // Vérifie si c'est un câble droit
//   for (int i = 0; i < 8; i++) {
//     if (ordreRecu[i] != (i == (ordreDroit[i] - 1) ? 1 : 0)) {
//       estDroit = false;
//     }
//   }
  
//   // Vérifie si c'est un câble croisé
//   for (int i = 0; i < 8; i++) {
//     if (ordreRecu[i] != (i == (ordreCroise[i] - 1) ? 1 : 0)) {
//       estCroise = false;
//     }
//   }
  
//   // Affiche le résultat
//   Serial.println("\nAnalyse du câble :");
//   if (estDroit && filsDetectes == 8) {
//     Serial.println("Câble DROIT détecté");
//   } else if (estCroise && filsDetectes == 8) {
//     Serial.println("Câble CROISÉ détecté");
//   } else {
//     Serial.println("Câble DÉFECTUEUX ou PARTIEL");
//     Serial.print("Nombre de fils détectés : ");
//     Serial.println(filsDetectes);
//     Serial.print("Ordre reçu : ");
//     for (int i = 0; i < 8; i++) {
//       Serial.print(ordreRecu[i]);
//       Serial.print(" ");
//     }
//     Serial.println();
//   }
// }


#include <Arduino.h>

// Classe pour gérer le testeur de câble RJ45
class CableRJ45Testeur {
private:
  // Broches de sortie pour les 8 fils (D0 à D7)
  int brochesSortie[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  // Broches d'entrée pour les 8 fils (A0 à A6 + D9)
  int brochesEntree[8] = {A0, A1, A2, A3, A4, A5, A6, 9};
  int ordreRecu[8]; // Stocke l'ordre reçu

  // Méthode privée pour analyser le câble
  void analyserCable(int filTeste) {
    // Ordre attendu pour câble droit (T568B)
    int ordreDroit[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // D0->A0, D1->A1, etc.
    // Ordre attendu pour câble croisé (T568B vers T568A)
    int ordreCroise[8] = {2, 5, 0, 3, 4, 1, 6, 7}; // D0->A2, D1->A5, etc.
    
    bool estDroit = true;
    bool estCroise = true;
    int filsDetectes = 0;
    
    // Compte les fils détectés
    for (int i = 0; i < 8; i++) {
      if (ordreRecu[i] == 1) filsDetectes++;
    }
    
    // Vérifie si le signal correspond à un câble droit
    if (filsDetectes != 1 || ordreRecu[ordreDroit[filTeste]] != 1) {
      estDroit = false;
    }
    
    // Vérifie si le signal correspond à un câble croisé
    if (filsDetectes != 1 || ordreRecu[ordreCroise[filTeste]] != 1) {
      estCroise = false;
    }
    
    // Affiche les résultats pour chaque fil testé
    Serial.print("Test du fil ");
    Serial.print(filTeste + 1);
    Serial.print(" : ");
    
    if (estDroit) {
      Serial.println("Connexion DROITE détectée");
    } else if (estCroise) {
      Serial.println("Connexion CROISÉE détectée");
    } else {
      Serial.println("DÉFECTUEUX ou NON CONNECTÉ");
      Serial.print("Fils détectés : ");
      Serial.println(filsDetectes);
      Serial.print("Ordre reçu : ");
      for (int i = 0; i < 8; i++) {
        Serial.print(ordreRecu[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
  }

public:
  // Constructeur
  CableRJ45Testeur() {
    // Initialisation des broches
    for (int i = 0; i < 8; i++) {
      pinMode(brochesSortie[i], OUTPUT);
      digitalWrite(brochesSortie[i], LOW);
      pinMode(brochesEntree[i], INPUT_PULLUP);
    }
  }

  // Méthode pour initialiser le testeur
  void initialiser() {
    Serial.begin(9600);
    while (!Serial); // Attend que le moniteur série soit prêt
    Serial.println("Testeur de câble RJ45 démarré (8 fils)...");
  }

  // Méthode pour exécuter le test
  void tester() {
    // Teste chaque fil en envoyant HIGH
    for (int i = 0; i < 8; i++) {
      digitalWrite(brochesSortie[i], HIGH);
      delay(100); // Attend que le signal se stabilise
      
      // Vérifie les entrées
      for (int j = 0; j < 8; j++) {
        ordreRecu[j] = digitalRead(brochesEntree[j]) == HIGH ? 1 : 0;
      }
      
      digitalWrite(brochesSortie[i], LOW);
      analyserCable(i);
      delay(100); // Pause entre les tests
    }
    
    Serial.println("Cycle terminé. Reprise...");
    delay(2000); // Pause avant de recommencer
  }
};

// Instance globale du testeur
CableRJ45Testeur testeur;

void setup() {
  testeur.initialiser();
}

void loop() {
  testeur.tester();
}
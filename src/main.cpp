/*
Titre:Titre du programme 
Auteur:Alseny Bah
Date:04/03/2025
Description:Une description du programme
Version:0.0.1 */



#include <Arduino.h>
 // Broches de sortie pour l'émetteur
int outputPins[] = {0, 1, 2, 3, 4, 5, 6, 7}; // D0 à D7

void setup() {
  Serial.begin(9600);
  while (!Serial); // Attend que le moniteur série soit prêt
  // Configure les broches comme sorties
  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], LOW); // Initialise à LOW
  }
  Serial.println("Émetteur prêt. Début des tests...");
}

void loop() {
  // Teste chaque fil en envoyant HIGH
  for (int i = 0; i < 8; i++) {
    digitalWrite(outputPins[i], HIGH);
    Serial.print("Envoi HIGH sur fil ");
    Serial.println(i + 1);
    delay(1000); // Attend 1 seconde
    digitalWrite(outputPins[i], LOW);
    delay(500); // Pause entre les tests
  }
  Serial.println("Cycle terminé. Reprise...");
  delay(2000); // Pause avant de recommencer
}  


//--------------------------------------------------------------------------------------------------------

// Ici vous avez le second code du microcontroleur MKR 1010 recepteur veuillez le commenté ou le décomenté pour seulement exécuté soit le code de l'émetteur ou du recepteur sur le bon microntroleur 

#include <Arduino.h>

// Broches utilisées pour le récepteur
int broches[] = {0, 1, 2, 3, 4, 5, 6, 7}; // D0 à D7 pour les 8 fils du RJ45
int ordreRecu[8]; // Stocke l'ordre reçu

void setup() {
  Serial.begin(9600);
  while (!Serial); // Attend que le moniteur série soit prêt
  
  // Configure toutes les broches comme entrées avec pull-up
  for (int i = 0; i < 8; i++) {
    pinMode(broches[i], INPUT_PULLUP);
  }
  
  Serial.println("Mode Récepteur activé. En attente de signaux...");
}

void loop() {
  bool signalDetecte = false;
  
  // Vérifie chaque broche pour détecter un signal
  for (int i = 0; i < 8; i++) {
    if (digitalRead(broches[i]) == HIGH) {
      ordreRecu[i] = 1;
      signalDetecte = true;
      Serial.print("Signal détecté sur broche ");
      Serial.println(i + 1);
      while (digitalRead(broches[i]) == HIGH); // Attend que le signal redescende
    } else {
      ordreRecu[i] = 0;
    }
  }
  
  // Analyse les résultats si un signal a été détecté
  if (signalDetecte) {
    analyserCable();
    delay(1000); // Pause avant le prochain cycle
  }
}

void analyserCable() {
  // Ordre attendu pour câble droit (T568B)
  int ordreDroit[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  // Ordre attendu pour câble croisé (T568B vers T568A)
  int ordreCroise[8] = {3, 6, 1, 4, 5, 2, 7, 8};
  
  bool estDroit = true;
  bool estCroise = true;
  int filsDetectes = 0;
  
  // Compte les fils détectés
  for (int i = 0; i < 8; i++) {
    if (ordreRecu[i] == 1) filsDetectes++;
  }

  // Vérifie si c'est un câble droit
  for (int i = 0; i < 8; i++) {
    if (ordreRecu[i] != (i == (ordreDroit[i] - 1) ? 1 : 0)) {
      estDroit = false;
    }
  }
  
  // Vérifie si c'est un câble croisé
  for (int i = 0; i < 8; i++) {
    if (ordreRecu[i] != (i == (ordreCroise[i] - 1) ? 1 : 0)) {
      estCroise = false;
    }
  }
  
  // Affiche le résultat
  Serial.println("\nAnalyse du câble :");
  if (estDroit && filsDetectes == 8) {
    Serial.println("Câble DROIT détecté");
  } else if (estCroise && filsDetectes == 8) {
    Serial.println("Câble CROISÉ détecté");
  } else {
    Serial.println("Câble DÉFECTUEUX ou PARTIEL");
    Serial.print("Nombre de fils détectés : ");
    Serial.println(filsDetectes);
    Serial.print("Ordre reçu : ");
    for (int i = 0; i < 8; i++) {
      Serial.print(ordreRecu[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
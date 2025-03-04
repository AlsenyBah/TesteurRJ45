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
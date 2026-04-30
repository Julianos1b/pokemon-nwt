#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pokemon.h"

//The Joystick Pins are Initialised here
#define VRX A0 //Vertical
#define VRY A1 //Horizontal
#define SW  2 //Middle Pressure

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Pokemon Initialisation
const int PokemonAmount = 4;

const int PokemonArraySize = PokemonAmount + 1;

Pokemon pokemons[PokemonArraySize] = {
    Pokemon(60, "Shiggy", 3, "water"),
    Pokemon(60, "Glumanda", 5, "fire"),
    Pokemon(60, "Bisasam", 4, "grass"),
    Pokemon(60, "Pikachu", 6, "electric"),
    Pokemon(60, "Evoli", 4, "grass")
};

int PokemonSelectedIndex = 0;

//Player Variables
int spielerHP = 100;

//Game Variables
enum Gamestates {
  START,
  AUSWAHLPROMPT,
  AUSWAHL,
  SPIEL,
  ENDE
};

const char* StartPromptText = "Druecke den Joystick um in den Auswahlbildschirm zu gelangen    ";
const char* AuswahlPromptText = "Bewege den Joystick nach Unten und Oben zur Auswahl    ";
int LcdPos = 0;
Gamestates Gamestate = START;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(SW, INPUT_PULLUP);
  lcd.setCursor(0, 0);
  lcd.print("Ready!");
  lcd.setCursor(0, 1);
  lcd.print("Hello World");
  delay(1500);
  lcd.clear();
}

void loop() {
  if (Gamestate == START) {
    StartPrompt();
  }
  if (Gamestate == AUSWAHLPROMPT) {
    AuswahlBildschirmPrompt();
  }
  if (Gamestate == AUSWAHL) {
    AuswahlBildschirm();
  }
}

void StartPrompt(){
    lcd.setCursor(0, 0);
    for (int i = 0; i < 16; i++) {
      lcd.print(StartPromptText[(LcdPos + i) % strlen(StartPromptText)]);
    }
    LcdPos++;
    delay(150);
    lcd.clear();
    if (digitalRead(SW)==0) {
    delay(1000);
    LcdPos = 0;
    Gamestate = AUSWAHLPROMPT;
    lcd.clear();
    delay(1000);
  }
}

void AuswahlBildschirmPrompt() {
    int x = analogRead(VRX);
    int y = analogRead(VRY);
    lcd.setCursor(0, 0);
    for (int i = 0; i < 16; i++) {
      lcd.print(AuswahlPromptText[(LcdPos + i) % strlen(AuswahlPromptText)]);
    }
    LcdPos++;
    delay(150);
    lcd.clear();

    if (y > 700) {
    lcd.clear();
    Gamestate = AUSWAHL;
    while(y > 700) {
      delay(1);
    }
    };
}

void AuswahlBildschirm()
{
  int y = analogRead(VRY);
  Serial.print("Hello World");
}
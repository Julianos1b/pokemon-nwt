#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pokemon.h"

// Player 1
bool p1Done = false;
int Player1Button = 8;
#define VRX1 A2
#define VRY1 A3
#define SW1 7

// Player 2
bool p2Done = false;
int Player2Button = 12;
#define VRX2 A0
#define VRY2 A1
#define SW2 10

LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x3F, 16, 2);

// Pokemon
const int PokemonAmount = 4;
const int PokemonArraySize = PokemonAmount + 1;

Pokemon pokemons[PokemonArraySize] = {
    Pokemon(60, "Shiggy", 3, "water", 600),
    Pokemon(60, "Glumanda", 5, "fire", 600),
    Pokemon(60, "Bisasam", 4, "grass", 600),
    Pokemon(60, "Pikachu", 6, "electric", 600),
    Pokemon(60, "Evoli", 4, "grass", 600)
};

int PokemonSelectedIndex = 0;
int PlayerAtTurn = 1;
Pokemon Players[2] = {
  Pokemon(0, "P1", 0, "", 0),
  Pokemon(0, "P2", 0, "", 0)
};

enum Gamestates {
  START,
  AUSWAHLPROMPT,
  AUSWAHL,
  WERTESETZEN,
  SPIEL,
  ENDE,
};

const char* StartPromptText = "Druecke den Joystick um in den Auswahlbildschirm zu gelangen    ";
const char* AuswahlPromptText = "Bewege den Joystick nach Unten und Oben zur Auswahl    ";
int LcdPos = 0;
Gamestates Gamestate = START;

void setup() {
  Serial.begin(9600);

  lcd1.init();
  lcd2.init();

  lcd1.backlight();
  lcd2.backlight();

  lcd1.clear();
  lcd2.clear();

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

  pinMode(Player1Button, INPUT_PULLUP);
  pinMode(Player2Button, INPUT_PULLUP);

  lcd1.print("Player 1");
  lcd2.print("Player 2");

  delay(1000);

  lcd1.clear();
  lcd2.clear();
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
  if (Gamestate == WERTESETZEN) {
    WerteSetzen();
  }
}

void StartPrompt(){
    lcd1.setCursor(0, 0);
    for (int i = 0; i < 16; i++) {
      lcd1.print(StartPromptText[(LcdPos + i) % strlen(StartPromptText)]);
    }
    LcdPos++;
    delay(150);
    lcd1.clear();
    if (digitalRead(SW1) == 0) {
    delay(1000);
    LcdPos = 0;
    Gamestate = AUSWAHLPROMPT;
    lcd1.clear();
    delay(1000);
  }
}

void AuswahlBildschirmPrompt() {
    int y = analogRead(VRY1);
    lcd1.setCursor(0, 0);
    for (int i = 0; i < 16; i++) {
      lcd1.print(AuswahlPromptText[(LcdPos + i) % strlen(AuswahlPromptText)]);
    }
    LcdPos++;
    delay(150);
    lcd1.clear();

    if (y > 700) {
    lcd1.clear();
    Gamestate = AUSWAHL;
    delay(200);
    };
}

void AuswahlBildschirm()
{
  static bool first = true;
  static bool movedDown = false;
  static bool movedUp = false;

  if (PlayerAtTurn == 1)
  {
    int y = analogRead(VRY1);

    if (first) {
      lcd1.clear();
      lcd1.setCursor(0, 0);
      lcd1.print("P1: ");
      lcd1.print(pokemons[PokemonSelectedIndex].name);

      lcd1.setCursor(0, 1);
      lcd1.print("Type:");
      lcd1.print(pokemons[PokemonSelectedIndex].type);

      first = false;
    }

    if (y > 700 && !movedDown) {
      if (PokemonSelectedIndex < PokemonAmount) {
        PokemonSelectedIndex++;
        first = true;
      }
      movedDown = true;
    }

    if (y < 300 && !movedUp) {
      if (PokemonSelectedIndex > 0) {
        PokemonSelectedIndex--;
        first = true;
      }
      movedUp = true;
    }

    if (y > 400 && y < 600) {
      movedDown = false;
      movedUp = false;
    }
  }

  if (PlayerAtTurn == 2)
  {
    int y = analogRead(VRY2);

    if (first) {
      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("P2: ");
      lcd2.print(pokemons[PokemonSelectedIndex].name);

      lcd2.setCursor(0, 1);
      lcd2.print("Type:");
      lcd2.print(pokemons[PokemonSelectedIndex].type);

      first = false;
    }

    if (y > 700 && !movedDown) {
      if (PokemonSelectedIndex < PokemonAmount) {
        PokemonSelectedIndex++;
        first = true;
      }
      movedDown = true;
    }

    if (y < 300 && !movedUp) {
      if (PokemonSelectedIndex > 0) {
        PokemonSelectedIndex--;
        first = true;
      }
      movedUp = true;
    }

    if (y > 400 && y < 600) {
      movedDown = false;
      movedUp = false;
    }
  }

  if (digitalRead(Player1Button) == LOW && PlayerAtTurn == 1) {
  Players[0] = pokemons[PokemonSelectedIndex];
  PlayerAtTurn = 2;
  PokemonSelectedIndex = 0;
  p1Done = true;
  first = true;
  delay(300);
  lcd1.clear();
  }

  if (digitalRead(Player2Button) == LOW && PlayerAtTurn == 2) {
  Players[1] = pokemons[PokemonSelectedIndex];
  p2Done = true;
  delay(300);
  lcd2.clear();
  }

  if (p1Done == true) {
    if(p2Done == true){
    Gamestate = WERTESETZEN;

    lcd1.clear();
    lcd2.clear();

    lcd1.print("READY!");
    lcd2.print("READY!");
    }
  }
}

void WerteSetzen() {
  for (int i = 1; i < 3; i++) {

    Serial.print("Healthpoints Player ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Players[i].healthpoints);

    Serial.print("Damage Player ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Players[i].damage);

    Serial.print("Name Player ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Players[i].name);

    Serial.print("Ability Energy Cost Player ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Players[i].ability_energy_cost);

    Serial.print("Type Player ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Players[i].type);
  }

  Gamestate = SPIEL;
}
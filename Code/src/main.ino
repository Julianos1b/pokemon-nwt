#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pokemon.h"
#include <Adafruit_NeoPixel.h>

//Neopixels
#define PIN1 3
#define PIN2 4

#define NUM_LEDS 8

Adafruit_NeoPixel strip1(NUM_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);

const byte Colors[10][3] = {
  {0, 0, 0},             //aus
  {40, 0, 90},          // dunkellila
  {80, 0, 80},         // lila
  {120, 0, 0},        // dunkelrot
  {180, 30, 0},      // richtung rot
  {200, 80, 0},     // dunkelorange
  {255, 120, 0},   // orange
  {80, 140, 0},   // hellgrün / gelbgrün
  {30, 110, 0},  // grün → gelbgrün
  {0, 80, 0}    // dunkelgrün   
};

// Player 1
bool p1Done = false;
#define VRX1 A2
#define VRY1 A3
#define SW1 8
double p1Advantage = 1.0;
bool p1stunned = false;
bool p1UltBlock = false;

// Player 2
bool p2Done = false;
#define VRX2 A0
#define VRY2 A1
#define SW2 12
double p2Advantage = 1.0;
bool p2stunned = false;
bool p2UltBlock = false;

bool P1HasBlocked = false;
bool P1HasAttacked = false;
bool P1HasUlted = false;
bool SW1Pressed = false;

bool P2HasBlocked = false;
bool P2HasAttacked = false;
bool P2HasUlted = false;
bool SW2Pressed = false;

double blockmove = 0.0;

LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

// Pokemon
const int PokemonAmount = 5;
const int PokemonArraySize = PokemonAmount + 1;

Pokemon pokemons[PokemonArraySize] = {
    Pokemon(600, 60, 10, "Splash", "Shiggy", "SuperShield", 6, "water"),
    Pokemon(600, 60, 10, "Burn", "Glumanda", "FireStorm", 4, "fire"),
    Pokemon(600, 60, 10, "Sting", "Bisasam", "Heal", 3, "grass"),
    Pokemon(600, 60, 10, "Shock", "Pikachu", "MegaShock", 4, "electric"),
    Pokemon(600, 60, 10, "Punch", "Evoli", "Heal", 5, "grass"),
    Pokemon(600, 500, 10, "KILL", "Admin", "KILL", 1, "Admin")
};

int PokemonSelectedIndex = 0;
int PlayerAtTurn = 1;
int MoveIndex = 1;

Pokemon Players[2] = {
  Pokemon(0, 0, 0, "", "P1", "", 0, ""),
  Pokemon(0, 0, 0, "", "P2", "", 0, ""),
};

int ultimates[2] = {1,2};

enum Gamestates {
  START,
  AUSWAHLPROMPT,
  AUSWAHL,
  WERTESETZEN,
  SPIEL,
  ENDE,
};


// LCD Text Stuff
const char* StartPromptText = "Druecke den Joystick um in den Auswahlbildschirm zu gelangen    ";
const char* AuswahlPromptText = "Bewege den Joystick nach Unten und Oben zur Auswahl    ";
int LcdPos = 0;
Gamestates Gamestate = START;

void setup() {
  Serial.begin(9600);

  strip1.begin();
  strip2.begin();

  strip1.show();
  strip2.show();

  lcd1.init();
  lcd2.init();

  lcd1.backlight();
  lcd2.backlight();

  lcd1.clear();
  lcd2.clear();

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

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
  if (Gamestate == SPIEL) {
    Game();
  }
  if (Gamestate == ENDE) {
    lcd1.clear();
    lcd2.clear();
    lcd1.print("ENDE");
    lcd2.print("ENDE");
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

  if (digitalRead(SW1) == LOW && PlayerAtTurn == 1) {
  Players[0] = pokemons[PokemonSelectedIndex];
  PlayerAtTurn = 2;
  PokemonSelectedIndex = 0;
  p1Done = true;
  first = true;
  delay(300);
  lcd1.clear();
  }

  if (digitalRead(SW2) == LOW && PlayerAtTurn == 2) {
  Players[1] = pokemons[PokemonSelectedIndex];
  p2Done = true;
  delay(300);
  lcd2.clear();
  }

  if (p1Done == true) {
    if(p2Done == true){
    Gamestate = WERTESETZEN;
    PlayerAtTurn = 1;

    lcd1.clear();
    lcd2.clear();

    lcd1.print("READY!");
    lcd2.print("READY!");
    }
  }
}

void WerteSetzen() {
  for (int i = 0; i < 2; i++) {

    Serial.println("================================");

    Serial.print("Player ");
    Serial.println(i + 1);

    Serial.print("Name: ");
    Serial.println(Players[i].name);

    Serial.print("Type: ");
    Serial.println(Players[i].type);

    Serial.print("HP: ");
    Serial.println(Players[i].healthpoints);

    Serial.print("Attack: ");
    Serial.println(Players[i].attackname);

    Serial.print("Attack Damage: ");
    Serial.println(Players[i].damage);

    Serial.print("Ultimate: ");
    Serial.println(Players[i].ultimatename);

    Serial.print("Energy Cost: ");
    Serial.println(Players[i].ability_energy_cost);

    Serial.println("================================");
  }

  SetRelations();
  Gamestate = SPIEL;
}

void Game()
{
  for(int i = 0; i < 2; i++) {
    if(Players[i].healthpoints <= 0)
    {
      Gamestate = ENDE;
    }
  }

  static bool first = true;
  static bool movedDown = false;
  static bool movedUp = false;

  if (PlayerAtTurn == 1)
  {
      if(p1stunned == true) {
      lcd1.clear();
      lcd1.print("du bist");
      lcd1.setCursor(0, 1);
      lcd1.print("geschockt !");
      strip1.clear();
      strip2.clear();
      strip1.show();
      strip2.show();
      lcd2.clear();
      lcd2.print("du bist dran");
      PlayerAtTurn = 2;
      Players[0].energy += 2;
      MoveIndex = 1;
      first = true;
      delay(800);
      lcd1.clear();
      lcd1.print("P1 ist dran !");
      p1stunned = false;
    }
    int y = analogRead(VRY1);

    if (first) {
      lcd1.clear();
      lcd1.setCursor(0, 0);
      switch(MoveIndex) {
        case 1:
        lcd1.print("Block");
        NeopixelIndex(MoveIndex);
        break;
        case 2:
        lcd1.print("Attack");
        NeopixelIndex(MoveIndex);
        break;
        case 3:
        lcd1.print("Ult");
        lcd1.setCursor(0, 1);
        lcd1.print("kosten : ");
        lcd1.print(Players[0].ability_energy_cost);
        NeopixelIndex(MoveIndex);
        break;
        case 4:
        lcd1.print("Skip");
        NeopixelIndex(MoveIndex);
        break;
        case 5:
        lcd1.print("Deine HP: ");
        lcd1.setCursor(0, 1);
        lcd1.print(Players[0].healthpoints);
        ColorNeopixel(Players[0].healthpoints);
        break;
        case 6:
        lcd1.print("Gegner HP: ");
        lcd1.setCursor(0, 1);
        lcd1.print(Players[1].healthpoints);
        ColorNeopixel(Players[1].healthpoints);
        break;
        case 7:
        lcd1.print("Deine Energie: ");
        lcd1.setCursor(0, 1);
        lcd1.print(Players[0].energy);
        NeopixelIndex(MoveIndex);
        break;
        case 8:
        lcd1.print("Gegner Energie: ");
        lcd1.setCursor(0, 1);
        lcd1.print(Players[1].energy);
        NeopixelIndex(MoveIndex);
        break;
        default:
        break;
      } 

      first = false;
    }

    if (y > 700 && !movedDown) {
      if (MoveIndex <= 7) {
        MoveIndex++;
        first = true;
      }
      movedDown = true;
    }

    if (y < 300 && !movedUp) {
      if (MoveIndex >= 2) {
        MoveIndex--;
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
    if(p2stunned == true) {
      lcd2.clear();
      lcd2.print("du bist");
      lcd2.setCursor(0, 1);
      lcd2.print("geschockt !");
      strip1.clear();
      strip2.clear();
      strip1.show();
      strip2.show();
      lcd1.clear();
      lcd1.print("du bist dran");
      PlayerAtTurn = 1;
      Players[1].energy += 2;
      MoveIndex = 1;
      first = true;
      delay(800);
      lcd2.clear();
      lcd2.print("P1 ist dran !");
      p2stunned = false;
    }
    int y = analogRead(VRY2);

    if (first) {
      lcd2.clear();
      lcd2.setCursor(0, 0);
      switch(MoveIndex) {
        case 1:
        lcd2.print("Block");
        NeopixelIndex(MoveIndex);
        break;
        case 2:
        lcd2.print("Attack");
        NeopixelIndex(MoveIndex);
        break;
        case 3:
        lcd2.print("Ult");
        lcd2.setCursor(0, 1);
        lcd2.print("kosten : ");
        lcd2.print(Players[1].ability_energy_cost);
        NeopixelIndex(MoveIndex);
        break;
        case 4:
        lcd2.print("Skip");
        NeopixelIndex(MoveIndex);
        break;
        case 5:
        lcd2.print("Deine HP: ");
        lcd2.setCursor(0, 1);
        lcd2.print(Players[1].healthpoints);
        ColorNeopixel(Players[1].healthpoints);
        break;
        case 6:
        lcd2.print("Gegner HP: ");
        lcd2.setCursor(0, 1);
        lcd2.print(Players[0].healthpoints);
        ColorNeopixel(Players[0].healthpoints);
        break;
        case 7:
        lcd2.print("Deine Energie: ");
        lcd2.setCursor(0, 1);
        lcd2.print(Players[1].energy);
        NeopixelIndex(MoveIndex);
        break;
        case 8:
        lcd2.print("Gegner Energie: ");
        lcd2.setCursor(0, 1);
        lcd2.print(Players[0].energy);
        NeopixelIndex(MoveIndex);
        break;
        default:
        lcd2.print("Block");
        break;
      }

      first = false;
    }

    if (y > 700 && !movedDown) {
      if (MoveIndex <= 7) {
        MoveIndex++;
        first = true;
      }
      movedDown = true;
    }

    if (y < 300 && !movedUp) {
      if (MoveIndex >= 2) {
        MoveIndex--;
        first = true;
      }
      movedUp = true;
    }

    if (y > 400 && y < 600) {
      movedDown = false;
      movedUp = false;
    }
  }

  if (digitalRead(SW1) == HIGH && PlayerAtTurn == 1){
    SW1Pressed = false;
    delay(50);
  }

  if (digitalRead(SW1) == LOW && PlayerAtTurn == 1) {
    if(SW1Pressed == false) {
    SW1Pressed = true;
    switch(MoveIndex) {
      case 1:
      if(P1HasBlocked == false){
      if(Players[0].energy >= 1) {
      blockmove = 0.2;
      lcd2.clear();
      lcd2.print("P1 blockt");
        Players[0].energy -= 1;
      P1HasBlocked = true;
      lcd1.clear();
      lcd1.print("Du Blockst");
      }
      } else {
        lcd1.clear();
        lcd1.print("Fehlgeschlagen");
      }
      break;
      case 2:
      if(P1HasAttacked == false)
      {
      if(Players[0].energy >= 2) {
      lcd2.clear();
      lcd2.print("P1 attackiert");
      Players[0].energy -= 2;
      P1HasAttacked = true;
      lcd1.clear();
      lcd1.print("Angriff !");
      BlockingGame();
      }
      } else {
        lcd1.clear();
        lcd1.print("Fehlgeschlagen");
      }
      break;
      case 3:
      if(P1HasUlted == false) 
      {
      if(Players[0].energy >= Players[0].ability_energy_cost) {
      lcd2.clear();
      Players[0].energy -= Players[0].ability_energy_cost;
      lcd1.clear();
      lcd1.print("ULTIMATE !!!");
      Ultimate();
      P1HasUlted = true;
      }
      } else {
        lcd1.clear();
        lcd1.print("Fehlgesschlagen");
      }
      break;
      case 4:
      strip1.clear();
      strip2.clear();
      strip1.show();
      strip2.show();
      lcd2.clear();
      lcd2.print("du bist dran");
      PlayerAtTurn = 2;
      Players[0].energy += 2;
      MoveIndex = 1;
      first = true;
      delay(300);
      lcd1.clear();
      lcd1.print("P2 ist dran !");
      resetTurnFlags();
      break; 
      default:
      break;
    }
  }
  }

  if (digitalRead(SW2) == HIGH && PlayerAtTurn == 2){
    SW2Pressed = false;
    delay(50);
  }

  if (digitalRead(SW2) == LOW && PlayerAtTurn == 2) {
    if(SW2Pressed == false) {
      SW2Pressed = true;
    switch(MoveIndex) {
      case 1:
      if(P2HasBlocked == false){
      if(Players[1].energy >= 1) {
      blockmove = 0.2;
      lcd1.clear();
      lcd1.print("P2 blockt");
        Players[1].energy -= 1;
      P2HasBlocked = true;
      lcd2.clear();
      lcd2.print("Du Blockst");
      }
      } else {
      lcd2.clear();
      lcd2.print("Fehlgeschlagen");
      }
      break;
      case 2:
      if(P2HasAttacked == false)
      {
      if(Players[1].energy >= 2) {
      lcd1.clear();
      lcd1.print("P2 attackiert");
        Players[1].energy -= 2;
      lcd2.clear();
      lcd2.print("Angriff !");
      BlockingGame();
      P2HasAttacked = true;
      }
      } else {
        lcd2.clear();
        lcd2.print("Fehlgeschlagen");
      }
      break;
      case 3:
      if(P2HasUlted == false){
      if(Players[1].energy >= Players[1].ability_energy_cost) {
      lcd1.clear();
        Players[1].energy -= Players[1].ability_energy_cost;
      lcd2.clear();
      lcd2.print("ULTIMATE !!!");
      Ultimate();
      P2HasUlted = true;
      }
      } else {
      lcd2.clear();
      lcd2.print("Fehlgeschlagen");
      }
      break;
      case 4:
      strip1.clear();
      strip2.clear();
      strip1.show();
      strip2.show();
      lcd1.clear();
      lcd1.print("du bist dran");
      PlayerAtTurn = 1;
      Players[1].energy += 2;
      MoveIndex = 1;
      first = true;
      delay(300);
      lcd2.clear();
      lcd2.print("P1 ist dran !");
      break; 
      default:
      break;
    }
  }
}
}

void resetTurnFlags() {
  P1HasBlocked = false;
  P1HasAttacked = false;
  P1HasUlted = false;

  P2HasBlocked = false;
  P2HasAttacked = false;
  P2HasUlted = false;
}

void ColorNeopixel(int Number) {
  if(PlayerAtTurn == 1) {
  strip1.clear();
  int Full = Number / 100;
  int Last = Number % 100;
  int ArrayNumber = Last / 10;
  for(int i = 0; i < Full; i++) {
    strip1.setPixelColor(i, strip1.Color(0, 255, 0));
  }
  strip1.setPixelColor(Full , strip1.Color(
                                                Colors[ArrayNumber][0], 
                                                Colors[ArrayNumber][1], 
                                                Colors[ArrayNumber][2]
                                              ));
  strip1.show();
  }
  if(PlayerAtTurn == 2) {
  strip2.clear();
  int Full = Number / 100;
  int Last = Number % 100;
  int ArrayNumber = Last / 10;
  for(int i = 0; i < Full; i++) {
    strip2.setPixelColor(i, strip2.Color(0, 255, 0));
  }
  strip2.setPixelColor(Full , strip2.Color(
                                                Colors[ArrayNumber][0], 
                                                Colors[ArrayNumber][1], 
                                                Colors[ArrayNumber][2]
                                              ));
  strip2.show();
  }
}

void NeopixelIndex(int Index) {
  if(PlayerAtTurn == 1) {
  strip1.clear();
  int invertedIndex = NUM_LEDS - Index;
  strip1.setPixelColor(invertedIndex, strip1.Color(0, 255, 0));
  strip1.show();
  }
  if(PlayerAtTurn == 2) {
  strip2.clear();
  int invertedIndex = NUM_LEDS - Index;
  strip2.setPixelColor(invertedIndex, strip2.Color(0, 255, 0));
  strip2.show();
  }
}

void BlockingGame() {
  if(PlayerAtTurn == 1) {
    double damageoff = 1;
    strip2.clear();
    strip2.setPixelColor(1, strip2.Color(0, 0, 255));
    strip2.show();
    delay(1000);
  for(int i = 8; i >= 0; i--) {
    strip2.setPixelColor(i, strip2.Color(0, 255, 0));
    delay(500);
    if(digitalRead(SW2) == 0) {
      switch(i) {
        case 1:
        damageoff -= 0.2 + blockmove;
        blockmove = 0.0;
        strip2.clear();
        break;
        case 2: 
        damageoff -= 0.1 + blockmove;
        blockmove = 0.0;
        strip2.clear();
        break;
        default:
        damageoff -= 0.0 + blockmove;
        blockmove = 0.0;
        strip2.clear();
        break;
      }
      break;
    }
    strip2.show();
  }
  Players[1].healthpoints -= Players[0].damage * p1Advantage * damageoff;
  strip2.clear();
  strip2.show();
  lcd2.clear();
  lcd2.print("Attacke fertig !");
  lcd2.setCursor(0, 1);
  lcd2.print("Schaden : ");
  lcd2.print(Players[0].damage * p1Advantage * damageoff);
  lcd1.clear();
  lcd1.print("Attacke ");
  lcd1.setCursor(0, 1);
  lcd1.print("erfolgreich !");
  }
    if(PlayerAtTurn == 2) {
    double damageoff = 1;
    strip1.clear();
    strip1.setPixelColor(1, strip1.Color(0, 0, 255));
    strip1.show();
    delay(1000);
  for(int i = 8; i >= 0; i--) {
    strip1.setPixelColor(i, strip1.Color(0, 255, 0));
    delay(500);
    if(digitalRead(SW1) == 0) {
      switch(i) {
        case 1:
        damageoff -= 0.2 + blockmove;
        blockmove = 0.0;
        strip1.clear();
        break;
        case 2: 
        damageoff -= 0.1 + blockmove;
        blockmove = 0.0;
        strip1.clear();
        break;
        default:
        damageoff -= 0.0 + blockmove;
        blockmove = 0.0;
        strip1.clear();
        break;
      }
      break;
    }
    strip1.show();
  }
  if(p1UltBlock == false){
    Players[0].healthpoints -= Players[1].damage * p2Advantage * damageoff;
    strip1.clear();
    strip1.show();
    lcd1.clear();
    lcd1.print("Attacke fertig !");
    lcd1.setCursor(0, 1);
    lcd1.print("Schaden : ");
    lcd1.print(Players[1].damage * p2Advantage * damageoff);
    lcd2.clear();
    lcd2.print("Attacke");
    lcd2.setCursor(0, 1);
    lcd2.print("erfolgreich !");
  }
    if(p1UltBlock == true){
    strip1.clear();
    strip1.show();
    lcd1.clear();
    lcd1.print("Attacke geblockt");
    lcd2.clear();
    lcd2.print("Attacke");
    lcd2.setCursor(0, 1);
    lcd2.print("war misserfolg !");
    p1UltBlock = false;
  }
  }
}

void SetRelations() {
  if(Players[0].type == "grass"){
    if(Players[1].type == "electric"){
      p1Advantage = 1.2;
      p2Advantage = 0.8;
  }
  }
    if(Players[0].type == "electric"){
    if(Players[1].type == "water"){
      p1Advantage = 1.2;
      p2Advantage = 0.8;
  }
  }
    if(Players[0].type == "water"){
    if(Players[1].type == "fire"){
      p1Advantage = 1.2;
      p2Advantage = 0.8;
  }
  }
    if(Players[0].type == "fire"){
    if(Players[1].type == "grass"){
      p1Advantage = 1.2;
      p2Advantage = 0.8;
  }

  }
    if(Players[1].type == "grass"){
    if(Players[0].type == "electric"){
      p2Advantage = 1.2;
      p1Advantage = 0.8;
  }
  }
    if(Players[1].type == "electric"){
    if(Players[0].type == "water"){
      p2Advantage = 1.2;
      p1Advantage = 0.8;
  }
  }
    if(Players[1].type == "water"){
    if(Players[0].type == "fire"){
      p2Advantage = 1.2;
      p1Advantage = 0.8;
  }
  }
    if(Players[1].type == "fire"){
    if(Players[0].type == "grass"){
      p2Advantage = 1.2;
      p1Advantage = 0.8;
  }
  }
}

void Ultimate() {
  if(PlayerAtTurn == 1)
  {
    lcd2.clear();
    if(Players[0].type == "grass")
    {
      Players[0].healthpoints += Players[0].ability_energy_cost * 20;
      lcd2.print("Player 1");
      lcd2.setCursor(0, 1);
      lcd2.print("Hat sich geheilt");
    }
    if(Players[0].type == "electric")
    {
      p2stunned = true;
      lcd2.print("Player 1");
      lcd2.setCursor(0, 1);
      lcd2.print("schockt dich");
    }
    if(Players[0].type == "water")
    {
      p1UltBlock = true;
      lcd2.print("Player 1");
      lcd2.setCursor(0, 1);
      lcd2.print("blockt alles");
    }
      if(Players[0].type == "fire")
    {
      Players[1].healthpoints -= Players[0].ability_energy_cost * 20;
      lcd2.print("Player 1");
      lcd2.setCursor(0, 1);
      lcd2.print("verbrennt dich");
    }
  }
  if(PlayerAtTurn == 2)
  {
    lcd1.clear();
    if(Players[1].type == "grass") 
    {
      Players[1].healthpoints += Players[1].ability_energy_cost * 20;
      lcd1.print("Player 2");
      lcd1.setCursor(0, 1);
      lcd1.print("Hat sich geheilt");
    }
    if(Players[1].type == "electric") 
    {
      p2stunned = true;
      lcd1.print("Player 2");
      lcd1.setCursor(0, 1);
      lcd1.print("schockt dich");
    }
    if(Players[1].type == "water") 
    {
      p1UltBlock = true;
      lcd1.print("Player 2");
      lcd1.setCursor(0, 1);
      lcd1.print("blockt alles");
    }
    if(Players[1].type == "fire") 
    {
      Players[0].healthpoints -= Players[1].ability_energy_cost * 20;
      lcd1.print("Player 2");
      lcd1.setCursor(0, 1);
      lcd1.print("verbrennt dich");
    }
  }
}
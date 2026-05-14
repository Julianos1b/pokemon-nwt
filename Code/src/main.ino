#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pokemon.h"

// Player 1
bool p1Done = false;
#define VRX1 A2
#define VRY1 A3
#define SW1 8

// Player 2
bool p2Done = false;
#define VRX2 A0
#define VRY2 A1
#define SW2 12

LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

// Pokemon
const int PokemonAmount = 4;
const int PokemonArraySize = PokemonAmount + 1;

Pokemon pokemons[PokemonArraySize] = {
    Pokemon(60, "Shiggy", "splash", 3, "shield", "SuperShield", "water", 600),
    Pokemon(60, "Glumanda", "burn",  5, "hurt", "FireStorm", "fire", 600),
    Pokemon(60, "Bisasam", "sting", 4, "heal", "Heal", "grass", 600),
    Pokemon(60, "Pikachu", "shock", 6, "shock", "MegaShock", "electric", 600),
    Pokemon(60, "Evoli", "Punch", 4, "heal", "Heal", "grass", 600)
};

int PokemonSelectedIndex = 0;
int PlayerAtTurn = 1;
Pokemon Players[2] = {
  Pokemon(0, "P1", "", 1, "", "", "", 0),
  Pokemon(0, "P2", "", 1, "", "", "", 0)
};

//Game Values

enum MoveChoice { 
  BLOCK, 
  ATTACK, 
  ULTIMATE, 
  SKIP 
};

int currentPlayer = 0;
int moveIndex = 0;

MoveChoice moves[4] = {
  BLOCK, 
  ATTACK, 
  ULTIMATE, 
  SKIP
};

MoveChoice selectedMove = SKIP;

bool moveChosen = false;

enum Gamestates {
  START,
  AUSWAHLPROMPT,
  AUSWAHL,
  WERTESETZEN,
  SPIEL,
  ENDE,
};

const char* moveName(MoveChoice m);
void applyMove(Pokemon &attacker, Pokemon &defender, MoveChoice move);

// LCD Text Stuff
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

    Serial.print("Ultimate Type: ");
    Serial.println(Players[i].ultimatetype);

    Serial.print("Energy Cost: ");
    Serial.println(Players[i].ability_energy_cost);

    Serial.println("================================");
  }

  Gamestate = SPIEL;
}

void Game() {

  Pokemon &p1 = Players[0];
  Pokemon &p2 = Players[1];

  int y;

  if (currentPlayer == 0) {
    y = analogRead(VRY1);

    lcd1.setCursor(0, 0);
    lcd1.print("P1 TURN        ");

    lcd1.setCursor(0, 1);
    lcd1.print(moveName(moves[moveIndex]));
    lcd1.print("            ");

  } else {
    y = analogRead(VRY2);

    lcd2.setCursor(0, 0);
    lcd2.print("P2 TURN        ");

    lcd2.setCursor(0, 1);
    lcd2.print(moveName(moves[moveIndex]));
    lcd2.print("            ");
  }

  static bool movedUp = false;
  static bool movedDown = false;

  if (y > 700 && !movedDown) {
    if (moveIndex < 3) moveIndex++;
    movedDown = true;
  }

  if (y < 300 && !movedUp) {
    if (moveIndex > 0) moveIndex--;
    movedUp = true;
  }

  if (y > 400 && y < 600) {
    movedUp = false;
    movedDown = false;
  }

  if ((currentPlayer == 0 && digitalRead(SW1) == LOW) ||
      (currentPlayer == 1 && digitalRead(SW2) == LOW)) {

    selectedMove = moves[moveIndex];

    if (currentPlayer == 0) {
      applyMove(p1, p2, selectedMove);
      currentPlayer = 1;
    } else {
      applyMove(p2, p1, selectedMove);
      currentPlayer = 0;
    }

    moveIndex = 0;

    delay(300);
  }

  if (p1.healthpoints <= 0) {
    lcd1.clear();
    lcd1.print("P2 WINS!");
    Gamestate = ENDE;
  }

  if (p2.healthpoints <= 0) {
    lcd1.clear();
    lcd1.print("P1 WINS!");
    Gamestate = ENDE;
  }
}
const char* moveName(MoveChoice m) {
  switch (m) {
    case BLOCK: return "BLOCK";
    case ATTACK: return "ATTACK";
    case ULTIMATE: return "ULTIMATE";
    case SKIP: return "SKIP";
  }
  return "";
}

void applyMove(Pokemon &attacker, Pokemon &defender, MoveChoice move) {

  if (move == ATTACK) {
    defender.healthpoints -= attacker.damage;
  }

  if (move == ULTIMATE) {
    defender.healthpoints -= attacker.damage * 2;
  }

  if (move == BLOCK) {

  }

  if (move == SKIP) {

  }

  if (defender.healthpoints < 0) {
    defender.healthpoints = 0;
  }
}
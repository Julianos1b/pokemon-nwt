#include "core/pokemon.h"

Pokemon pokemons[5] = {
    Pokemon(60, "Shiggy", 3, "water"),
    Pokemon(60, "Glumanda", 5, "fire"),
    Pokemon(60, "Bisasam", 4, "grass"),
    Pokemon(60, "Pikachu", 6, "electric"),
    Pokemon(60, "Evoli", 4, "grass")
};

int spielerHP = 100;

void setup() {
  Serial.begin(9600);

  spielerHP -= shiggy.damage;

  Serial.println(spielerHP);
}

void loop() {}
#ifndef POKEMON_H
#define POKEMON_H

class Pokemon {
public:
    int healthpoints;
    int damage;
    const char* name;
    int ability_energy_cost;
    const char* type;

    Pokemon(int d, const char* n, int a, const char* t, int hp) {
        healthpoints = hp;
        damage = d;
        name = n;
        ability_energy_cost = a;
        type = t;
    }
};

#endif
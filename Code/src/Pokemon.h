#ifndef POKEMON_H
#define POKEMON_H

class Pokemon {
public:
    int healthpoints;
    int damage;
    String name;
    int ability_energy_cost;
    String type;

    Pokemon(int d, String n, int a, String t, int hp) {
        healthpoints = hp;
        damage = d;
        name = n;
        ability_energy_cost = a;
        type = t;
    }
};

#endif
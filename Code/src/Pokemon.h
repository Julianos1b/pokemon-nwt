class Pokemon {
public:
    int healthpoints;
    int damage;
    int energy;
    const char* attackname;
    const char* name;
    const char* ultimatename;
    int ability_energy_cost;
    const char* type;

    Pokemon(int hp,
            int dm,
            int en,
            const char* at,
            const char* na,
            const char* ul,
            int ab,
            const char* ty
            )
    {
        this->healthpoints = hp;
        this->damage = dm;
        this->energy = en;
        this->attackname = at;
        this->name = na;
        this->ultimatename = ul;
        this->ability_energy_cost = ab;
        this->type = ty;
    }
};
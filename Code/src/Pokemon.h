class Pokemon {
public:
    int healthpoints;
    int damage;
    int energy;
    String attackname;
    String name;
    String ultimatename;
    int ability_energy_cost;
    String type;

    Pokemon(int hp,
            int dm,
            int en,
            String at,
            String na,
            String ul,
            int ab,
            String ty
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
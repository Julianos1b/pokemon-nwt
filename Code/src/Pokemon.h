class Pokemon {
public:
    int healthpoints;
    int damage;
    int energy;
    const char* attackname;
    const char* name;
    const char* ultimatename;
    const char* ultimatetype;
    int ability_energy_cost;
    const char* type;

    Pokemon(int hp,
            int en,
            const char* name,
            const char* type,
            int damage,
            const char* attackname,
            const char* ultimatename,
            const char* ultimatetype,
            int energy)
    {
        this->energy = en;
        this->healthpoints = hp;
        this->name = name;
        this->type = type;
        this->damage = damage;
        this->attackname = attackname;
        this->ultimatename = ultimatename;
        this->ultimatetype = ultimatetype;
        this->ability_energy_cost = energy;
    }
};
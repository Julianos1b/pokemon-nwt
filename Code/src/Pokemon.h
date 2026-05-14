class Pokemon {
public:
    int healthpoints;
    int damage;
    const char* attackname;
    const char* name;
    const char* ultimatename;
    const char* ultimatetype;
    int ability_energy_cost;
    const char* type;

    Pokemon(int hp,
            const char* name,
            const char* type,
            int damage,
            const char* attackname,
            const char* ultimatename,
            const char* ultimatetype,
            int energy)
    {
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
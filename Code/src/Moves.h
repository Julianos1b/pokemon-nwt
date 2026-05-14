class Pokemon {
public:
    const char* block;
    const char* attack;
    const char* ult;
    const char* skip;

    Pokemon(
        const char* bl,
        const char* at,
        const char* ul,
        const char* sk
    )
    {
        this->block = bl;
        this->attack = at;
        this->ult = ul;
        this->skip = sk;
    }
};
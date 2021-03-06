#define GRENADES_MAX PLAYERS_MAX*3
//each can throw all their grenades all at once

struct Grenade {
    unsigned char active;
    unsigned char owner;
    float fuse_length;
    float created;
    struct Position pos;
    struct Velocity velocity;
};

struct Grenade* grenade_add(void);
void grenade_update(float dt);

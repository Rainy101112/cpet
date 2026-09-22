#ifndef INCLUDE_PET_H_
#define INCLUDE_PET_H_

#include <regex>
#include <stdbool.h>

struct pet_status {
    int mood;   // Pet mood. Sad / Peaceful / Happy
    int hunger; // Hunger. Hungry / Normal / Full
    bool is_sleeping;
    bool is_playing;
};

struct pet {
    struct pet_status *status;
    int age;    // Age of the pet. (day)
    int exp;    // Experience
    int level;  // Pet level
    char *name;
};

struct inventory {
    int food;   // Food in inventory. (g)
    int soap;   // Soap in inventory. (piece)
};

struct player {
    struct inventory *inventory;
    int money;  // Money. (dollar $)
    char *name;
};

typedef struct pet_status pet_status_t;
typedef struct pet pet_t;
typedef struct inventory inventory_t;
typedef struct player player_t;

#endif // INCLUDE_PET_H_

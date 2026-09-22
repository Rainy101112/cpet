#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>

#include "pet.h"
#include "ini.h"

player_t *create_player(void);
pet_t *create_pet(void);
void free_player(player_t *player);
void free_pet(pet_t *pet);

player_t *create_player(void) {
    player_t *player = (player_t *)malloc(sizeof(player_t));
    if (player) {
        player->inventory = (inventory_t *)malloc(sizeof(inventory_t));

        if (player->inventory) {
            player->inventory->food = 0;
            player->inventory->soap = 0;

            player->money = 0;

            player->name = NULL;
        }

        return player;
    }

    return NULL;
}

pet_t *create_pet(void) {
    pet_t *pet = (pet_t *)malloc(sizeof(pet_t));
    if (pet) {
        pet->status = (pet_status_t *)malloc(sizeof(pet_status_t));
        if (pet->status) {
            pet->status->hunger = 100;
            pet->status->mood = 100;
            pet->status->is_sleeping = false;
            pet->status->is_playing = false;

            pet->age = 0;
            pet->exp = 0;
            pet->level = 0;

            pet->name = NULL;
        }

        return pet;
    }

    return NULL;
}

void free_player(player_t *player) {
    if (player) {
        if (player->inventory) {
            free(player->inventory);
            free(player);
            return;
        }
        free(player);
        return;
    }

    perror("Failed to free memory");
}

void free_pet(pet_t *pet) {
    if (pet) {
        if (pet->status) {
            free(pet->status);
            free(pet);
            return;
        }
        free(pet);
        return;
    }

    perror("Failed to free memory");
}

int main() {
    printf("Welcome back, dear pet owner!\n");



    return 0;
}

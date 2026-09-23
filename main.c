#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "pet.h"
#include "ini.h"

char player_name[32];
char pet_name[32];

int main() {
    printf("Welcome back, dear pet owner!\n");

    player_t *player = create_player(); if (!player) return 0;
    pet_t *pet = create_pet(); if (!pet) return 0;

    printf("Enter your name: ");
    if (fgets(player_name, sizeof(player_name), stdin)) {
        size_t len = strlen(player_name);
        if (len > 0 && player_name[len - 1] == '\n') player_name[len - 1] = '\0';   // Remove '\n'
        set_player_name(player, player_name);
    } else {set_player_name(player, "player");} // Default name

    printf("Enter your pet's name: ");
    if (fgets(pet_name, sizeof(pet_name), stdin)) {
        size_t len = strlen(pet_name);
        if (len > 0 && pet_name[len - 1] == '\n') pet_name[len - 1] = '\0'; // Remove '\n'
        set_pet_name(pet, pet_name);
    } else {set_pet_name(pet, "pet");}  // Default name

    printf("%s is %s's pet now!\n", pet->name, player->name);

    free_player(player);
    free_pet(pet);

    return 0;
}
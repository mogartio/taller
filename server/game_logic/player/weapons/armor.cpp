#include "armor.h"

#include <cmath>
#include <memory>
#include <utility>

#include "../player.h"

Chest::Chest(Stage& stage): Weapon(stage, 1, 20, CHEST) {}

void Chest::shoot(int damage, bool) {
    // Recibe daño
    ammo -= damage;
    if (ammo == 0) {
        // Caso de que se quede sin daño posible, desaparece la armadura
        player->pick_chest(std::make_unique<Unarmed>(stage));
    }
}


Helmet::Helmet(Stage& stage, int id): Weapon(stage, 1, 20, id) {}

void Helmet::shoot(int damage, bool) {
    // Recibe daño
    ammo -= damage;
    if (ammo == 0) {
        // Caso de que se quede sin daño posible, desaparece el casco
        player->pick_helmet(std::make_unique<Unarmed>(stage));
    }
}

#include "player.h"

#include <cstdlib>
#include <memory>

#include "../../../common/messages/generic_msg.h"
#include "weapons/pistols.h"
#include "weapons/weapon.h"
#include "weapons/armor/armor.h"

using namespace ActionsId;
Player::Player(Coordinate& initial_position, int id, const std::string& name,
               const PlayerObserver& obs):
        id(id),
        is_alive(true),
        name(name),
        should_notify(true),
        initial_position(initial_position) {
    attach(obs);
}

void Player::init_for_stage(Stage* stage) {
    this->stage = stage;
    this->position = std::make_unique<PlayerPosition>(initial_position, *this, *stage);
    is_alive = true;
    current_actions.clear();
    notify_moved();  // los notify hacen que se broadcasteen mensajes
}

void Player::pick_weapon(std::shared_ptr<Weapon> new_weapon) {
    weapon = std::move(new_weapon);
    weapon->set_player(this);
    notify_picked_weapon();

}

void Player::pick_chest(std::shared_ptr<Weapon> new_chest) {
    chest = std::move(new_chest);
    chest->set_player(this);
    notify_picked_chest();
}

void Player::pick_helmet(std::shared_ptr<Weapon> new_helmet) {
    helmet = std::move(new_helmet);
    helmet->set_player(this);
    notify_picked_helmet();
}

void Player::unarm_self() { 
    pick_weapon(std::make_unique<Unarmed>(*stage)); 
    pick_chest(std::make_unique<Unarmed>(*stage));
    pick_helmet(std::make_unique<Unarmed>(*stage));
}

Coordinate Player::get_position() { return position->get_position(); }

int Player::get_facing_direction() { return position->get_facing_direction(); }


int Player::get_id() { return id; }

void Player::occupy(Coordinate& coordinate) { position->occupy(coordinate); }

std::vector<Coordinate> Player::get_occupied() { return position->get_occupied(); }

void Player::add_action(int& command) { current_actions.insert(command); }

void Player::remove_action(int& command) {
    current_actions.erase(command);
    if (command == JUMP) {  // esto va a terminar siendo un diccionario de funciones probablemente
        position->released_jump();
    }
    if (command == 6) {
        position->stop_aiming_up();
    }
    if (command == SHOOT) {
        if (weapon == nullptr) {
            return;
        }
        weapon->stop_shooting();
    }
    if (command == THROW_WEAPON) {
        if (!weapon->is_unarmed()) {
            weapon->finish_throw(position->get_aiming_direction(), position->is_aiming_up(),
                                 std::move(weapon));
            // notify_dropped_weapon(weapon_id);
            pick_weapon(std::make_unique<Unarmed>(
                    *stage));  // tecnicamente nunca habria que avisar que se droppeo algo
        } else if (!chest->is_unarmed()) {
            chest->finish_throw(position->get_aiming_direction(), position->is_aiming_up(),
                                std::move(chest));
            // notify_dropped_weapon(weapon_id);
            pick_chest(std::make_unique<Unarmed>(
                    *stage));  // tecnicamente nunca habria que avisar que se droppeo algo
        } else if (!helmet->is_unarmed()) {
            helmet->finish_throw(position->get_aiming_direction(), position->is_aiming_up(),
                                 std::move(helmet));
            // notify_dropped_weapon(weapon_id);
            pick_helmet(std::make_unique<Unarmed>(
                    *stage));  // tecnicamente nunca habria que avisar que se droppeo algo

        }
    }
}

void Player::execute(const int& command) {
    if (!is_alive) {
        return;
    }
    if (command == SHOOT) {
        shoot();
        return;
    }
    if (command == THROW_WEAPON) {
        if (!weapon->is_unarmed()) {
            weapon->start_throw();
        } else if (!chest->is_unarmed()) {
            chest->start_throw();
        } else if (!helmet->is_unarmed()) {
            helmet->start_throw();
        }
    }
}

void Player::update() {
    if (!is_alive) {
        return;
    }
    should_notify = false;
    std::set<int> moving_commands;  // comandos que te emocionan son...
    for (int command: current_actions) {
        execute(command);
        if (command == 6 || command == MOVE_LEFT || command == MOVE_RIGHT || command == JUMP) {
            moving_commands.insert(command);
        }
    }
    move(moving_commands);
    if (weapon != nullptr) {
        weapon->update();
    }
    if (chest != nullptr) {
        chest->update();
    }
    if (helmet != nullptr) {
        helmet->update();
    }
    if (should_notify) {
        notify_moved();
    }
}

void Player::notify_moved() {
    Coordinate current_position = position->get_position();
    for (const PlayerObserver* obs: observers) {
        obs->update(name, current_position.x, current_position.y, position->get_state(),
                    position->get_facing_direction());
    }
}

void Player::notify_picked_weapon() {
    for (const PlayerObserver* obs: observers) {
        obs->update(name, weapon->get_id());
    }
}

void Player::notify_picked_chest() {
    for (const PlayerObserver* obs: observers) {
        obs->update(name, chest->get_id());
    }
}

void Player::notify_picked_helmet() {
    for (const PlayerObserver* obs: observers) {
        obs->update(name, helmet->get_id());
    }
}


// void Player::notify_dropped_weapon(uint8_t id) {
//     for (PlayerObserver* obs: observers) {
//         obs->update(name, id);
//     }
// }

void Player::move(const std::set<int>& movements) { position->move(movements); }

void Player::shoot() {
    if (weapon == nullptr) {
        return;
    }
    weapon->shoot(position->get_aiming_direction(), position->is_aiming_up());
}

void Player::die() { is_alive = false; }

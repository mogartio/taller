#include "player_position.h"

#include <string>

#include "player.h"
#define LEFT "a"
#define RIGHT "d"
#define UP "w"
#define AIM_LEFT -1
#define AIM_RIGHT 1

PlayerPosition::PlayerPosition(Coordinate& initial_coordinates, Player& player, Stage& stage):
        position(initial_coordinates), player(player), stage(stage), facing_direction(AIM_RIGHT) {
    air_state = std::move(std::make_unique<Grounded>());
}

void PlayerPosition::move(std::set<std::string>& directions) {
    int x_offset = 0;
    for (std::string direction: directions) {
        if (direction.compare(LEFT) == 0) {  // si direccion es izq...
            facing_direction = AIM_LEFT;
            x_offset = -1;
        } else if (direction.compare(RIGHT) == 0) {
            facing_direction = AIM_RIGHT;
            x_offset = 1;
            // OBS: si se manda instruccion de izq y der al mismo tiempo, se va a la der
        } else if (direction.compare(UP) == 0) {
            air_state->jump(*this);
        } else {
            continue;
        }
    }
    free_occupied();
    move_horizontally(x_offset);
    move_vertically(air_state->get_offset());
    air_state->update(stage.should_fall(*this), *this);
}

void PlayerPosition::move_horizontally(int offset) {
    Coordinate current(position.x + offset, position.y);
    if (stage.is_valid_position(current, player.get_id())) {
        position = current;
    }
}
void PlayerPosition::released_w() { air_state->stop_jumping(*this); }
void PlayerPosition::set_state(std::unique_ptr<AirState> new_state) {
    air_state = std::move(new_state);
}

void PlayerPosition::move_vertically(int offset) {
    int direction_handler = 1;
    if (offset < 0) {
        direction_handler = -1;
    }
    for (int i = 0; i < offset * direction_handler; i++) {
        Coordinate current(position.x, position.y + direction_handler);
        if (stage.is_valid_position(current, player.get_id())) {
            position = current;
        } else {
            return;
        }
    }
}

void PlayerPosition::occupy(Coordinate& coordinate) { occupied.push_back(coordinate); }
void PlayerPosition::free_occupied() { occupied.clear(); }

std::vector<Coordinate> PlayerPosition::get_occupied() { return occupied; }

Coordinate PlayerPosition::get_position() { return position; }

int PlayerPosition::get_facing_direction() { return facing_direction; }
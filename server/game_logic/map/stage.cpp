#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "../player/player.h"
#define FLOOR 1
#define BACKGROUND 0
#define PLAYER_SIZE 3
#define LEFT "a"
#define RIGHT "d"
#define UP "w"
#include "../player/projectile.h"

#include "csv_reader.h"
#include "stage.h"

Stage::Stage(const std::string& file_name): map(0, 0) {
    CSVReader reader(file_name);
    map = std::move(reader.read_map());
}

void Stage::print() { map.print(); }

void Stage::add_projectile(std::unique_ptr<Projectile>&& projectile) {
    projectiles.push_back(std::move(projectile));
}

void Stage::remove_projectile(std::unique_ptr<Projectile>& projectile) {
    auto position_to_delete = std::find_if(
            projectiles.begin(), projectiles.end(),
            [&](const std::unique_ptr<Projectile>& p) { return p.get() == projectile.get(); });
    if (position_to_delete != projectiles.end()) {  // si se encontro borrar, sino se encontro... no
        projectiles.erase(position_to_delete);
    }
}

void Stage::update() {
    for (auto& c: coordinates_to_delete) {
        map.set(c, BACKGROUND);
    }
    for (auto iterator = projectiles.begin(); iterator != projectiles.end();) {
        size_t initial_projectiles = projectiles.size();
        ray_trace(*iterator);
        if (iterator == projectiles.end()) {
            break;
        }
        if (initial_projectiles != projectiles.size()) {
            // si cambio la cantidad de elementos se borro el elemento al que
            // el iterator apuntaba antes lo que hace que ahora el iterator
            // apunte al siguiente elemento en la lista y no hay que cambiarlo
            continue;
        }
        iterator++;
    }
}

void Stage::draw_player(Player& player) {
    Coordinate init_position = player.get_position();
    int x = init_position.x;
    int y = init_position.y;
    for (int i = 0; i < PLAYER_SIZE; i++) {
        for (int j = 0; j < PLAYER_SIZE; j++) {
            Coordinate current(x + i, y + j);
            player.occupy(current);
            map.set(current, player.get_id());
        }
    }
}

void Stage::delete_player_from_stage(Player& player) {
    std::vector<Coordinate> occupied = player.get_occupied();
    for (auto& coordinate: occupied) {
        map.set(coordinate, BACKGROUND);
    }
}

bool Stage::should_fall(PlayerPosition& player_position) {
    Coordinate current_position = player_position.get_position();
    Coordinate duck_feet(current_position.x, current_position.y + PLAYER_SIZE);
    for (int i = 0; i < PLAYER_SIZE; i++) {
        Coordinate aux(duck_feet.x + i, duck_feet.y);
        if (map.get(aux) == FLOOR) {
            return false;
        }
    }
    return true;
}

bool Stage::is_valid_position(Coordinate position, int color) {
    for (int i = 0; i < PLAYER_SIZE; i++) {
        for (int j = 0; j < PLAYER_SIZE; j++) {
            Coordinate aux(position.x + j, position.y + i);
            if (map.out_of_range(aux)) {
                return false;
            }
            int value = map.get(aux);
            if (value != BACKGROUND && value != color) {
                return false;
            }
        }
    }
    return true;
}

// direction es un versor del eje x. es decir es 1 o -1
// se llama ray_trace pero ya no lo es
void Stage::ray_trace(std::unique_ptr<Projectile>& projectile) {
    int direction = projectile->get_x_direction();
    double deviation_angle = projectile->get_deviation_angle();
    int reach = projectile->get_speed();  // cuanto tiene que moverse en un tick
    Coordinate initial_position = projectile->get_position();
    int direction_handler = direction;  // para usar el mismo loop sea la direccion 1 o -1
    for (int i = 0; i < reach; i++) {
        // Probablemente sea pegarse un tiro en el pie usar trigonometria. no me pude resistir
        Coordinate bullet_position(
                round(initial_position.x + ((i)*sin(deviation_angle)) * direction_handler),
                round(initial_position.y + (i)*cos(deviation_angle)));
        std::cout << bullet_position.x << bullet_position.y << std::endl;
        int next_tile = map.get(bullet_position);
        if (next_tile == -1) {
            remove_projectile(projectile);
            return;
        }
        if ((next_tile == BACKGROUND || next_tile == projectile->get_id() || next_tile == 4) &&
            i == reach - 1) {
            map.set(bullet_position, projectile->get_id());
            projectile->move(bullet_position);
            coordinates_to_delete.push_back(bullet_position);
            /*} else if(next_tile == PLAYER){
                kill(player); */
        } else if (next_tile == FLOOR) {
            remove_projectile(projectile);
            return;
        }
    }
}

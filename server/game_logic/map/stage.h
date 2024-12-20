#ifndef STAGE_H
#define STAGE_H

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "../player/weapons/projectiles/projectile.h"

#include "map.h"

class Player;          // Declaracion adelantada para evitar dependencia circular.
class PlayerPosition;  // Idem
class MysteryBox;      // Declaracion adelantada para evitar dependencia circular.
class Stage {
private:
    Map map;
    void printStage();
    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::vector<std::shared_ptr<Projectile>> new_weapons;
    std::vector<std::tuple<Coordinate, int>> explosions;
    std::vector<Coordinate> coordinates_to_delete;
    SendQueuesMonitor<std::shared_ptr<GenericMsg>>& senders;
    std::vector<std::shared_ptr<MysteryBox>> boxes;
    std::vector<std::shared_ptr<MysteryBox>> new_boxes;
    ProjectileObserver obs;
    std::map<int, Player*> players;
    void explode_vertically(Coordinate starting_position, int radius, int vertical_direction,
                            bool& keep_going_horizontally);
    std::shared_ptr<Projectile> find_projectile_in(Coordinate init_position, int size);
    std::shared_ptr<MysteryBox> find_box_in(Coordinate init_position, int size);
    void remove_box(std::shared_ptr<MysteryBox>& box);

public:
    // Son es para poder mockear la clase mas facilmente
    void draw_player(Player&);
    int is_valid_position(Coordinate, int);
    Stage(Map& map, SendQueuesMonitor<std::shared_ptr<GenericMsg>>&,
          std::shared_ptr<std::set<uint>> ids);
    void delete_player_from_stage(Player&);
    void print();
    bool should_fall(PlayerPosition&);
    void add_projectile(std::shared_ptr<Projectile>&&);
    void remove_projectile(std::shared_ptr<Projectile>&);
    void update();
    void set(const Coordinate&, const int);
    int get(const Coordinate&);
    std::vector<uint16_t> get_vector_representation() { return map.get_vector_representation(); }
    void set_explosion(Coordinate, int);
    void explode(Coordinate, int);
    std::shared_ptr<Weapon> pick_weapon(Coordinate);
    void add_player(Player*, int id);
    void kill(int id);
    void draw(int object, int size, Coordinate init_position);
    void add_box(std::shared_ptr<MysteryBox> box);
    void add_new_box(std::shared_ptr<MysteryBox> box);
    bool should_fall(Coordinate init_position, int size);

    // las cosas que un objeto en position de tamanio size esta tocando en un momento
    std::set<int> things_projectile_hits(Coordinate position, int size);

    void break_box(Coordinate position);
    // chequear si tiene armadura/casco y hace daño
    bool take_damage(int player_id);
};

#endif

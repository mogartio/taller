#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <cmath>
#include <cstdlib>
#include <memory>
#include <utility>

#include "../../../../../common/coordinate.h"
#include "../../subject.h"
class Weapon;
class Stage;

class Projectile: public ProjectileSubject {
protected:
    int speed;
    int x_direction;
    double deviation_angle;
    Coordinate position;
    int reach;
    int distance_covered;
    int id;
    std::vector<std::pair<uint16_t, uint16_t>> trail;
    bool despawns_on_contact;

public:
    Projectile(Coordinate& initial_position, int x_direction, int reach, int speed,
               double deviation_angle, int id, bool despawns_on_contact):
            speed(speed),
            x_direction(x_direction),
            deviation_angle(deviation_angle),
            position(initial_position),
            reach(reach),
            distance_covered(0),
            id(id),
            despawns_on_contact(despawns_on_contact) {}
    virtual void move(Coordinate new_position) { position = new_position; }
    virtual int get_x_direction() { return x_direction; }
    virtual int get_speed() { return speed; }
    virtual double get_deviation_angle() { return deviation_angle; }
    virtual Coordinate get_position() { return position; }
    virtual ~Projectile() = default;
    virtual int get_id() { return id; }
    virtual void update() {}
    virtual bool ray_trace(Stage& stage);
    virtual void notify() override;
};


class CowboyBullet: public Projectile {
public:
    CowboyBullet(Coordinate& initial_position, int x_direction, bool is_aiming_up, int reach):
            Projectile(initial_position, x_direction, reach, 15, 0, 8, true) {
        deviation_angle = M_PI / 2 + (M_PI / 2 * is_aiming_up);
    }
};

class MagnumBullet: public Projectile {
public:
    MagnumBullet(Coordinate& initial_position, int x_direction, bool is_aiming_up, int reach):
            Projectile(initial_position, x_direction, reach, 15, 0, 8, true) {
        deviation_angle = M_PI / 1.92 + (M_PI / 2 * is_aiming_up);
    }
};
#endif
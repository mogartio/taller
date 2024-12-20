#ifndef HEAVY_H
#define HEAVY_H

#include "weapon.h"
class Ak47: public Weapon {
private:
    int bullets_shot;                    // la cantidad de balas disparadas sin levantar la tecla
    bool stopped_pressing_since_picked;  // para que no dispare cuando la agarras

public:
    explicit Ak47(Stage&);
    virtual void shoot(int, bool) override;
    virtual void stop_shooting() override;
    virtual void finish_throw(int x_direction, bool, std::shared_ptr<Weapon> weapon) override;
};
class Shotgun: public Weapon {
private:
    bool is_loaded;
    int bullets_per_shot;
    int init_deviation;

public:
    explicit Shotgun(Stage&);
    virtual void shoot(int, bool) override;
    virtual void stop_shooting() override;
};
class Sniper: public Weapon {
    bool is_loaded;

public:
    explicit Sniper(Stage&);
    virtual void shoot(int, bool) override;
    virtual void stop_shooting() override;
};
#endif

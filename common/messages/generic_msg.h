#ifndef GENERIC_MSG_H
#define GENERIC_MSG_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

// TODO: Aca se puede directamente agregar los imports de los handlers
// TODO: No se porque se los declara tempranamente aca????
#include "descripcion-lobby.h"
#include "handler_read.h"
#include "handler_recv.h"
#include "handler_send.h"

namespace ActionsId {

enum ActionId : uint8_t {
    MOVE_LEFT = 0x01,
    MOVE_RIGHT = 0x02,
    JUMP = 0x03,
    SHOOT = 0x04,
    PLAY_DEAD = 0x05,
    AIM_UP = 0x06,
    THROW_WEAPON = 0x07,
    CHEAT = 0x08,
};


}

namespace ProjectilesId {

enum ProjectileId : uint8_t {
    UNARMED = 0x00,
    AK_47 = 0x05,
    DUEL_PISTOL = 0x06,
    COWBOY_PISTOL = 0x07,
    MAGNUM = 0x08,
    GRENADE = 0x09,
    SNIPER = 0x0A,
    LIVE_BANANA = 0X0B,
    SHOTGUN = 0x0C,
    PEW_PEW_LASER = 0x0D,
    BANANA = 0x0E,
    LASER_RIFLE = 0x0F,

    // balas (?)
    LASER = 0x10,           // PEW_PEW_LASER y LASER_RIFLE
    BULLET_PISTOL = 0x11,   // AK_47, DUEL_PISTOL, COWBOY_PISTOL, MAGNUM, SHOTGUN, SNIPER
    BULLET_SHOTGUN = 0x12,  // AK_47, DUEL_PISTOL, COWBOY_PISTOL, MAGNUM, SHOTGUN, SNIPER

    // armadura
    HELMET = 0x20,
    HELMET2 = 0x21,
    HELMET3 = 0x22,
    CHEST = 0x23,


    EXPLOSION = 0x30,

    MYSTERY_BOX = 0x40,
};
}

class GenericMsg {
public:
    enum MsgTypeHeader : uint8_t {
        INFO_LOBBY_MSG = 0x00,
        CUSTOMIZED_PLAYER_INFO_MSG = 0x01,
        VIEW_LOBBIES_MSG = 0x02,
        CHOOSE_LOBBY_MSG = 0x03,
        CREATE_LOBBY_MSG = 0x04,
        GO_BACK_MSG = 0x05,
        EXIT_FROM_LOBBY_MSG = 0x06,
        START_GAME_MSG = 0x07,
        PICKUP_DROP_MSG = 0x08,
        START_ACTION_MSG = 0x09,
        STOP_ACTION_MSG = 0x0A,
        SEND_LOBBIES_LIST_MSG = 0x0B,
        EVERYTHING_OK_MSG = 0x0C,
        ERROR_MSG = 0x0D,
        SEND_MAP_MSG = 0x0E,
        GAME_ENDED_MSG = 0x0F,
        WINNER_MSG = 0x10,
        UPDATED_PLAYER_INFO_MSG = 0x11,
        PROJECTILE_INFO_MSG = 0x12,
        PLAYER_INFO_MSG = 0x13,  // mensaje para enviar info de un jugador especifico
        NOT_PROJECTILE_INFO = 0x14,
        SHOOT_MSG = 0x15,
        START_ROUND_MSG = 0x16,
    };

    enum Type : uint8_t {
        LOBBY_MSG = 0x01,
        GAME_MSG = 0x02,
    };


    enum FacingDirection : uint8_t {
        RIGHT = 0x01,
        LEFT = 0x02,
        UP = 0x03,
        DOWN = 0x04,
    };


    enum DuckColor : uint8_t {
        WHITE = 0x00,
        YELLOW = 0x01,
        ORANGE = 0x02,
        GREY = 0x03,
    };

    enum PlayerReadyState : uint8_t {
        NOT_READY = 0x00,
        READY = 0x01,
    };

    enum LobbyState : uint8_t {
        NOT_STARTING = 0x00,
        STARTING = 0x01,
    };

    enum Theme : uint8_t {
        DAY = 0x00,
        NIGHT = 0x01,
    };

private:
    MsgTypeHeader header;
    Type type;
    int id_client;

public:
    explicit GenericMsg(MsgTypeHeader header, Type type);

    virtual void accept_send(HandlerSender& handler) = 0;
    virtual void accept_recv(HandlerReceiver& handler) = 0;
    virtual void accept_read(HandlerReader& handler) = 0;
    uint8_t get_header() const;
    uint8_t get_type() const;
    int get_id_client() const;
    void set_id_client(int id_client);
    virtual ~GenericMsg() = default;
};

class InfoLobbyMsg: public GenericMsg {
private:
    std::list<DescripcionPlayer> players;
    uint8_t max_players;
    uint8_t lobby_id;
    uint8_t starting_game;  // 0x00 no, 0x01 yes

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    InfoLobbyMsg();

    explicit InfoLobbyMsg(std::list<DescripcionPlayer> players, uint8_t max_players,
                          uint8_t lobby_id, uint8_t starting_game);

    void set_players(std::list<DescripcionPlayer> players);

    void set_max_players(uint8_t max_players);

    void set_lobby_id(uint8_t lobby_id);

    void set_starting_game(uint8_t starting_game);

    std::list<DescripcionPlayer> get_players() const;

    uint8_t get_max_players() const;

    uint8_t get_lobby_id() const;

    uint8_t get_starting_game() const;
};


class CustomizedPlayerInfoMsg: public GenericMsg {
private:
    uint8_t lobby_id;
    uint8_t color;
    std::string player_name;
    std::string player_new_name;
    uint8_t is_ready;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    CustomizedPlayerInfoMsg();

    explicit CustomizedPlayerInfoMsg(uint8_t lobby_id, uint8_t color, std::string player_name,
                                     std::string player_new_name, uint8_t is_ready);

    void set_lobby_id(uint8_t lobby_id);

    uint8_t get_lobby_id() const;

    void set_color(uint8_t color);

    uint8_t get_color() const;

    std::string get_player_name() const;

    void set_player_name(std::string player_name);

    std::string get_player_new_name() const;

    void set_player_new_name(std::string player_new_name);

    bool get_is_ready() const;

    void set_is_ready(uint8_t is_ready);
};

class ViewLobbiesMsg: public GenericMsg {
private:
public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    ViewLobbiesMsg();
};

class ChooseLobbyMsg: public GenericMsg {
private:
    uint8_t lobby_id;
    std::string player_name;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    ChooseLobbyMsg();

    explicit ChooseLobbyMsg(uint8_t lobby_id, std::string player_name);

    void set_lobby_id(uint8_t lobby_id);

    uint8_t get_lobby_id() const;

    std::string get_player_name() const;

    void set_player_name(std::string player_name);
};

class CreateLobbyMsg: public GenericMsg {
private:
    std::string player_name;
    std::string lobby_name;
    uint8_t max_players;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    CreateLobbyMsg();

    CreateLobbyMsg(std::string player_name, std::string lobby_name, uint8_t max_players);

    void set_player_name(std::string player_name);

    void set_lobby_name(std::string lobby_name);

    void set_max_players(uint8_t max_players);

    std::string get_player_name() const;

    std::string get_lobby_name() const;

    uint8_t get_max_players() const;
};

class GoBackMsg: public GenericMsg {
private:
public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    GoBackMsg();
};

class ExitFromLobbyMsg: public GenericMsg {
private:
    std::string player_name;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    ExitFromLobbyMsg();

    explicit ExitFromLobbyMsg(std::string player_name);

    void set_player_name(std::string player_name);

    std::string get_player_name() const;
};

class StartGameMsg: public GenericMsg {
private:
public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    StartGameMsg();
};

class PickupDropMsg: public GenericMsg {
private:
    std::string player_name;
    uint16_t item_id;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    PickupDropMsg();

    explicit PickupDropMsg(std::string player_name, uint16_t item_id);

    void set_item_id(uint16_t item_id);

    void set_player_name(std::string player_name);

    uint16_t get_item_id() const;

    std::string get_player_name() const;
};

class StartActionMsg: public GenericMsg {
private:
    uint8_t action_id;
    std::string player_name;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    StartActionMsg();

    explicit StartActionMsg(uint8_t action_id, std::string player_name);

    void set_action_id(uint8_t action_id);

    void set_player_name(std::string player_name);

    uint8_t get_action_id() const;

    std::string get_player_name() const;
};

class StopActionMsg: public GenericMsg {
private:
    uint8_t action_id;
    std::string player_name;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    StopActionMsg();

    explicit StopActionMsg(uint8_t action_id, std::string player_name);

    void set_action_id(uint8_t action_id);

    void set_player_name(std::string player_name);

    uint8_t get_action_id() const;

    std::string get_player_name() const;
};


class SendLobbiesListMsg: public GenericMsg {
private:
    std::vector<DescripcionLobby> lobbies;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    SendLobbiesListMsg();

    explicit SendLobbiesListMsg(std::vector<DescripcionLobby> lobbies);

    void set_lobbies(std::vector<DescripcionLobby> lobbies);

    std::vector<DescripcionLobby> get_lobbies() const;
};

class EverythingOkMsg: public GenericMsg {
private:
public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    EverythingOkMsg();
};

class ErrorMsg: public GenericMsg {
private:
    std::string error_msg;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    ErrorMsg();

    explicit ErrorMsg(std::string error_msg);

    void set_error_msg(std::string error_msg);

    std::string get_error_msg() const;
};

class SendMapMsg: public GenericMsg {
private:
    std::vector<uint16_t> map;  // le puse string pero no se que va
    uint16_t filas;
    uint16_t columnas;
    uint8_t theme;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    SendMapMsg();

    explicit SendMapMsg(std::vector<uint16_t> map, uint16_t filas, uint16_t columnas,
                        uint8_t theme);

    void set_map(std::vector<uint16_t> map);

    std::vector<uint16_t> get_map() const;

    void set_filas(uint16_t filas);

    void set_columnas(uint16_t columnas);

    void set_theme(uint8_t theme);

    uint16_t get_filas() const;

    uint16_t get_columnas() const;

    uint8_t get_theme() const;
};

class GameEndedMsg: public GenericMsg {
private:
public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    GameEndedMsg();
};

class WinnerMsg: public GenericMsg {
private:
    std::string winner_name;
    // type stats (a definir)

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    WinnerMsg();

    explicit WinnerMsg(std::string winner_name);

    void set_winner_name(std::string winner_name);

    std::string get_winner_name() const;
};

class UpdatedPlayerInfoMsg: public GenericMsg {
private:
    std::string player_name;
    std::pair<uint16_t, uint16_t> position;
    uint8_t state;
    uint8_t facing_direction;
    uint8_t facing_direction_second;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    UpdatedPlayerInfoMsg();

    explicit UpdatedPlayerInfoMsg(std::string player_name, std::pair<uint16_t, uint16_t> position,
                                  uint8_t state, uint8_t facing_direction,
                                  uint8_t facing_direction_second = 0);

    std::string get_player_name() const;

    std::pair<uint8_t, uint8_t> get_position() const;

    uint8_t get_state() const;

    uint8_t get_facing_direction() const;

    void set_player_name(std::string player_name);

    void set_position(uint8_t x, uint8_t y);

    void set_state(uint8_t state);

    void set_facing_direction(uint8_t facing_direction);

    uint8_t get_facing_direction_second() const;

    void set_facing_direction_second(uint8_t facinDirectionSecond);
};

class ProjectileInfoMsg: public GenericMsg {
private:
    uint8_t pos_x;
    uint8_t pos_y;
    uint8_t item;
    std::vector<std::pair<uint8_t, uint8_t>> trail;
    uint8_t facing_direction_first;
    uint8_t facing_direction_second;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    ProjectileInfoMsg();

    explicit ProjectileInfoMsg(std::vector<std::pair<uint8_t, uint8_t>>, uint8_t pos_x,
                               uint8_t pos_y, uint8_t item, uint8_t facing_direction_first = 0,
                               uint8_t facing_direction_second = 0);

    uint8_t get_pos_x() const;

    uint8_t get_pos_y() const;

    uint8_t get_item() const;

    std::vector<std::pair<uint8_t, uint8_t>> get_trail() const;

    void set_pos_x(uint8_t pos_x);

    void set_pos_y(uint8_t pos_y);

    void set_item(uint8_t item);

    void set_trail(std::vector<std::pair<uint8_t, uint8_t>> trail);

    uint8_t get_facing_direction_first() const;

    void set_facing_direction_first(uint8_t facing_direction_first);

    uint8_t get_facing_direction_second() const;

    void set_facing_direction_second(uint8_t facing_direction_second);
};


class PlayerInfoMsg: public GenericMsg {
private:
    std::string player_name;
    uint8_t color;

public:
    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    PlayerInfoMsg();

    explicit PlayerInfoMsg(std::string player_name, uint8_t color);

    std::string get_player_name() const;

    uint8_t get_color() const;

    void set_player_name(std::string player_name);

    void set_color(uint8_t color);
};

class NotProyectileInfo: public GenericMsg {
private:
    uint8_t item;
    std::pair<uint8_t, uint8_t> position_x_y;

public:
    NotProyectileInfo();

    explicit NotProyectileInfo(uint8_t item, std::pair<uint8_t, uint8_t> position_x_y);

    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    uint8_t get_item() const;

    std::pair<uint8_t, uint8_t> get_position_x_y() const;

    void set_item(uint8_t item);

    void set_position_x_y(std::pair<uint8_t, uint8_t> position_x_y);
};

class ShootMsg: public GenericMsg {
private:
    std::string player_name;

public:
    ShootMsg();

    explicit ShootMsg(std::string player_name, std::pair<uint8_t, uint8_t> position_x_y);

    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;

    std::string get_player_name() const;

    void set_player_name(std::string player_name);
};

class StartRoundMsg: public GenericMsg {

public:
    StartRoundMsg();

    void accept_send(HandlerSender& handler) override;

    void accept_recv(HandlerReceiver& handler) override;

    void accept_read(HandlerReader& handler) override;
};


#endif

#include <map>

#include <SDL2/SDL_timer.h>

#include "frontOnePlaye.h"
#define TILES_TO_PIXELS 16

enum Front_event { MOVE_LEFT, MOVE_RIGHT, JUMP_EVENT, PLAY_DEAD, END };

OnePlayer::OnePlayer(Queue<GenericMsg*>& queueRecive, std::string playerName,
                     std::atomic<bool>& running):
        queueRecive(queueRecive), playerName(playerName), running(running) {}

void OnePlayer::play() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return;
    }

    GenericMsg* matriz = queueRecive.pop();

    std::vector<uint16_t> mapa;
    uint16_t filas;
    uint16_t columnas;
    if (matriz->get_header() == GenericMsg::MsgTypeHeader::SEND_MAP_MSG) {
        SendMapMsg* map = dynamic_cast<SendMapMsg*>(matriz);
        mapa = map->get_map();
        filas = map->get_filas();
        columnas = map->get_columnas();
    } else if (matriz == nullptr) {
        throw("Algo anda mal! Mandaste un msj que nda que ver");
    }

    Window win(columnas * TILES_TO_PIXELS, filas * TILES_TO_PIXELS);

    Map map(win.get_rend(), mapa);
    map.makeMap(columnas, filas);

    GenericMsg* jugador = queueRecive.pop();

    if (jugador->get_header() == GenericMsg::MsgTypeHeader::UPDATED_PLAYER_INFO_MSG) {
        std::cout << "se entro, recibio el msg updateando la pos" << std::endl;
        UpdatedPlayerInfoMsg* player = dynamic_cast<UpdatedPlayerInfoMsg*>(jugador);
        map.addPlayer(player->get_position().first, player->get_position().second, 2,
                      player->get_player_name());
    }

    const Uint32 frame_rate = 1000 / 30;      // 30 FPS
    Uint32 last_frame_time = SDL_GetTicks();  // Tiempo del último frame

    bool stated_palying = false;
    std::string player_name;
    std::pair<uint16_t, uint16_t> position;
    uint8_t facing_direction = 1;

    std::map<int, bool> pressed_keys;
    pressed_keys[SDL_SCANCODE_E] = false;
    pressed_keys[SDL_SCANCODE_F] = false;
    pressed_keys[SDL_SCANCODE_W] = false;
    pressed_keys[SDL_SCANCODE_S] = false;
    pressed_keys[SDL_SCANCODE_A] = false;
    pressed_keys[SDL_SCANCODE_D] = false;

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time = current_time - last_frame_time;

        GenericMsg* msj;
        bool poped = queueRecive.try_pop(msj);

        if (poped) {
            if (msj->get_header() == GenericMsg::MsgTypeHeader::UPDATED_PLAYER_INFO_MSG) {
                UpdatedPlayerInfoMsg* player = dynamic_cast<UpdatedPlayerInfoMsg*>(msj);
                player_name = player->get_player_name();
                position = player->get_position();
                uint8_t state = player->get_state();
                facing_direction = player->get_facing_direction();
                map.update(player_name, position.first, position.second, DuckState(state),
                           Side(facing_direction - 1));
                stated_palying = true;
            } else if (msj->get_header() == GenericMsg::MsgTypeHeader::GAME_ENDED_MSG) {
                // directa de que termino la partida y de q hay que mostrar la pantalla de fin
            }
        } else if (stated_palying) {
            map.update(player_name, position.first, position.second, DuckState::STANDING,
                       Side(facing_direction - 1));
        }

        // Renderiza los objetos en la ventana
        if (elapsed_time >= frame_rate) {
            win.clear();
            map.fill();
            win.fill();
            last_frame_time = current_time;
        }

        // Controla la frecuencia de cuadros por segundo (FPS)
        SDL_Delay(std::max(0, static_cast<int>(frame_rate - (SDL_GetTicks() - current_time))));
    }
}

#include "eventhandler.h"

#include <map>

#include <SDL2/SDL_timer.h>

#define ID_ITEM 0x05

EventHandler::EventHandler(Queue<GenericMsg*>& queueSend, std::string& p1,
                           std::atomic<bool>& running, std::string p2):
        queueSend(queueSend), playerName1(p1), running(running), playerName2(p2) {
    // Key down actions
    key_accion_map[std::make_tuple(SDL_KEYDOWN, SDL_SCANCODE_E)] = [this]() {
        return new StartActionMsg(ActionsId::SHOOT, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYDOWN, SDL_SCANCODE_F)] = [this]() {
        return new PickupDropMsg(ID_ITEM, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYDOWN, SDL_SCANCODE_W)] = [this]() {
        return new StartActionMsg(ActionsId::JUMP, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYDOWN, SDL_SCANCODE_S)] = [this]() {
        return new StartActionMsg(ActionsId::PLAY_DEAD, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYDOWN, SDL_SCANCODE_A)] = [this]() {
        return new StartActionMsg(ActionsId::MOVE_LEFT, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYDOWN, SDL_SCANCODE_D)] = [this]() {
        return new StartActionMsg(ActionsId::MOVE_RIGHT, playerName1);
    };

    // Key up actions
    key_accion_map[std::make_tuple(SDL_KEYUP, SDL_SCANCODE_S)] = [this]() {
        return new StopActionMsg(ActionsId::PLAY_DEAD, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYUP, SDL_SCANCODE_A)] = [this]() {
        return new StopActionMsg(ActionsId::MOVE_LEFT, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYUP, SDL_SCANCODE_D)] = [this]() {
        return new StopActionMsg(ActionsId::MOVE_RIGHT, playerName1);
    };
    key_accion_map[std::make_tuple(SDL_KEYUP, SDL_SCANCODE_W)] = [this]() {
        return new StopActionMsg(ActionsId::JUMP, playerName1);
    };

    // Inicializo el estado de las teclas
    pressed_keys_state[SDL_SCANCODE_E] = false;
    pressed_keys_state[SDL_SCANCODE_F] = false;
    pressed_keys_state[SDL_SCANCODE_W] = false;
    pressed_keys_state[SDL_SCANCODE_S] = false;
    pressed_keys_state[SDL_SCANCODE_A] = false;
    pressed_keys_state[SDL_SCANCODE_D] = false;
}

bool EventHandler::corroboraciones(SDL_Event& event) {
    return (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) ||
           (pressed_keys_state[event.key.keysym.scancode] && event.type == SDL_KEYDOWN);
}

void EventHandler::run() {
    // Mientras el thread siga corriendo
    GenericMsg* msg;
    bool mensajeEnviado = false;
    while (running) {
        SDL_Event event;
        // Procesa los eventos encolados
        if (SDL_WaitEvent(&event)) {  // esperamos un evento exitoso
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (corroboraciones(event)) {
                continue;
            }

            // si es un evento de tecla presionada o soltada
            pressed_keys_state[event.key.keysym.scancode] =
                    event.type ==
                    SDL_KEYDOWN;  // si es un evento de tecla soltada, el valor pasa a ser false
            try {
                msg = key_accion_map.at(std::make_tuple(event.type, event.key.keysym.scancode))();
                mensajeEnviado = false;
            } catch (std::out_of_range& e) {
                // si no se encontro la tecla en el mapa, no hacemos nada
                continue;
            }
            if (!mensajeEnviado) {
                queueSend.push(msg);
                mensajeEnviado = true;
            }
        }
    }
}

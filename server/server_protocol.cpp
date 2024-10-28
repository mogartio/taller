#include "server_protocol.h"

#include <cstdint>
#include <string>
#include <vector>

ServerProtocol::ServerProtocol(Socket& skt): ProtocoloCommon(skt) {}

void ServerProtocol::handle_send(const EverythingOkMsg& msg) {
    uint8_t header = msg.get_header();
    send_u_int8_t(header);
}

void ServerProtocol::handle_send(const ErrorMsg& msg) {
    uint8_t header = msg.get_header();
    send_u_int8_t(header);
    std::string error_msg = msg.get_error_msg();
    send_string(error_msg);
}

void ServerProtocol::handle_recv(ViewLobbiesMsg& msg) { (void)msg; }

void ServerProtocol::handle_recv(ChooseLobbyMsg& msg) {
    uint8_t lobby_id = recv_u_int8_t();
    msg.set_lobby_id(lobby_id);
}

void ServerProtocol::handle_recv(CreateLobbyMsg& msg) { (void)msg; }

void ServerProtocol::handle_recv(GoBackMsg& msg) { (void)msg; }

void ServerProtocol::handle_recv(StartGameMsg& msg) { (void)msg; }

void ServerProtocol::handle_recv(ExitFromLobbyMsg& msg) {
    std::string player_name = recv_string();
    msg.set_player_name(player_name);
}

void ServerProtocol::handle_send(const SendLobbiesListMsg& msg) {
    uint8_t header = msg.get_header();
    send_u_int8_t(header);
    std::vector<std::string> lobbies = msg.get_lobbies();
    // mando el numero de lobbies
    uint8_t lobbies_size = lobbies.size();
    send_u_int8_t(lobbies_size);
    // mando los nombres de los lobbies
    for (int i = 0; i < lobbies_size; i++) {
        send_string(lobbies[i]);
        // lobbies[i].get_amount_of_players();
    }
}

void ServerProtocol::handle_recv(CustomizedPlayerInfoMsg& msg) {
    uint8_t color = recv_u_int8_t();
    msg.set_color(color);
    std::string player_name = recv_string();
    msg.set_player_name(player_name);
}

void ServerProtocol::handle_recv(PickupDropMsg& msg) {
    uint8_t item_id = recv_u_int8_t();
    msg.set_item_id(item_id);
    std::string player_name = recv_string();
    msg.set_player_name(player_name);
}

void ServerProtocol::handle_recv(StartActionMsg& msg) {
    uint8_t action_id = recv_u_int8_t();
    msg.set_action_id(action_id);
    std::string player_name = recv_string();
    msg.set_player_name(player_name);
}

void ServerProtocol::handle_recv(StopActionMsg& msg) {
    uint8_t action_id = recv_u_int8_t();
    msg.set_action_id(action_id);
    std::string player_name = recv_string();
    msg.set_player_name(player_name);
}

void ServerProtocol::handle_send(const GameEndedMsg& msg) {
    uint8_t header = msg.get_header();
    send_u_int8_t(header);
}

void ServerProtocol::handle_send(const WinnerMsg& msg) {
    uint8_t header = msg.get_header();
    send_u_int8_t(header);
    std::string player_name = msg.get_winner_name();
    send_string(player_name);
}

#ifndef HANDLER_SENDER_H
#define HANDLER_SENDER_H

class InfoLobbyMsg;
class CustomizedPlayerInfoMsg;
class ViewLobbiesMsg;
class ChooseLobbyMsg;
class CreateLobbyMsg;
class GoBackMsg;
class ExitFromLobbyMsg;
class StartGameMsg;
class PickupDropMsg;
class StartActionMsg;
class StopActionMsg;
class SendLobbiesListMsg;
class EverythingOkMsg;
class ErrorMsg;
class SendMapMsg;
class GameEndedMsg;
class WinnerMsg;
class UpdatedPlayerInfoMsg;
class ProjectileInfoMsg;
class PlayerInfoMsg;
class NotProyectileInfo;
class ShootMsg;
class StartRoundMsg;


class HandlerSender {
public:
    // from client
    virtual void handle_send(const InfoLobbyMsg& msg);
    virtual void handle_send(const CustomizedPlayerInfoMsg& msg);
    virtual void handle_send(const ViewLobbiesMsg& msg);
    virtual void handle_send(const ChooseLobbyMsg& msg);
    virtual void handle_send(const CreateLobbyMsg& msg);
    virtual void handle_send(const GoBackMsg& msg);
    virtual void handle_send(const ExitFromLobbyMsg& msg);
    virtual void handle_send(const StartGameMsg& msg);
    virtual void handle_send(const PlayerInfoMsg& msg);
    // in-game commands
    virtual void handle_send(const PickupDropMsg& msg);
    virtual void handle_send(const StartActionMsg& msg);
    virtual void handle_send(const StopActionMsg& msg);
    virtual void handle_send(const StartRoundMsg& msg);
    // from server
    virtual void handle_send(const SendLobbiesListMsg& msg);
    virtual void handle_send(const EverythingOkMsg& msg);
    virtual void handle_send(const ErrorMsg& msg);
    virtual void handle_send(const SendMapMsg& msg);
    virtual void handle_send(const GameEndedMsg& msg);
    virtual void handle_send(const WinnerMsg& msg);
    // in-game from server
    virtual void handle_send(const UpdatedPlayerInfoMsg& msg);
    virtual void handle_send(const ProjectileInfoMsg& msg);
    virtual void handle_send(const NotProyectileInfo& msg);
    virtual void handle_send(const ShootMsg& msg);

    virtual ~HandlerSender() = default;
};

#endif

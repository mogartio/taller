#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "../common/queue.h"
#include "../common/socket/socket.h"
#include "../common/thread.h"

#include "acceptor.h"
#include "client.h"
#include "clients_monitor.h"
#include "read_input.h"
#include "send_queues_monitor.h"

class Server {
private:
    Socket srv;
    ClientsMonitor clients;
    Queue<std::string> recv_queue;  // Pongo std::string pero va a cambiar segun el protocolo
    SendQueuesMonitor<std::string> send_queues;

public:
    explicit Server(const char* port);
    void run();
};
#endif

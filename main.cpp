
#include "Client/Client.h"
#include "Daemons/TSSEvaluator.h"

#include <thread>
#include <chrono>

int main() {
    Server server;
    Client client(server);

    TSSEvaluator daemon(server);

    daemon.run();

    client.run();

    daemon.stop();
}
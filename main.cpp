
#include "Client/Client.h"
#include "Deamons/TSSEvaluator.h"

#include <thread>
#include <chrono>

int main() {
    Server server;
    Client client(server);

    TSSEvaluator deamon(server);

    deamon.run();

    client.run();

    deamon.stop();
}
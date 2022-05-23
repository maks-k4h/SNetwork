cmake_minimum_required(VERSION 3.21)
project(SNetwork)

set(CMAKE_CXX_STANDARD 14)

add_executable(SNetwork
        main.cpp
        Core/MessageID.cpp
        Core/MessageID.h
        Core/Message.cpp
        Core/Message.h
        Server/Server.cpp
        Server/Server.h
        Server/Factories/MessageFactory.cpp
        Server/Factories/MessageFactory.h
        Client/Client.cpp Client/Client.h Core/MessageNode.cpp Core/MessageNode.h Daemons/TSSEvaluator.cpp Daemons/TSSEvaluator.h)

// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#define ENET_IMPLEMENTATION

#include <iostream>
// #include <winsock2.h> // for Windows
#include "CppEnetWrapper.hpp"
#include <stdexcept>
#include <ctime>
// #include <windows.h> // for Windows
using namespace std;

// This is a minimal example and test of using CppEnetWrapper
// Argument -s create server, otherwise client

int main(int argc, char *argv[], char *env[])
{
    std::cout << "CppEnetWrapperMinimal started ... " << std::endl;
    std::cout << "Usage: CppEnetWrapperMinimal [-s]" << std::endl;
    
    bool isServer = false;
    try
    {
        if (argc == 2)
        {
            if (argv[1][0] == "-s"[0] || argv[1][0] == "-S"[0])
                isServer = true;
        }
    }
    catch (exception &e)
    {
    };

    // // // // // // // // // // // // // // // // // // // // //
    CppEnetWrapper::MyEnetNetwork net;
    vector<int> eraryVector;
    // // // // // // // // // // // // // // // // // // // // //

    // Connection each other
    if (isServer)
    {
        // Server Session
        std::cout << "Server session started ... " << std::endl;
        net
            .setIsServer(true)
            .setBindHostName("localhost")
            .setBindPort(7996)
            .setMaxClients(4)
            .setChannels(2)
            .setAmountIn(0)
            .setAmountOut(0);
        net.initENet();
        net.registerClients(1000, 10);
    }
    else
    {
        // Client Session
        std::cout << "Client session started ... " << std::endl;
        net
            .setIsServer(false)
            .setRemoteHostName("localhost")
            .setRemotePort(7996)
            .setOutConnections(1);
        net.initENet();
        net.registerServer(1000, 10);
    }

    // bidirectional communication transfer data there and back in vector<int> type
    vector<int> bufToSendtoServer = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    vector<int> bufToSendtoClient = {15, 16, 17, 18, 999, 65535};

    //
    //
    // example of main transfer loop
    //
    //
#define DATA_RECEIVED (int)3

    time_t startTime = time(nullptr);
    while ((time(nullptr) - startTime < 20 /*main timeout for tests*/))
    {
        if (isServer)
        {
            // receiving data from other side
            int status = net.serverHostService(eraryVector, 50);
            if (eraryVector.size() > 0 && status == DATA_RECEIVED)
            {
                for (size_t i = 0; i < eraryVector.size(); i++)
                    cout << eraryVector[i] << " ";
                cout << endl;
            }

            // Send vector to client
            net.sendVectorToClient(bufToSendtoClient, bufToSendtoClient.size());
        }
        else
        {
            // receiving data from other side
            int status = net.clientHostService(eraryVector, 50);
            if (eraryVector.size() > 0 && status == DATA_RECEIVED)
            {
                for (size_t i = 0; i < eraryVector.size(); i++)
                    cout << eraryVector[i] << " ";
                cout << endl;
            }
            // Send vector to server
            net.sendVectorToServer(bufToSendtoServer, bufToSendtoServer.size());
        }
    }

    return 0;
}

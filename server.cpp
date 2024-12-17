#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <boost/json.hpp>
#include "routes.h"
#include "config.h"

using namespace std;

int main()
{
    boost::json::value json_data = getConfig();

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Failed to create socket" << endl;
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    int port = json_data.at("port").as_int64();
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "Failed to bind socket" << endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) == -1)
    {
        cerr << "Failed to listen on socket" << endl;
        close(serverSocket);
        return 1;
    }

    while (true)
    {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1)
        {
            cerr << "Failed to accept connection" << endl;
            continue;
        }

        char recvBuffer[1024] = {0};
        ssize_t bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        if (bytesReceived <= 0)
        {
            cerr << "Failed to receive data" << endl;
            close(clientSocket);
            continue;
        }

        cout << recvBuffer << endl;

        string request(recvBuffer);
        size_t pos = request.find(' ');
        size_t endPos = request.find(' ', pos + 1);
        string path = request.substr(pos + 1, endPos - pos - 1);

        cout << path << endl;

        string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
        string content = renderView(path, recvBuffer, getConfigMap());
        response += content;

        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
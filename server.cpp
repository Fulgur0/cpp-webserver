#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <boost/json/src.hpp>
#include <fstream>
#include "routes.h"

using namespace std;

int main()
{
    ifstream input("config.json");
    std::stringstream buffer;
    buffer << input.rdbuf();
    boost::json::value json_data = boost::json::parse(buffer.str());

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    int port = json_data.at("port").as_int64();
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    while (true)
    {
        listen(serverSocket, 5);
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        char recvBuffer[1024] = {0};
        recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        cout << recvBuffer << endl;

        string path = "";
        for (int i = 4; i < strlen(recvBuffer); i++)
        {
            if (recvBuffer[i] == ' ')
            {
                break;
            }
            path += recvBuffer[i];
        }
        cout << path << endl;

        string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
        string line;

        string content = renderView(path);
        response += content;

        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
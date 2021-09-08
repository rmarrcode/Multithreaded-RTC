#include "ConnectionInfo.h"

ConnectionInfo::ConnectionInfo(int listening, int clientSocket, sockaddr_in client) {
    _listening = listening;
    _clientSocket = clientSocket;
    _client = client;
}
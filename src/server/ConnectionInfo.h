#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
using namespace std;

class ConnectionInfo {
    public:
        ConnectionInfo(int listening, int clientSocket, sockaddr_in client);
        int listening() {return _listening;};
        int clientSocket() {return _clientSocket;};
        sockaddr_in client() {return _client;};
    private:
        int _listening;
        int _clientSocket;
        sockaddr_in _client;
};
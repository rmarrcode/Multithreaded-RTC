#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <pthread.h> 

#include "ConnectionInfo.h"

using namespace std;

void* handleConnection(void* infovdptr) {
    ConnectionInfo info = *(ConnectionInfo *)infovdptr;

    int listening = info.listening();
    int clientSocket = info.clientSocket();
    sockaddr_in client = info.client();

    // start of func
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
    
    memset(host, 0, NI_MAXHOST); 
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    close(listening);

    // While loop: accept and echo message back to client
    char buf[4096];

    while (true) {
        memset(buf, 0, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0) {
            cout << "Client disconnected " << endl;
            break;
        }

        cout << string(buf, 0, bytesReceived) << endl;

        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    // Close the socket
    close(clientSocket);
    //end of func
}

int main(int argc, char *argv[]) {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
    // Bind the ip address and port to a socket
    // TODO: separate socket class, find avail socket?
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    
    bind(serverSocket, (sockaddr*)&hint, sizeof(hint));

    listen(serverSocket, SOMAXCONN);
    
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    
    while(1) {
        int clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
        ConnectionInfo info(serverSocket, clientSocket, client);
        pthread_t thread;
        pthread_create(&thread, NULL, handleConnection, (void *)&info);
    }
    return 0;
}
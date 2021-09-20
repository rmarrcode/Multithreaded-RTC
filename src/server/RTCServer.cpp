#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <pthread.h> 

using namespace std;

//TODO: record all available sockets and communicate to them 
void* handleConnection(void* p_clientSocket) {
    int clientSocket = *(int *)p_clientSocket;
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // port the client is connect on  
    memset(host, 0, NI_MAXHOST); 
    memset(service, 0, NI_MAXSERV);

    cout << "connected on " << clientSocket << endl;
       
    // accept and echo message back to client
    char buf[4096];
    while (true) {
        memset(buf, 0, 4096);
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
    close(clientSocket);
}

//TODO: handle failures
int main(int argc, char *argv[]) {
    //Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in sockInfo;
    sockInfo.sin_family = AF_INET;
    sockInfo.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &sockInfo.sin_addr);
    bind(serverSocket, (sockaddr*)&sockInfo, sizeof(sockInfo));
    listen(serverSocket, SOMAXCONN);
    
    // Wait for a connection
    // Create new thread and handle
    while(1) {
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        int clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
        pthread_t thread;
        pthread_create(&thread, NULL, handleConnection, &clientSocket);
    }
    return 0;
}
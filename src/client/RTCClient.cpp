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

void* toServer(void* info) {
    pair<string, int>* pairPtr = (pair<string,int>*)info;
    string username = pairPtr->first;
    int sock = pairPtr->second; 
    char buf[4096];
    string userInput;
    while(1) {
        getline(cin, userInput);
        //Send to server
        string msg = username + " :: " + userInput; 
        int sendRes = send(sock, msg.c_str(), msg.size() + 1, 0);
        if (sendRes == -1) {
            cout << "Could not send to server!\n";
            continue;
        }
    }
    return NULL;
}

void* fromServer(void* info) {    
    int sock = *(int *)info;
    char buf[4096];
    while(1) {
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            cout << "There was an error getting a response from server\n";
        }
        //Display response
        else {
            cout << string(buf, bytesReceived) << "\n";
        }
    }  
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "use format ./client <username>\n";
        return 1;
    }
    string username = argv[1];
    //Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return 1;
    }

    //Create a hint structure for the server we're connecting with
    int port = 54000;
    string ipAddress = "0.0.0.0";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        return 1;
    }
    
    pair<string, int> pairPtr = {username, sock};
    pthread_t toServerThread;
    pthread_create(&toServerThread, NULL, toServer, &pairPtr);
    

    
    pthread_t fromServerThread;
    pthread_create(&fromServerThread, NULL, fromServer, &sock);
    

    while (1) {}

    //Close the socket
    close(sock);

    return 0;
}

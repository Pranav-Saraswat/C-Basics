#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>

#define PORT 8080
#define MAX_CLIENTS 4

void swapValues(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void handleClient(int clientSocket) {
    int x, y;

    // Receive values from the client
    recv(clientSocket, &x, sizeof(int), 0);
    recv(clientSocket, &y, sizeof(int), 0);

    // Perform the swap
    swapValues(&x, &y);

    // Send the result back to the client
    send(clientSocket, &x, sizeof(int), 0);
    send(clientSocket, &y, sizeof(int), 0);

    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket, opt = 1;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse the address and port
    #include <netinet/in.h>

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept and handle incoming connections
    while (1) {
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Handle the client in a separate thread or process
        if (fork() == 0) {
            close(serverSocket); // Child process doesn't need the listening socket
            handleClient(clientSocket);
            exit(0);
        }

        close(clientSocket); // Parent process doesn't need the client socket
    }

    close(serverSocket);

    return 0;
}

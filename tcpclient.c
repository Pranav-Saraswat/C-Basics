#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    serverAddr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    int x = 5, y = 9;

    // Send values to the server
    send(clientSocket, &x, sizeof(int), 0);
    send(clientSocket, &y, sizeof(int), 0);

    // Receive swapped values from the server
    recv(clientSocket, &x, sizeof(int), 0);
    recv(clientSocket, &y, sizeof(int), 0);

    printf("Swapped values: X = %d, Y = %d\n", x, y);

    close(clientSocket);

    return 0;
}

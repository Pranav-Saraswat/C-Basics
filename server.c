#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024


int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Receive five integer numbers from the client
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        int num = atoi(buffer);
        printf("Received number %d from client\n", num);
        sum += num;
    }

    // Calculate average
    float average = (float)sum / 5;

    // Send the average back to the client
    sprintf(buffer, "%f", average);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, client_len);

    printf("Average sent to client: %f\n", average);

    // Close the socket
    #include <unistd.h>

    // ...

    close(sockfd);

    return 0;
}

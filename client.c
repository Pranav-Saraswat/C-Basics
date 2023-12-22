#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
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

    // Send five integer numbers to the server
    for (int i = 0; i < 5; ++i) {
        int num;
        printf("Enter number %d: ", i + 1);
        scanf("%d", &num);
        sprintf(buffer, "%d", num);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    // Receive the average from the server
    recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    float average = atof(buffer);

    printf("Received average from server: %f\n", average);

    // Close the socket
    close(sockfd);

    return 0;
}

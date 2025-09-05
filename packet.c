#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1259

struct DLIHeader
{
    uint16_t PacketSize;
    uint8_t  MessageID;
    uint8_t  Source;
    uint32_t SeqNo;
    uint64_t TimeStamp;
    uint16_t VersionMajor;
    uint16_t VersionMinor;
} __attribute__((packed));


struct DLIMessageIncoming
{
    struct DLIHeader Header;
    uint32_t TimeslotNumber;
    uint8_t EpochNumber;
    uint16_t NPG;
    uint8_t PackingStructure;
    uint16_t SourceTrackNumber;
    uint64_t MsgSendTime;
    uint8_t NumJWords;
    uint8_t WordArray[35]; // WordArray[NumJWords];
};


int main( int argc, char **argv )
{
    int sockfd;
    struct sockaddr_in server_addr;

    char text[] = "CMSG CMSG CMSG CMSG CMSG CMSG CMSG";

    struct DLIMessageIncoming message = {
        .Header = {
            sizeof(struct DLIMessageIncoming),
            7, 10, 0, 0, 0, 0
        },
        0, 0, 0, 0, 0, 0, 35
    };
    memcpy(message.WordArray, text, sizeof(text));

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr.sin_port = htons(SERVER_PORT); // Convert port to network byte order



    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send the message
    ssize_t bytes_sent = sendto(
        sockfd, &message, sizeof(message), 0,
        (const struct sockaddr *)&server_addr,
        sizeof(server_addr)
    );

    if (bytes_sent < 0)
    {
        perror("Error sending UDP packet");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // printf("UDP packet sent to %s:%d with message: \"%s\"\n", SERVER_IP, SERVER_PORT, message);

    // Close the socket
    close(sockfd);

    return 0;
}


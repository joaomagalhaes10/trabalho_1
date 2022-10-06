#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "ModbusAP.h"
#include "ModbusTCP.h"

#define IP "127.0.0.1"
#define SERVER_PORT 5502
#define IN_BUF_LEN 100

int main()
{
    int socket_communication, in;
    struct sockaddr_in server;
    char in_buf[IN_BUF_LEN], out_buf[IN_BUF_LEN];

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (inet_aton(IP, &server.sin_addr) == 0)
    {
        printf("ERROR 1: client.c -> main");
        exit(1);
    }


}
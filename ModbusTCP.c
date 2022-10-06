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

int Send_Modbus_request()
{
    // Connect server
    if (connect(socket_communication, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Connection failed...\n");
        return -1;
    }
    else
    {
        printf("Successfully connected to server...\n");
    }

    while (1)
    {
        bzero(in_buf, IN_BUF_LEN);
        bzero(out_buf, IN_BUF_LEN);

        // Waiting for client data
        printf("<What do you want to say?> ");
        fgets(out_buf, IN_BUF_LEN, stdin);
        size_buff = strlen(out_buf);

        out = send(socket_communication, out_buf, size_buff, 0);
        if (out < 0)
        {
            printf("Send failed...\n");
            return -1;
        }
        else
        {
            printf("Sent data (%d bytes): %s\n", size_buff, out_buf);
        }

        in = recv(socket_communication, in_buf, size_buff, 0);
        if (in < 0)
        {
            printf("Recv failed...\n");
            return -1;
        }
        else
        {
            printf("Received data (%d bytes): %s\n", size_buff, in_buf);
        }
    }
}
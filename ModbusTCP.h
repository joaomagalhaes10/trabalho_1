#ifndef _MODBUSTCP_H
#define _MODBUSTCP_H

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


/*********************
 * FUNCTIONS HEADERS *
 *********************/
/*******************************************************************************************
 * SEND_MODBUS_REQUEST()
 * Send the client's request to the server.
 * Arguments description:
 *  - server_add - struct that contains the server's address;
 *  - port - port that will be used for the connection;
 *  - APDU - it's the stream that will be send to the server (value passed by the
 *      argument);
 *  - APDUlen - length of the stream on the application layer (value passed by the 
 *      argument);
 *  - APDU_R - stream relative to the server's response (value returned by the argument).
 *******************************************************************************************/
int Send_Modbus_request(struct sockaddr_in * server_add, int port, uint8_t * APDU, uint16_t APDUlen, uint8_t * APDU_R, uint16_t * APDU_Rlen);
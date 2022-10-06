#ifndef _MODBUSAP_H
#define _MODBUSAP_H

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

#include "ModbusTCP.h"

/*********************
 * FUNCTIONS CODES
 *********************/
#define CODE_R_HOLD_REGS 0x03
#define CODE_W_MULT_REGS 0x10




/*********************
 * FUNCTIONS HEADERS *
 *********************/
/*******************************************************************************************
 * READ_H_REGS()
 * Execute the function of reading holding registers in the server.
 * Arguments description:
 *  - server_add - struct that contains the server's address;
 *  - port - port that will be used for the connection;
 *  - st_r = start address of the holding registers - 1, because of uint16_t that can only
 *      store values from 0 to 65535;
 *  - n_r - number of holding registers to be read by the server;
 *  - val - vector that will be returned with the pretended values. For reference, the
 *      length of the vector is n_r.
 * Returns: number of read registers - ok, <0 - error
 *******************************************************************************************/
int Read_h_regs(struct sockaddr_in * server_add, int port, uint32_t st_r, uint16_t n_r, uint16_t * val);

/*******************************************************************************************
 * WRITE_MULTIPLE_REGS()
 * Execute the function of write holding registers in the server.
 * Arguments description:
 *  - server_add - struct that contains the server's address;
 *  - port - port that will be used for the connection;
 *  - st_r = start address of the holding registers - 1, because of uint16_t that can only
 *      store values from 0 to 65535;
 *  - n_r - number of holding registers to be written by the server;
 *  - val - vector that is passed with the pretended values. For reference, the
 *      length of the vector is n_r.
 * Returns: number of written registers - ok, <0 - error
 *******************************************************************************************/
int Write_multiple_regs(struct sockaddr_in * server_add, int port, uint32_t st_r, uint16_t n_r, uint16_t * val);
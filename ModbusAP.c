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
#include "ModbusAP.h"

int Read_h_regs(struct sockaddr_in *server_add, int port, uint32_t st_r, uint16_t n_r, uint16_t *val)
{
    // CHECK CONSISTENCY OF PARAMETERS
    if (server_add == NULL)
    {
        printf("ERROR 1: ModbusAP.c -> Read_h_regs\n");
        return -1;
    }
    else if ((port < 0) || (port > 65535))
    {
        printf("ERROR 2: ModbusAP.c -> Read_h_regs\n");
        return -2;
    }
    else if ((st_r < 1) || (st_r > 65536))
    {
        printf("ERROR 3: ModbusAP.c -> Read_h_regs\n");
        return -3;
    }
    else if ((uint32_t)n_r > 65536 - 1 + 1)
    {
        printf("ERROR 4: ModbusAP.c -> Read_h_regs\n");
        return -4;
    }
    else if (st_r + (uint32_t)n_r - 1 > 65536)
    {
        printf("ERROR 5: ModbusAP.c -> Read_h_regs\n");
        return -5;
    }

    // create variables
    uint8_t APDU[256], APDU_R[256]; // APDU -> SENT, APDU_R -> RECEIVED
    uint8_t APDU_function;
    uint16_t APDUlen, APDU_Rlen;
    int i, count1, count2;

    // assembles APDU (MODBUS PDU)
    APDU_function = CODE_R_HOLD_REGS;
    APDUlen = 5;

    st_r = st_r - 1;

    APDU[0] = (uint8_t)APDU_function; // funtion code
    APDU[1] = (uint8_t)st_r >> 8;     // starting address - HIGH
    APDU[2] = (uint8_t)st_r & 0x00FF; // starting address - LOW
    APDU[3] = (uint8_t)n_r >> 8;      // quantity of registers - HIGH
    APDU[4] = (uint8_t)n_r & 0x00FF;  // quantity of registers - LOW

    printf("APDU in Read_h_regs: [");
    for (i = 0; i < APDUlen; i++)
    {
        if (i < APDUlen - 1)
        {
            printf(" %u |", APDU[i]);
        }
        else
        {
            printf(" %u ]\n", APDU[i]);
        }
    }

    // send modbus request
    if (Send_Modbus_request(server_add, port, APDU, APDUlen, APDU_R, &APDU_Rlen) < 0)
    {
        printf("ERROR 6: ModbusAP.c -> Read_h_regs -> Send_Modbus_request\n");
        return -6;
    }

    // check APDU reply (APDU_R)
    printf("APDU_R in Read_h_regs: [");
    for (i = 0; i < APDU_Rlen; i++)
    {
        if (i < APDU_Rlen - 1)
        {
            printf(" %u |", APDU_R[i]);
        }
        else
        {
            printf(" %u ]\n", APDU_R[i]);
        }
    }

    if (APDU_R[0] == CODE_R_HOLD_REGS + 0x80) // error code 0x83
    {
        switch (APDU_R[1])
        {
        case 0x01:
            printf("ERROR, ILLEGAL FUNCTION: ModbusAP.c -> Read_h_regs");
            return -101;
            break;

        case 0x02:
            printf("ERROR, ILLEGAL DATA ADDRESS: ModbusAP.c -> Read_h_regs");
            return -102;
            break;

        case 0x03:
            printf("ERROR, ILLEGAL DATA VALUE: ModbusAP.c -> Read_h_regs");
            return -103;
            break;

        default:
            printf("ERROR, DEFAULT: ModbusAP.c -> Read_h_regs");
            return -100;
            break;
        }
    }

    // saving the register value from APDU_R
    count2 = 2;
    for (count1 = 0; count1 < n_r; count1++)
    {
        val[count1] = (APDU_R[count2] << 8) + APDU_R[count2 + 1];
        printf("%u + %u = %u\n", APDU_R[count2] << 8, APDU_R[count2 + 1], val[count1]);
        count2 += 2;
    }

    return n_r;
}

int Write_multiple_regs(struct sockaddr_in *server_add, int port, uint32_t st_r, uint16_t n_r, uint16_t *val)
{
    // CHECK CONSISTENCY OF PARAMETERS
    if (server_add == NULL)
    {
        printf("ERROR 1: ModbusAP.c -> Write_multiple_regs\n");
        return -1;
    }
    else if ((port < 0) || (port > 65535))
    {
        printf("ERROR 2: ModbusAP.c -> Write_multiple_regs\n");
        return -2;
    }
    else if ((st_r < 1) || (st_r > 65536))
    {
        printf("ERROR 3: ModbusAP.c -> Write_multiple_regs\n");
        return -3;
    }
    else if ((uint32_t)n_r > 65536 - 1 + 1)
    {
        printf("ERROR 4: ModbusAP.c -> Write_multiple_regs\n");
        return -4;
    }
    else if (st_r + (uint32_t)n_r - 1 > 65536)
    {
        printf("ERROR 5: ModbusAP.c -> Write_multiple_regs\n");
        return -5;
    }
    else if (val == NULL)
    {
        printf("ERROR 6: ModbusAP.c -> Write_multiple_regs\n");
        return -6;
    }

    // create variables
    uint8_t APDU[256], APDU_R[256]; // APDU -> SENT, APDU_R -> RECEIVED
    uint8_t APDU_function;
    uint16_t APDUlen, APDU_Rlen;
    int i, count1, count2;

    // assembles APDU (MODBUS PDU)
    APDU_function = CODE_W_MULT_REGS;
    APDUlen = n_r * 2 + 6;

    st_r = st_r - 1;

    APDU[0] = (uint8_t)APDU_function; // funtion code
    APDU[1] = (uint8_t)st_r >> 8;     // starting address - HIGH
    APDU[2] = (uint8_t)st_r & 0x00FF; // starting address - LOW
    APDU[3] = (uint8_t)n_r >> 8;      // quantity of registers - HIGH
    APDU[4] = (uint8_t)n_r & 0x00FF;  // quantity of registers -LOW
    APDU[5] = (uint8_t)(n_r * 2);     // byte count

    

    printf("APDU in Write_multiple_regs: [");
    for (i = 0; i < APDUlen; i++)
    {
        if (i < APDUlen - 1)
        {
            printf(" %u |", APDU[i]);
        }
        else
        {
            printf(" %u ]\n", APDU[i]);
        }
    }
}
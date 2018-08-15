#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PCAN_CHANNEL PCAN_USBBUS1
#define PCAN_BAUDRATE PCAN_BAUD_500K

#ifndef __APPLE__
#include <asm/types.h>
#define DWORD  __u32
#define WORD   unsigned short
#define BYTE   unsigned char
#define LPSTR  char*
#include "PCANBasic.h"
#else
#include "PCBUSB.h"
#endif

#define APM_En            (BYTE)0x01
#define APM_D_En          (BYTE)0x00
#define ASM_En            (BYTE)0x02
#define ASM_D_En          (BYTE)0x00
#define APM_IGNORE        (BYTE)0x4
#define ACC_StopReq_En    (BYTE)0x01
#define ACC_StopReq_D_En  (BYTE)0x00

int main(int argc, char *argv[])
{
    TPCANMsg message;
    TPCANStatus status;
    unsigned long n = 0;
    
    status = CAN_Initialize(PCAN_CHANNEL, PCAN_BAUDRATE, 0, 0, 0);
    printf("Initialize CAN: 0x%lx\n", status);
    if(status != PCAN_ERROR_OK) goto leave;
    
    message.ID = 0x156;
    message.LEN = 8;
    message.MSGTYPE = PCAN_MESSAGE_STANDARD;
    memset(message.DATA, 0x00, message.LEN);
    
    for(;;) {
        while((status = CAN_Write(PCAN_CHANNEL, &message)) == PCAN_ERROR_OK) {
            message.DATA[0] = APM_En;
            message.DATA[1] = 2 * 0.5;
            message.DATA[2] = 2;
            message.DATA[3] = 0;
            message.DATA[4] = 0;
            message.DATA[5] = 0;
            message.DATA[6] = 0;
            message.DATA[7]++;
        }
    }
leave:
    CAN_Uninitialize(PCAN_NONEBUS);
    return 0;
}

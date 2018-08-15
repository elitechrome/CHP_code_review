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
    if(status != PCAN_ERROR_OK)
    {
        printf("PEAK_CAN ERROR\n");
    }//goto leave;
    
    message.ID = 0x156;
    message.LEN = 8;
    message.MSGTYPE = PCAN_MESSAGE_STANDARD;
    memset(message.DATA, 0x00, message.LEN);
    
    
    
    
    

    TPCANMsg message_Mo_Val;
    TPCANStatus status_Mo_Val;
    unsigned long n_Mo_Val = 0;

    status_Mo_Val = CAN_Initialize(PCAN_CHANNEL, PCAN_BAUDRATE, 0, 0, 0);
    printf("Initialize CAN: 0x%lx\n", status_Mo_Val);
    if(status_Mo_Val != PCAN_ERROR_OK)
    {
        printf("PEAK_CAN ERROR_Mo_Val\n");
    }//goto leave;

    message_Mo_Val.ID = 0x157;
    message_Mo_Val.LEN = 8;
    message_Mo_Val.MSGTYPE = PCAN_MESSAGE_STANDARD;
    memset(message_Mo_Val.DATA, 0x00, message_Mo_Val.LEN);
    
    
    
    u_int16_t steer_angle_ = 20;    // 0x14 // value * 10 => 0xc8
    u_int16_t accel_value_ = -2;
    
    for(;;)
    {
//        while((status = CAN_Write(PCAN_CHANNEL, &message)) == PCAN_ERROR_OK) {
//
//            message.DATA[0] = APM_En;
//            message.DATA[1] = 2 * 0.5;
//            message.DATA[2] = 2;
//            message.DATA[3] = 0;
//            message.DATA[4] = 0;
//            message.DATA[5] = 0;
//            message.DATA[6] = 0;
//            message.DATA[7]++;
//        }
        
        //status = CAN_Write(PCAN_CHANNEL, &message);
        
        message.DATA[0] = APM_En;
        message.DATA[1] = 2 * 0.5;
        message.DATA[2] = 2;
        message.DATA[3] = 0;
        message.DATA[4] = 0;
        message.DATA[5] = 0;
        message.DATA[6] = 0;
        message.DATA[7]++;
        
        if((status = CAN_Write(PCAN_CHANNEL, &message)) != PCAN_ERROR_OK) printf("test\n");

        
            
            message_Mo_Val.DATA[0] = (u_int8_t)(((10*steer_angle_) & 0x00ff));
            message_Mo_Val.DATA[1] = (u_int8_t)((((10*steer_angle_) & 0xff00) >> 8));
            message_Mo_Val.DATA[2] = 100;
            message_Mo_Val.DATA[3] = (u_int8_t)((u_int16_t)(100 * (accel_value_ + 10.23)) & 0x00ff);
            message_Mo_Val.DATA[4] = (u_int8_t)(((u_int16_t)(100 * (accel_value_ + 10.23)) >> 8) & 0x00ff);
            message_Mo_Val.DATA[5] = 0;
            message_Mo_Val.DATA[6] = 0;
            message_Mo_Val.DATA[7] = 0;

            if((status_Mo_Val = CAN_Write(PCAN_CHANNEL, &message_Mo_Val)) != PCAN_ERROR_OK) printf("test mo\n");
        
//            printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %d\n",
//                   (int) message.ID, (int) message.LEN, (int) message.DATA[0],
//                   (int) message.DATA[1], (int) message.DATA[2],
//                   (int) message.DATA[3], (int) message.DATA[4],
//                   (int) message.DATA[5], (int) message.DATA[6],
//                   (int) message.DATA[7]);
    }
//leave:
//    printf("why???\n");
//    CAN_Uninitialize(PCAN_NONEBUS);
    return 0;
}

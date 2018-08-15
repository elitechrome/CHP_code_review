#include "can_mac_val.h"

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
    TPCANHandle dev_name;
    SOCKETCAN_FRAME frame;
    
    int fd;
    fd_set fds;
    
    status = CAN_Initialize(PCAN_USBBUS1, PCAN_BAUDRATE, 0, 0, 0);
    printf("Initialize CAN: 0x%lx\n", status);
    if(status != PCAN_ERROR_OK)
    {
        perror("PCAN_ERROR\n");
    }
    

    unsigned char count = 0;
    while(1)
    {
        u_int8_t APM_Slevel_val = 2;
        
        //        can.can_memset();
        
        //    printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
        //           (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
        //           (int) can.frame.data[1], (int) can.frame.data[2],
        //           (int) can.frame.data[3], (int) can.frame.data[4],
        //           (int) can.frame.data[5], (int) can.frame.data[6],
        //           (int) can.frame.data[7]);
        
//        unsigned long tmp_id = 0x156;
//        unsigned char tmp_dlc = 8;
        
        memset(&message, 0, sizeof(TPCANMsg));
        
        message.ID = 0x156;
        message.LEN = 8;
        message.MSGTYPE = PCAN_MESSAGE_STANDARD;

//        can.write_param(tmp_id, tmp_dlc, PCAN_MESSAGE_STANDARD);
        //    can.frame.can_id = 0x156;
        //    can.frame.can_dlc = 8;
        
        message.DATA[0] = APM_En;                     // APM_IGNORE, APM_En
        message.DATA[1] = APM_Slevel_val * 0.5;
        message.DATA[2] = 2;//(u_int8_t)0x10;         // ASM_StopRequest(Req AEB action), ASM_En(En : 0x10, D_En : 0x00)
        message.DATA[3] = 0;
        message.DATA[4] = 0;
        message.DATA[5] = 0;
        message.DATA[6] = 0;
        message.DATA[7] = count++;                           // alive-count    can.write();
//        message.DATA[7]++;

        status = CAN_Write(PCAN_USBBUS1, &message);

//        printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %d\n",
//               (int) message.ID, (int) message.LEN, (int) message.DATA[0],
//               (int) message.DATA[1], (int) message.DATA[2],
//               (int) message.DATA[3], (int) message.DATA[4],
//               (int) message.DATA[5], (int) message.DATA[6],
//               (int) message.DATA[7]);
        
        
        
        
        
        
        // 입력 이벤트가 변할 때 만 출력하는 코드
        status = CAN_GetValue(PCAN_CHANNEL, PCAN_RECEIVE_EVENT, &fd, sizeof(int));
        
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        while(select(fd+1, &fds, NULL, NULL, NULL) > 0) break;
        status = CAN_Read(PCAN_CHANNEL, &message, NULL);
        
        if (status != PCAN_ERROR_OK) {
            printf("Error 0x%lx\n", status);
        }
        
        
        if(message.ID == 0x711)
        {
            printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %d\n",
                   (int) message.ID, (int) message.LEN, (int) message.DATA[0],
                   (int) message.DATA[1], (int) message.DATA[2],
                   (int) message.DATA[3], (int) message.DATA[4],
                   (int) message.DATA[5], (int) message.DATA[6],
                   (int) message.DATA[7]);
            
            // id = 0x711
            bool ASM_Fd_En_         = 0;        // ASM Manual/Auto 모드 변환 요청 피드백
            u_int8_t ASM_Fd_VSpeed_  = 0;        // ASM vehicle speed output
            u_int8_t Mo_Fd_AlvCnt_   = 0;        // Alive-count feedback value
            
            ASM_Fd_En_          = message.DATA[0] & 0x1;
            ASM_Fd_VSpeed_       = message.DATA[1] & 0xff;
            Mo_Fd_AlvCnt_        = message.DATA[7] & 0xff;
            
            
            //printf("ASM_Fd_En : %d\n", ASM_Fd_En_);
            if(ASM_Fd_En_ == true)      printf("ASM Enable flag\n");
            else                        printf("ASM Disable flag\n");
            
            printf("Vehicle Speed \t: %d\n", ASM_Fd_VSpeed_);
            printf("Alive-count value \t: %d\n", Mo_Fd_AlvCnt_);
            
        }
        
//        can.frame.data[0] = APM_En;                   // APM_IGNORE, APM_En
//        can.frame.data[1] = APM_Slevel_val * 0.5;
//        can.frame.data[2] = 0;//(u_int8_t)0x10;         // ASM_StopRequest(Req AEB action), ASM_En(En : 0x10, D_En : 0x00)
//        can.frame.data[3] = 0;
//        can.frame.data[4] = 0;
//        can.frame.data[5] = 0;
//        can.frame.data[6] = 0;
//        can.frame.data[7] = count++;                           // alive-count    can.write();
//        //        can.frame.data[7]++;
        
//        printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %d\n",
//               (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
//               (int) can.frame.data[1], (int) can.frame.data[2],
//               (int) can.frame.data[3], (int) can.frame.data[4],
//               (int) can.frame.data[5], (int) can.frame.data[6],
//               (int) can.frame.data[7]);
        
        
//        can.can_write();
        //sleep(1);
    }
    
    return 0;
}

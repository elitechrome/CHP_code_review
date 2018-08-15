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
//    CAN_AVL can(PCAN_USBBUS1);
//    CAN_AVL can_read(PCAN_USBBUS1);
//    can.write_param(0x111, 8);  // message에 씀
    
    
    
    
    
    
    TPCANMsg message;
    TPCANStatus status;
    unsigned long n = 0;
    
    status = CAN_Initialize(PCAN_USBBUS1, PCAN_BAUD_500K, 0, 0, 0);
    printf("Initialize CAN: 0x%lx\n", status);
    if(status != PCAN_ERROR_OK) {};
    
    message.ID = 0x156;
    message.LEN = 8;
    message.MSGTYPE = PCAN_MESSAGE_STANDARD;
    memset(message.DATA, 0x00, message.LEN);
    
    for(;;) {

    }
    
    
    
    
    
    
    unsigned char count = 0;
    while(1)
    {
//        can.can_write();    // message에 쓰고 frame으로 넘김
//        //can.frame.can_id  = 0x123;
//        //can.frame.can_dlc = 8;
//        can.frame.data[0] = 0x00;
//        can.frame.data[1] = 0x23;
//        can.frame.data[2] = 0x34;
//        can.frame.data[3] = 0x45;
//        can.frame.data[4] = 0x56;
//        can.frame.data[5] = 0x67;
//        can.frame.data[6] = 0x78;
//        can.frame.data[7] = count;
//        sleep(1);
//
//        can.SOCKETCAN_TO_PEAKCAN();
//        printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
//               (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
//               (int) can.frame.data[1], (int) can.frame.data[2],
//               (int) can.frame.data[3], (int) can.frame.data[4],
//               (int) can.frame.data[5], (int) can.frame.data[6],
//               (int) can.frame.data[7]);
//
//        //printf(" %02x\n", can.message.DATA[7]);
//        // write(***) 만 써도 들어감. 다만 리턴값이 바이트를 출력하기 때문에 nbytes로 받을 수 있는 것.
//        //can.nbytes = write(can.s, &can.frame, sizeof(struct can_frame));
//        //printf("Wrote %d bytes,\t\t count = %d\n", nbytes, count);
//        //std::cout << "Wrote " << can.nbytes << " bytes,\t\t count = " << count << std::endl;
//
//        count++;

        
        
        
        
        
        
//
//        // Mo_Conf (Module Configuration)
//        //  used when write some command
//        u_int8_t APM_Slevel_val = 2;
//
//        //    memset(can.message.DATA, 0, sizeof(BYTE)*8);
//        //    memset(&can.message, 0, sizeof(TPCANMsg));
//        //    can.PEAKCAN_TO_SOCKETCAN();
//
//        can.can_memset();
//
//        //    printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
//        //           (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
//        //           (int) can.frame.data[1], (int) can.frame.data[2],
//        //           (int) can.frame.data[3], (int) can.frame.data[4],
//        //           (int) can.frame.data[5], (int) can.frame.data[6],
//        //           (int) can.frame.data[7]);
//
//        unsigned long tmp_id = 0x156;
//        unsigned char tmp_dlc = 8;
//        can.write_param(tmp_id, tmp_dlc, PCAN_MESSAGE_STANDARD);
//        //    can.frame.can_id = 0x156;
//        //    can.frame.can_dlc = 8;
//
//        can.frame.data[0] = APM_En;                   // APM_IGNORE, APM_En
//        can.frame.data[1] = APM_Slevel_val * 0.5;     // APM_Slevel(Steering angular speed level) 0.5*(real_value), [0, 510]
//        can.frame.data[2] = 2;         // ASM_StopRequest(Req AEB action), ASM_En(En : 0x10, D_En : 0x00)
//        can.frame.data[3] = 0;
//        can.frame.data[4] = 0;
//        can.frame.data[5] = 0;
//        can.frame.data[6] = 0;
//        //can.message.DATA[2] = 0x1;
//        can.frame.data[7] = count++;                           // alive-count    can.write();
////        can.frame.data[7]++;
//
////        printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %d\n",
////               (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
////               (int) can.frame.data[1], (int) can.frame.data[2],
////               (int) can.frame.data[3], (int) can.frame.data[4],
////               (int) can.frame.data[5], (int) can.frame.data[6],
////               (int) can.frame.data[7]);
//
//
//        can.can_write();
        
        
        
        
        
        
        
        
        while((status = CAN_Write(PCAN_CHANNEL, &message)) == PCAN_ERROR_OK) {
            message.DATA[0] = APM_En;
            message.DATA[1] = 2 * 0.5;
            message.DATA[2] = 0;
            message.DATA[3] = 0;
            message.DATA[4] = 0;
            message.DATA[5] = 0;
            message.DATA[6] = 0;
            message.DATA[7]++;
            
        }
        
        
        
        
        
        
        
        
        
        
        
        
//        can_read.can_read();
//
//        if(can_read.frame.can_id == 0x711)
//        {
//                    printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %d\n",
//                           (int) can_read.frame.can_id, (int) can_read.frame.can_dlc, (int) can_read.frame.data[0],
//                           (int) can_read.frame.data[1], (int) can_read.frame.data[2],
//                           (int) can_read.frame.data[3], (int) can_read.frame.data[4],
//                           (int) can_read.frame.data[5], (int) can_read.frame.data[6],
//                           (int) can_read.frame.data[7]);
//
//            // id = 0x711
//            bool ASM_Fd_En_         = 0;        // ASM Manual/Auto 모드 변환 요청 피드백
//            u_int8_t ASM_Fd_VSpeed_  = 0;        // ASM vehicle speed output
//            u_int8_t Mo_Fd_AlvCnt_   = 0;        // Alive-count feedback value
//
//            ASM_Fd_En_          = can_read.frame.data[0] & 0x1;
//            ASM_Fd_VSpeed_       = can_read.frame.data[1] & 0xff;
//            Mo_Fd_AlvCnt_        = can_read.frame.data[7] & 0xff;
//
//
//            //printf("ASM_Fd_En : %d\n", ASM_Fd_En_);
//            if(ASM_Fd_En_ == true)      printf("ASM Enable flag\n");
//            else                        printf("ASM Disable flag\n");
//
//            printf("Vehicle Speed \t: %d\n", ASM_Fd_VSpeed_);
//            printf("Alive-count value \t: %d\n", Mo_Fd_AlvCnt_);
//
//        }
        
        usleep(1);
        //usleep(20000);
    }
    
	return 0;
}

#include "can_mac_val.h"

int main(int argc, char *argv[]) 
{
    CAN_AVL can(PCAN_USBBUS1);

    while(1)
    {
        can.can_read();

        if(can.frame.can_id == 0x710)
        {
            printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
                   (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
                   (int) can.frame.data[1], (int) can.frame.data[2],
                   (int) can.frame.data[3], (int) can.frame.data[4],
                   (int) can.frame.data[5], (int) can.frame.data[6],
                   (int) can.frame.data[7]);
        }
        if(can.frame.can_id == 0x711)
        {
            printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
                   (int) can.frame.can_id, (int) can.frame.can_dlc, (int) can.frame.data[0],
                   (int) can.frame.data[1], (int) can.frame.data[2],
                   (int) can.frame.data[3], (int) can.frame.data[4],
                   (int) can.frame.data[5], (int) can.frame.data[6],
                   (int) can.frame.data[7]);
        }
//        can.PEAKCAN_TO_SOCKETCAN();

    }
    
	return 0;
}


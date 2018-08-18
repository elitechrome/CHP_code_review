/*

    필수!!!!!!! CAN 연결 순서 체크하고 진행하기
    (제어캔, 샤시캔 구분하는 방법 찾아서 캔 연결 순서를 무관하게 만들어보기)

 */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#include "can_mac_val.h"
#include "pd_control.h"

#define APM_En            (BYTE)0x01
#define APM_D_En          (BYTE)0x00
#define ASM_En            (BYTE)0x02
#define ASM_D_En          (BYTE)0x00
#define APM_IGNORE        (BYTE)0x4
#define ACC_StopReq_En    (BYTE)0x01
#define ACC_StopReq_D_En  (BYTE)0x00

#define CONTROL_CAN_DEVICE PCAN_USBBUS1
#define CHASSIS_CAN_DEVICE PCAN_USBBUS1
#define PCAN_BAUD_RATE PCAN_BAUD_500K

#define CAN_CONTROL_ENABLE
//#define CAN_CHASSIS_ENABLE

// print_ control can flag
bool print_can_control_state_flag = 0;
bool print_vehicle_speed_flag = 0;
bool print_alive_count_flag = 0;
bool print_steer_angle_flag = 0;
bool print_cluster_speed_display_flag = 1;

// print_ chassis can flag
bool print_driving_tq_flag = 0;
bool print_cluster_engine_rpm_flag = 0;
bool print_new_msg_engine_rpm_flag = 0;         // motor rpm인듯
bool print_rpm_without_electro_flag = 0;        // engine rpm인듯
bool print_brake_pressure_flag = 0;
bool print_whl_spd_flag = 0;
bool print_whl_pul_flag = 0;

// state
u_int16_t APM_state = APM_D_En;   // APM_D_En, APM_En
u_int16_t ASM_state = ASM_D_En;   // ASM_D_En, ASM_En

// control param
u_int8_t APM_Slevel_val = 250;  // 100 ~ 250, (if this value set to 0, APM set to 150)
u_int16_t Steer_Cmd = 20;    // 0x14 // value * 10 => 0xc8
float aReqMax_Cmd = -1.;       // -5.00 ~ 5.00 소수점 2째 자리까지 가능
int cluster_speed_display_value = 10;

// PD control param
bool speed_pid_control_enable_flag = 0;
int target_vehicle_speed = 20;             // 현재 속도
int real_vehicle_speed = 0;               // 타겟 속도
float K_p = 0.1;
float K_d = 0.5;

// 0.2, 0.5 -> 15

// CAN error flag (전역으로 선언)
bool can_error_flag = 0;

// Control CAN
void Mo_Conf(CAN_AVL &can_);
void Mo_Val(CAN_AVL &can_);
bool Report_APM(CAN_AVL &can_);
bool Report_ASM(CAN_AVL &can_);
bool Report_Misc(CAN_AVL &can_);
void can_print(CAN_AVL can_);

// Chassis CAN
void TCU_DVT13_Driving_Tq(CAN_AVL can_);            // ID : 0x162, LEN : 3
void TCU_DCT13_Engine_RPM(CAN_AVL can_);            // ID : 0x162, LEN : 3
void NEW_MSG_ENGINE(CAN_AVL can_);                  // ID : 0x240, LEN : 8, rpm
void NEW_MSG_2_RPM_WITHOUT_ELECTRO(CAN_AVL can_);   // ID : 0x371, LEN : 8
void Brake_Pressure(CAN_AVL can_);                  // ID : 0x371, LEN : 8
void WHL_Speed(CAN_AVL can_);                       // ID : 0x386, LEN : 8, km/h
void WHL_PUL(CAN_AVL can_);                         // ID : 0x387, LEN : 6

// thread function
void CAN_RW(std::atomic<bool>& , bool);

static BYTE CAN_alive_count = 0;
std::mutex m;

int main()
{
    PD_CONTROL pd_control;
    int status;
    
    const auto wait_duration = std::chrono::milliseconds(10);

    std::atomic<bool> running { true } ;

    // CAN Read Write
    std::thread *th1;
    th1 = new std::thread(CAN_RW, std::ref(running), can_error_flag);

    while(running)
    {
        /*
         차량 제어 코드 작성
         */
        
        // S_t, K_p, K_d는 상수, S_r은 실시간 CAN receive data. S_r에 뮤텍스 들어갈 수 도 있음.
        if(speed_pid_control_enable_flag == 1)
        {
            aReqMax_Cmd = pd_control.MV_Cal_Func(target_vehicle_speed, real_vehicle_speed, K_p, K_d);
            aReqMax_Cmd = aReqMax_Cmd > 5 ? 5 : aReqMax_Cmd;
            aReqMax_Cmd = aReqMax_Cmd < -5 ? -5 : aReqMax_Cmd;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(can_error_flag == true)
        {
            running = false;
        }
    }
    running = false;
    th1->join();

    return 0;
}

void CAN_RW(std::atomic<bool>& _running, bool can_error_flag_)
{
    printf("can_error_flag : %d\n", can_error_flag_);
    
#ifdef CAN_CONTROL_ENABLE
    CAN_AVL can_control(CONTROL_CAN_DEVICE, PCAN_BAUD_RATE);      // 제어 캔. write/read
    memset(&can_control.message, 0, sizeof(TPCANMsg));
    // can_control.PEAKCAN_TO_SOCKETCAN();
#endif
    
#ifdef CAN_CHASSIS_ENABLE
    CAN_AVL can_chassis(CHASSIS_CAN_DEVICE, PCAN_BAUD_RATE);      // 샤시 캔. only read
    memset(&can_chassis.message, 0, sizeof(TPCANMsg));
    can_chassis.PEAKCAN_TO_SOCKETCAN();
#endif
    
    while(_running)
    {
        
#ifdef CAN_CONTROL_ENABLE
        //////////////////////////////////////////////
        ////////////////  can_control  ///////////////
        //////////////////////////////////////////////
        
        // 초기화
        // 0x156 값넣고
        Mo_Conf(can_control);   // id 0x156
        // write
        
        // 초기화
        // 0x157 값 넣고
        Mo_Val(can_control);    // id 0x157
        // write
        
        //        can_control.can_memset();
        can_control.can_read();
        
        //        can_print(can_control);
        
        if(can_control.frame.can_id == 0x710)  
        {
            m.lock();
            can_error_flag = Report_APM(can_control);
            m.unlock();
        }
        if(can_control.frame.can_id == 0x711) 
        {
            m.lock();
            can_error_flag = Report_ASM(can_control);
            m.unlock();
        }
        if(can_control.frame.can_id == 0x71f)  
        {
            m.lock();
            can_error_flag = Report_Misc(can_control);
            m.unlock();
        }
        
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        //////////////////////////////////////////////
#endif
        
        
        
        
        
        
#ifdef CAN_CHASSIS_ENABLE
        //////////////////////////////////////////////
        //////////////// can_chassis /////////////////
        //////////////////////////////////////////////
        
        //can_chassis.can_memset();
        can_chassis.can_read();
        
        if(can_chassis.frame.can_id == 0x162) TCU_DVT13_Driving_Tq(can_chassis);    // 얜 write같음. test 해보진 말 것
        if(can_chassis.frame.can_id == 0x162) TCU_DCT13_Engine_RPM(can_chassis);    // 얜 write같음. test 해보진 말 것
        if(can_chassis.frame.can_id == 0x240) NEW_MSG_ENGINE(can_chassis);
        if(can_chassis.frame.can_id == 0x371) NEW_MSG_2_RPM_WITHOUT_ELECTRO(can_chassis);
        if(can_chassis.frame.can_id == 0x371) Brake_Pressure(can_chassis);
        if(can_chassis.frame.can_id == 0x386) WHL_Speed(can_chassis);
        if(can_chassis.frame.can_id == 0x387) WHL_PUL(can_chassis);
        
        
        //////////////////////////////////////////////
        //////////////////////////////////////////////
        //////////////////////////////////////////////
#endif
        

        if(can_error_flag_ == true)
            break;
        
        //        usleep(20000);
    }
    
    //return 0; // 원래는 해줘야함. return (void *)value;
}


void Mo_Conf(CAN_AVL &can_)
{
    
    //    memset(can_.message.DATA, 0, sizeof(BYTE)*8);
    //    memset(&can_.message, 0, sizeof(TPCANMsg));
    //    can_.PEAKCAN_TO_SOCKETCAN();
    can_.can_memset();
    
    //    can_print(can_);
    
    unsigned long tmp_id = 0x156;
    unsigned char tmp_dlc = 8;
    can_.write_param(tmp_id, tmp_dlc);
    
    can_.frame.data[0] = APM_state;                  // APM_IGNORE, (APM_En / APM_D_En)
    can_.frame.data[1] = APM_Slevel_val * 0.5;      // APM_Slevel(Steering angular speed level) 0.5*(real_value), [0, 510]
    can_.frame.data[2] = ASM_state;                         // ASM_StopRequest(Req AEB action), ASM_En(En : 0x10, D_En : 0x00)
    can_.frame.data[3] = 0;
    can_.frame.data[4] = 0;
    can_.frame.data[5] = 0;
    can_.frame.data[6] = 0;
    can_.frame.data[7] = CAN_alive_count++;             // alive-count    can.write();
    
    //    can_print(can_);
    
    can_.can_write();
}


void Mo_Val(CAN_AVL &can_)
{
    can_.can_memset();      // data만 memset
    
    unsigned long tmp_id = 0x157;
    unsigned char tmp_dlc = 8;
    
    can_.write_param(tmp_id, tmp_dlc);
    
    can_.frame.data[0] = (u_int8_t)(((10*Steer_Cmd) & 0x00ff));
    // Steer_Cmd    Andgle of Steering      10 * (real_value)   [-500, 500]
    can_.frame.data[1] = (u_int8_t)((((10*Steer_Cmd) & 0xff00) >> 8));
    // Steer_Cmd
    can_.frame.data[2] = cluster_speed_display_value;
    //can_.message.DATA[2] = (u_int8_t)10 & 0x00ff;
    can_.frame.data[3] = (u_int8_t)((u_int16_t)(100 * (aReqMax_Cmd + 10.23)) & 0x00ff);
    // aReqMax_Cmd  Acceleration Control    100*((value)+10.23) [-5.0, 5.0]     멈추려면 -값 주면 된다고 함.
    can_.frame.data[4] = (u_int8_t)(((u_int16_t)(100 * (aReqMax_Cmd + 10.23)) >> 8) & 0x00ff);
    // aReqMax_Cmd
    can_.frame.data[5] = 0;
    can_.frame.data[6] = 0;
    can_.frame.data[7] = 0;
    
    can_.can_write();
}


bool Report_APM(CAN_AVL &can_)      // id = 0x710
{
    bool can_error_flag_ = 0;
    
    //    can_print(can_);
    
    bool APM_Fd_En_         = 0;        // APM Manual/Auto 모드 변환 요청 피드백
    bool APM_Fd_Override_    = 0;        // APM override check
    u_int16_t APM_Fd_SteerAngle = 0;
    
    //    u_int16_t abc = can_.frame.data[3];
    //    u_int16_t abc_2 = abc << 8;
    
    APM_Fd_En_ = can_.frame.data[0] & 0x1;
    APM_Fd_Override_ = can_.frame.data[1] & 0x1;
    APM_Fd_SteerAngle = 0.1 * ((can_.frame.data[3] << 8)|can_.frame.data[2]);
    
    if(print_can_control_state_flag == 1)
    {
        if(APM_Fd_En_ == true)      printf("APM Enable flag\n");
        else                        printf("APM Disable flag\n");
    }

    if(APM_Fd_Override_ == true) printf("APM Override flag\n");
    
    if(print_steer_angle_flag == 1) printf(" APM_Fd_SteerAngle : %d\n",APM_Fd_SteerAngle);
    
    return can_error_flag_;
}


bool Report_ASM(CAN_AVL &can_)      // id = 0x711
{
    bool can_error_flag_ = 0;
    
    //    can_print(can_);
    
    bool ASM_Fd_En_         = 0;        // ASM Manual/Auto 모드 변환 요청 피드백
    u_int8_t ASM_Fd_VSpeed_  = 0;        // ASM vehicle speed output
    u_int8_t Mo_Fd_AlvCnt_   = 0;        // Alive-count feedback value
    
    ASM_Fd_En_          = can_.frame.data[0] & 0x1;
    ASM_Fd_VSpeed_       = can_.frame.data[1] & 0xff;
    Mo_Fd_AlvCnt_        = can_.frame.data[7] & 0xff;
    
    
    //printf("ASM_Fd_En : %d\n", ASM_Fd_En_);
    if(print_can_control_state_flag == 1)
    {
        if(ASM_Fd_En_ == true)      printf("ASM Enable flag\n");
        else                        printf("ASM Disable flag\n");
    }
    
    if(print_vehicle_speed_flag == 1)    printf("Vehicle Speed \t\t: %d\n", ASM_Fd_VSpeed_);
    if(print_alive_count_flag == 1)    printf("Alive-count value \t\t: %d\n", Mo_Fd_AlvCnt_);
    
    if(speed_pid_control_enable_flag == 1) real_vehicle_speed = ASM_Fd_VSpeed_;
    if(print_cluster_speed_display_flag == 1) cluster_speed_display_value = ASM_Fd_VSpeed_;
    //printf("APM_Fd_En_ \t= %d \t\t APM_Fd_Override \t=%d\n", APM_Fd_En_, APM_Fd_Override);
    
    return can_error_flag_;
}



bool Report_Misc(CAN_AVL &can_)
{
    bool can_error_flag_ = 0;
    // Repotr Misc
    
    
    /*
     코드 작성하기
     */
    
    return can_error_flag_;
}

void TCU_DVT13_Driving_Tq(CAN_AVL can_)
{
    u_int16_t driving_tq = 0;
    
    driving_tq = can_.frame.data[0] | ((u_int16_t)(can_.frame.data[1] << 6) << 2);
    
    if(print_driving_tq_flag == 1) printf("Driving_Tq : %d\n", driving_tq);
}

void TCU_DCT13_Engine_RPM(CAN_AVL can_)
{
    //    can_print(can_);
    
    // (0.9766, 0)
    u_int16_t cluster_engine_RPM = 0;
    bool cluster_engine_RPM_flag = 0;
    
    cluster_engine_RPM = (((u_int16_t)(can_.frame.data[2] << 1)) << 5) |(can_.frame.data[1] >> 2);
    cluster_engine_RPM_flag = (can_.frame.data[2] >> 7);
    
    cluster_engine_RPM = cluster_engine_RPM * 1;    // real value mul
    
    if(print_cluster_engine_rpm_flag == 1) printf("Cluster Engine RPM : %d\n", cluster_engine_RPM);
}

void NEW_MSG_ENGINE(CAN_AVL can_)
{
    u_int16_t rpm_1 = 0;
    u_int16_t rpm_2 = 0;
    
    rpm_1 = can_.frame.data[0] | ((u_int16_t)can_.frame.data[1] & 0x0f) << 8; // 0b0000111 == 0x0f
    rpm_2 = can_.frame.data[2] | ((u_int16_t)can_.frame.data[3]) << 8;
    
    if(print_new_msg_engine_rpm_flag == 1)
    {
        printf("NEW_MSG_ENGINE RPM_1 : %d\n", rpm_1);
        printf("NEW_MSG_ENGINE RPM_2 : %d\n", rpm_2);
    }

}

void NEW_MSG_2_RPM_WITHOUT_ELECTRO(CAN_AVL can_)
{
    // bit범위가 Brake_Pressure랑 겹침.
    //  1. dbc 잘못 읽거나
    //  2. dbc 잘못 쓰여졌거나
    u_int16_t rpm_without_electro = 0;
    
    rpm_without_electro = can_.frame.data[2] | ((u_int16_t)can_.frame.data[3] << 8);
    
    if(print_rpm_without_electro_flag == 1) printf("RPM_Without_Electro : %d\n", rpm_without_electro);
}

void Brake_Pressure(CAN_AVL can_)
{
    /*
     주석된 부분은 기존에 정상작동으로 보이던 코드.
     dbc 기준 Motorola, Intel의 코드 해석법이 다르다.
     
     */
    
//        u_int16_t brake_pressure = 0;
//
//        brake_pressure = (can_.frame.data[0] >> 7) | (((u_int16_t)can_.frame.data[1]) << 1) | (((u_int16_t)can_.frame.data[2]) << 9);
//
//        brake_pressure = brake_pressure * 1;    // real value mul
    
    u_int16_t brake_pressure = 0;
    
    // 7|16@0+ -> big-endian 방식으로 7bit부터 오른쪽 방향으로 16bit 읽음
    brake_pressure = can_.frame.data[1] | (((u_int16_t)can_.frame.data[0]) << 8);
    
    brake_pressure = brake_pressure * 1;    // real value mul

    if(print_brake_pressure_flag == 1) printf("Brake_Pressure : %d\n", brake_pressure);
}

void WHL_Speed(CAN_AVL can_)
{
    u_int16_t whl_spd_rl = 0;   //wheel speed rear left
    u_int16_t whl_spd_rr = 0;   //wheel speed rear right
    
    whl_spd_rl = (can_.frame.data[4]) | ((((u_int16_t)can_.frame.data[5]) & 0x3f) << 8);    // 0b00111111 == 0x3f
    whl_spd_rr = (can_.frame.data[6]) | ((((u_int16_t)can_.frame.data[7]) & 0x3f) << 8);
    
    if(print_whl_spd_flag == 1)
    {
        printf("whl_spd_rl : %d\n", whl_spd_rl);
        printf("whl_spd_rr : %d\n", whl_spd_rr);
    }

}

void WHL_PUL(CAN_AVL can_)
{
    u_int8_t whl_pul_rl = 0;
    u_int8_t whl_pul_rr = 0;
    
    whl_pul_rl = can_.frame.data[2];
    whl_pul_rr = can_.frame.data[3];
    
    if(print_whl_pul_flag == 1)
    {
        printf("whl_pul_rl : %d\n", whl_pul_rl);
        printf("whl_pul_rr : %d\n", whl_pul_rr);
    }

}

void can_print(CAN_AVL can_)
{
    printf("  - R | HEX | ID:%04x LEN:%1x DATA:%02x %02x %02x %02x %02x %02x %02x %02x\n",
           (int) can_.frame.can_id, (int) can_.frame.can_dlc, (int) can_.frame.data[0],
           (int) can_.frame.data[1], (int) can_.frame.data[2],
           (int) can_.frame.data[3], (int) can_.frame.data[4],
           (int) can_.frame.data[5], (int) can_.frame.data[6],
           (int) can_.frame.data[7]);
}

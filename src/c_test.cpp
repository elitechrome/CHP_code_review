//
//  c_test.cpp
//  pcan_mac
//
//  Created by Macbook Pro on 13/08/2018.
//

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "sys/time.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void sys_time_header_example();
float Vehicle_Speed();

void* do_loop(void *data)
{
    int i;
    
    int me = *((int *)data);
    for (i = 0; i < 10; i++)
    {
        printf("%d - Got %d\n", me, i);
        sleep(1);
    }
}

int main()
{
    int       thr_id;
    pthread_t p_thread[3];
    
    int status;
    int a = 1;
    int b = 2;
    int c = 3;
    
    thr_id = pthread_create(&p_thread[0], NULL, do_loop, (void *)&a);
    thr_id = pthread_create(&p_thread[1], NULL, do_loop, (void *)&b);
    thr_id = pthread_create(&p_thread[2], NULL, do_loop, (void *)&c);
    
    while(1)
    {
        printf("test\n");
        sleep(1);
    }
    
    pthread_join(p_thread[0], (void **) &status);
    pthread_join(p_thread[1], (void **) &status);
    pthread_join(p_thread[2], (void **) &status);
    
    printf("programing is end\n");
    return 0;
}


//int main()
//{
//    struct timeval t1, t2;
//    double elapsedTime;
//
//    // start timer
//    gettimeofday(&t1, NULL);
//
//    // do something
//    // ...
//
//    for(int i = 0; i < 100; i++)
//        for(int j = 0; j < 100; j++)
//            for(int c = 0; c < 100; c++)
//            {
//                int a = 3;
//            }
//
//    // stop timer
//    gettimeofday(&t2, NULL);
//
//    // compute and print the elapsed time in millisec
//    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
//    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
//    std::cout << elapsedTime << " ms.\n";
//
//    /*
//
//     input_speed = whl_spd_rl $$$ whl_sped_rr;
//     or
//     input_speed = can_output_vehicle_speed;
//
//     */
//
//    u_int16_t target_speed = 10;
//    u_int16_t input_speed = 0;
//    u_int16_t output_speed = 0;
//
//    // 현재 input speed 갱신 (renew VSpeed from CAN_Read)
//    input_speed = Vehicle_Speed();
//
//    float error = target_speed - input_speed;   // or use MSE
//
//    float time = 0.;
//
//    float K_p = 0.5;
//    float K_d = 0;
//
//    // P control    // K_p * error
//
//    // D control    // gradient
//
//
//
//
//
//
//
//
//
////    u_int8_t a = 0b11111111;
////    u_int8_t b = 0b11111111;
////
////    u_int16_t whl_spd_rr = a | ((((u_int16_t)b) & 0x7f) << 8);
////
////    printf("%04x\n", whl_spd_rr);
////
////
////    u_int16_t test = (a) | ((((u_int16_t)b) & 0x3f) << 8);
////    printf("%04x\n", test);
//
//    return 0;
//}

float Vehicle_Speed()
{
    float tmp = 0;
    
    // do something
    // ...
    
    return tmp;
}

void sys_time_header_example()
{
    struct timeval t1, t2;
    double elapsedTime;
    
    // start timer
    gettimeofday(&t1, NULL);
    
    // do something
    // ...

    // stop timer
    gettimeofday(&t2, NULL);
    
    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    std::cout << elapsedTime << " ms.\n";
}

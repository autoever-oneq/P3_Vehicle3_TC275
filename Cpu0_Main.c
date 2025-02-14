/************************************************************
 * Copyright (c) 2023, Infineon Technologies AG
 * Encoder reading using GPT12 Incremental Interface Mode
 ************************************************************/

#include "IfxGpt12_IncrEnc.h"
#include "IfxCpu.h"

#include "IfxScuWdt.h"
#include "IfxPort.h"
#include "IfxStm.h"
#include "Ifx_Types.h"

#include "Ifx_DateTime.h"
#include "SysSe/Bsp/Bsp.h"

#include "IfxGtm_reg.h"
#include "GTM_ATOM_PWM.h"

#include "STM_Interrupt.h"

#include "Driver_Adc.h"
#include "Driver_Stm.h"
#include "Can.h"

#define WAIT_TIME   10              /* Number of milliseconds to wait between each duty cycle change                */

IfxCpu_syncEvent g_cpuSyncEvent = 0;

volatile float32 angle = 30;
volatile float32 input_speed = 30;

typedef enum {
    CMD_NONE,       // 명령 없음 (PI 제어 유지)
    CMD_FORWARD,    // 직진
    CMD_BACKWARD,    // 정지
    CMD_STOP,       // 정지
    CMD_TURN_RIGHT,  // 우회전
    CMD_TURN_LEFT  // 좌회전
} CommandType;

volatile CommandType commandState = CMD_STOP;  // 현재 실행 중인 명령

void AppScheduling(void);
void initPins(void);

int core0_main(void)
{
    // Initialize system
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /* Initialize a time variable */
//    Ifx_TickTime ticksFor10ms = IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME);

    // Install interrupt handlers
//    IfxCpu_Irq_installInterruptHandler(&ISR_IncrIncZero, ISR_PRIORITY_INCRENC_ZERO);

//     Initialize peripherals
    initIncrEnc();
    initIncrEncB(); // B모터 driver 만들기 추가
    initGtmATomPwm();
    initPins();
    Driver_Stm_Init();

    /* CAN_init */
    init_message();
    init_can();

    // Main loop
    while(1)
    {
        AppScheduling();
    }

    return 0;
}

// 핀 초기화
void initPins(void)
{
    // 방향 핀 초기화
    IfxPort_setPinMode(DIRA_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(DIRA_PIN, IfxPort_State_low);

    // 브레이크 핀 초기화
    IfxPort_setPinMode(BRAKEA_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(BRAKEA_PIN, IfxPort_State_low);

    // 방향 핀 초기화 B
    IfxPort_setPinMode(DIRB_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(DIRB_PIN, IfxPort_State_low);

    // 브레이크 핀 초기화 B
    IfxPort_setPinMode(BRAKEB_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(BRAKEB_PIN, IfxPort_State_low);
}

void AppTask1ms(void)
{
}

void AppTask10ms(void)
{
    isrSTM();
}

void AppTask40ms(void)
{
    /* Can transmit */
    update_motor_rpm(&g_MessageInfo);
    transmit_message(&g_MessageInfo, VEHICLE_STATUS_ID);
}

void AppTask100ms(void)
{
    switch (commandState)
    {
        case CMD_FORWARD:
//            goStraight(input_speed);  // 속도 30으로 직진
              goStraight(g_MessageInfo.vehicle_control.MSG.motor_rpm);
            break;
        case CMD_BACKWARD:
            goBackward(input_speed);  // 속도 30으로 후진
            break;
        case CMD_STOP:
            stopMotors();
            commandState = CMD_NONE;
            break;
        case CMD_TURN_RIGHT:
//            setSteeringControl(angle*(-1)); // -30° 조향
            setSteeringControl(g_MessageInfo.vehicle_control.MSG.steering_angle_delta);
            commandState = CMD_NONE;
            break;
        case CMD_TURN_LEFT:
//            setSteeringControl(angle); // 30° 조향
            setSteeringControl(g_MessageInfo.vehicle_control.MSG.steering_angle_delta);
            commandState = CMD_NONE;
            break;
        default: // CMD_NONE 아무 명령도 없으면 기존 상태 유지
            break;
    }
}

void AppTask1000ms(void)
{
}

void AppScheduling(void)
{
    if(stSchedulingInfo.u8nuScheduling1msFlag == 1u)
    {
        stSchedulingInfo.u8nuScheduling1msFlag = 0u;
        AppTask1ms();

        if(stSchedulingInfo.u8nuScheduling10msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling10msFlag = 0u;
            AppTask10ms();
        }

        if(stSchedulingInfo.u8nuScheduling40msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling40msFlag = 0u;
            AppTask40ms();
        }

        if(stSchedulingInfo.u8nuScheduling100msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling100msFlag = 0u;
            AppTask100ms();
        }
        if(stSchedulingInfo.u8nuScheduling1000msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling1000msFlag = 0u;
            AppTask1000ms();
        }
    }
}

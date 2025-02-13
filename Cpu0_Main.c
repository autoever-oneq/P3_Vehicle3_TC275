/************************************************************
 * Copyright (c) 2023, Infineon Technologies AG
 * Encoder reading using GPT12 Incremental Interface Mode
 ************************************************************/

#include "IfxGpt12_IncrEnc.h"
#include "IfxCpu.h"
//#include "Cpu/Irq/IfxCpu_Irq.h"
//#include "IfxCpu_Irq.h"

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

//#include "Ifx_IntPrioDef.h"

// Interrupt priority definitions
#define ISR_PRIORITY_INCRENC_ZERO 6

IfxCpu_syncEvent g_cpuSyncEvent = 0;

IfxGpt12_IncrEnc_Config gpt12Config;
IfxGpt12_IncrEnc_Config gpt12ConfigB; // motor config B
IfxGpt12_IncrEnc gpt12;
IfxGpt12_IncrEnc gpt12B; // 이거 추가해봄

// 이거 ADC 변수들
static int ADC0_temp = 0;
static int ADC1_temp = 0;
static int ADC2_temp = 0;
static int threshold = 4000;

// 엔코더 설정
#define PULSES_PER_REV 330     // 한 채널당 펄스 수 330으로 변경
const uint32 CPR = (PULSES_PER_REV * 4);  // 4체배시 한바퀴 펄스 수

// 핀 정의
#define PWMA_PIN &MODULE_P02,1   // PWM 핀 (P2.1)
#define BRAKEA_PIN &MODULE_P02,7 // 브레이크 핀 (P2.7)
#define DIRA_PIN &MODULE_P10,1   // 방향 제어 핀 (P10.1)

// 핀 정의 B 추가
#define PWMB_PIN &MODULE_P10,3   // PWMB 핀 (P10.3)
#define BRAKEB_PIN &MODULE_P02,6 // 브레이크B 핀 (P2.6)
#define DIRB_PIN &MODULE_P10,2   // 방향 제어B 핀 (P10.2)

volatile uint8 motor_speed = 0;    // 0~100
volatile boolean motor_dir = 0;    // 0:정방향, 1:역방향
volatile boolean motor_enable = 0;  // 0:제동, 1:해제

volatile float32 RPM_CMD1=0;
volatile float32 RPM_CMD2=0; // 추가 B

// Interrupt handler for zero crossing
//IFX_INTERRUPT(ISR_IncrIncZero, 0, ISR_PRIORITY_INCRENC_ZERO)
//{
//    IfxGpt12_IncrEnc_onZeroIrq(&gpt12Config);
//}

volatile float32 angle = 10;
volatile float32 input_speed = 30;

typedef enum {
    CMD_NONE,       // 명령 없음 (PI 제어 유지)
    CMD_FORWARD,    // 직진
    CMD_STOP,       // 정지
    CMD_TURN_LEFT,  // 좌회전
    CMD_TURN_RIGHT  // 우회전
} CommandType;

volatile CommandType commandState = CMD_STOP;  // 현재 실행 중인 명령

void AppScheduling(void);

void initIncrEnc(void)
{
    // Initialize global clocks
    IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_8);
    IfxGpt12_setGpt2BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt2BlockPrescaler_4);

    // Create module config
//    IfxGpt12_IncrEnc_Config gpt12Config;
    IfxGpt12_IncrEnc_initConfig(&gpt12Config, &MODULE_GPT120);

    // Configure encoder parameters
    gpt12Config.base.offset               = 100;                    // Initial position offset
    gpt12Config.base.reversed             = FALSE;               // Count direction not reversed
    gpt12Config.base.resolution           = PULSES_PER_REV;                // Encoder resolution
    gpt12Config.base.periodPerRotation    = 1;                   // Number of periods per rotation
    gpt12Config.base.resolutionFactor     = IfxStdIf_Pos_ResolutionFactor_fourFold;  // Quadrature mode
    gpt12Config.base.updatePeriod         = 0.001;              // 1ms update period
    gpt12Config.base.speedModeThreshold   = 100;                // Threshold for speed calculation mode
    gpt12Config.base.minSpeed             = 10;                 // Minimum speed in rpm
    gpt12Config.base.maxSpeed             = 5000;                // Maximum speed in rpm

    // Configure pins
    gpt12Config.pinA = &IfxGpt120_T2INA_P00_7_IN;     // Encoder A signal -> T3IN4  39
    gpt12Config.pinB = &IfxGpt120_T2EUDA_P00_8_IN;    // Encoder B signal -> T3EUD  26
    gpt12Config.pinZ = NULL;                          // No Z signal used
    gpt12Config.pinMode = IfxPort_InputMode_pullDown;   // Use internal pullup

    // Configure interrupts
    gpt12Config.zeroIsrPriority = ISR_PRIORITY_INCRENC_ZERO;
    gpt12Config.zeroIsrProvider = IfxSrc_Tos_cpu0;

    // Enable speed filter
    gpt12Config.base.speedFilterEnabled = TRUE;
    gpt12Config.base.speedFilerCutOffFrequency = gpt12Config.base.maxSpeed / 2 * IFX_PI * 2;

    // Initialize module
    IfxGpt12_IncrEnc_init(&gpt12, &gpt12Config);

}

void initIncrEncB(void) // motor timer2 connect
{
    // Initialize global clocks
    IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_8); // gpt1(timer 2, 3, 4)
    IfxGpt12_setGpt2BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt2BlockPrescaler_4); // gpt2(timer 5, 6)

    // Create module config
//    IfxGpt12_IncrEnc_Config gpt12Config;
    IfxGpt12_IncrEnc_initConfig(&gpt12ConfigB, &MODULE_GPT120);

    // Configure encoder parameters
    gpt12ConfigB.base.offset               = 100;                    // Initial position offset
    gpt12ConfigB.base.reversed             = FALSE;               // Count direction not reversed
    gpt12ConfigB.base.resolution           = PULSES_PER_REV;                // Encoder resolution
    gpt12ConfigB.base.periodPerRotation    = 1;                   // Number of periods per rotation
    gpt12ConfigB.base.resolutionFactor     = IfxStdIf_Pos_ResolutionFactor_fourFold;  // Quadrature mode
    gpt12ConfigB.base.updatePeriod         = 0.001;              // 1ms update period
    gpt12ConfigB.base.speedModeThreshold   = 100;                // Threshold for speed calculation mode
    gpt12ConfigB.base.minSpeed             = 10;                 // Minimum speed in rpm
    gpt12ConfigB.base.maxSpeed             = 5000;                // Maximum speed in rpm

    // Configure pins
    gpt12ConfigB.pinA = &IfxGpt120_T4INA_P02_8_IN;     // Encoder A 51
    gpt12ConfigB.pinB = &IfxGpt120_T4EUDA_P00_9_IN;    // Encoder B 28
    gpt12ConfigB.pinZ = NULL;                          // No Z signal used
    gpt12ConfigB.pinMode = IfxPort_InputMode_pullDown;   // Use internal pullup

    // Configure interrupts
    gpt12ConfigB.zeroIsrPriority = ISR_PRIORITY_INCRENC_ZERO;
    gpt12ConfigB.zeroIsrProvider = IfxSrc_Tos_cpu0;

    // Enable speed filter
    gpt12ConfigB.base.speedFilterEnabled = TRUE;
    gpt12ConfigB.base.speedFilerCutOffFrequency = gpt12ConfigB.base.maxSpeed / 2 * IFX_PI * 2;

    // Initialize module
    IfxGpt12_IncrEnc_init(&gpt12B, &gpt12ConfigB);
}

// 모터 제어 함수
void setMotorControl(uint8 direction, uint8 enable)
{
    // 브레이크 설정
    if (enable == 0)
    {
        IfxPort_setPinState(BRAKEA_PIN, IfxPort_State_high); // 브레이크 활성화
        // PWM 출력 중지
        //GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL1 = 0;
        return;
    }
    else
    {
        IfxPort_setPinState(BRAKEA_PIN, IfxPort_State_low); // 브레이크 비활성화
        //GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL1 = 2;
    }

    // 방향 설정
    if (direction == 0)
    {
        IfxPort_setPinState(DIRA_PIN, IfxPort_State_low); // 정방향
    }
    else
    {
        IfxPort_setPinState(DIRA_PIN, IfxPort_State_high); // 역방향
    }


}

void setMotorControlB(uint8 direction, uint8 enable)
{
    // 브레이크 설정
    if (enable == 0)
    {
        IfxPort_setPinState(BRAKEB_PIN, IfxPort_State_high); // 브레이크 활성화
        // PWM 출력 중지
        //GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL1 = 0;
        return;
    }
    else
    {
        IfxPort_setPinState(BRAKEB_PIN, IfxPort_State_low); // 브레이크 비활성화
        //GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL1 = 2;
    }

    // 방향 설정
    if (direction == 0)
    {
        IfxPort_setPinState(DIRB_PIN, IfxPort_State_low); // 정방향
    }
    else
    {
        IfxPort_setPinState(DIRB_PIN, IfxPort_State_high); // 역방향
    }
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



float32 speed;
sint32 rawPosition;
IfxStdIf_Pos_Dir direction;

float32 speedB;
sint32 rawPositionB;
IfxStdIf_Pos_Dir directionB;

void Encoder_update(void) // 이 코드 수정 필요
{
    IfxGpt12_IncrEnc_update(&gpt12);
    speed = IfxGpt12_IncrEnc_getSpeed(&gpt12); // 여기서 0 받음?
    rawPosition = IfxGpt12_IncrEnc_getRawPosition(&gpt12);
    direction = IfxGpt12_IncrEnc_getDirection(&gpt12);
}

void Encoder_updateB(void) // 이 코드가 기존 Encoder 코드와 겹침
{
    IfxGpt12_IncrEnc_update(&gpt12B);
    speedB = IfxGpt12_IncrEnc_getSpeed(&gpt12B);
    rawPositionB = IfxGpt12_IncrEnc_getRawPosition(&gpt12B);
    directionB = IfxGpt12_IncrEnc_getDirection(&gpt12B);
}

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
    Ifx_TickTime ticksFor10ms = IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME);

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

    //initPeripherals(); /* Initialize the STM module */

//    Driver_Stm_Init();
    //Driver_Adc_Init();

    // Main loop
    while(1)
    {
        AppScheduling();
    }

    return 0;
}


void AppTask1ms(void)
{
}

void AppTask10ms(void)
{
    isrSTM();

    update_motor_rpm(&g_MessageInfo);
    transmit_message(&g_MessageInfo, VEHICLE_STATUS_ID);
}

void AppTask100ms(void)
{
    switch (commandState)
    {
        case CMD_FORWARD:
            goStraight(input_speed);  // 속도 30으로 직진
            break;
        case CMD_STOP:
            stopMotors();
            break;
        case CMD_TURN_LEFT:
            setSteeringControl(angle*(-1)); // -30° 조향
            commandState = CMD_NONE;
            break;
        case CMD_TURN_RIGHT:
            setSteeringControl(angle); // 30° 조향
            commandState = CMD_NONE;
            break;
        default: // CMD_NONE 아무 명령도 없으면 기존 상태 유지
//            stopMotors();
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

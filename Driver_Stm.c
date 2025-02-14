//Driver_Stm.c
/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Stm.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxStm_regdef.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    Ifx_STM             *stmSfr;            /**< \brief Pointer to Stm register base */
    IfxStm_CompareConfig stmConfig;         /**< \brief Stm Configuration structure */
    volatile uint32      counter;           /**< \brief interrupt counter */
} App_Stm;


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
App_Stm g_Stm; /**< \brief Stm global data */
//uint32 u32nuCounter1ms = 0u;
SchedulingFlag stSchedulingInfo;

typedef struct
{
    uint32 u32nuCnt1ms;
    uint32 u32nuCnt10ms;
    uint32 u32nuCnt100ms;
}TestCnt;

TestCnt stTestcnt = {0};

/***********************************************************************/
/*Function*/ 
/***********************************************************************/
IFX_INTERRUPT(STM_Int0Handler, 0, 100);
Ifx_STM g_Ifx_STM;
//uint32 timeStamp = g_Stm.stmSfr->TIM0.U;
void Driver_Stm_Init(void)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxStm_enableOcdsSuspend(&MODULE_STM0);

    g_Stm.stmSfr = &MODULE_STM0;
    IfxStm_initCompareConfig(&g_Stm.stmConfig);

    g_Stm.stmConfig.triggerPriority = 100u;
    g_Stm.stmConfig.typeOfService   = IfxSrc_Tos_cpu0;
    g_Stm.stmConfig.ticks           = 100000u;

    IfxStm_initCompare(g_Stm.stmSfr, &g_Stm.stmConfig);
    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

}


void STM_Int0Handler(void)
{
    IfxCpu_enableInterrupts();    
    
    IfxStm_clearCompareFlag(g_Stm.stmSfr, g_Stm.stmConfig.comparator);
    IfxStm_increaseCompare(g_Stm.stmSfr, g_Stm.stmConfig.comparator, 100000u);

    stTestcnt.u32nuCnt1ms++;

    if((stTestcnt.u32nuCnt1ms % 1) == 0u)
    {
        stSchedulingInfo.u8nuScheduling1msFlag = 1u;
    }  

    if((stTestcnt.u32nuCnt1ms % 10) == 0u)
    {
        stSchedulingInfo.u8nuScheduling10msFlag = 1u;
    }

    if(stTestcnt.u32nuCnt1ms % 40 == 0){
        stSchedulingInfo.u8nuScheduling40msFlag = 1u;
    }

    if((stTestcnt.u32nuCnt1ms % 100) == 0u)
    {
        stSchedulingInfo.u8nuScheduling100msFlag = 1u;
    }

    if((stTestcnt.u32nuCnt1ms % 1000) == 0u)
    {
        stSchedulingInfo.u8nuScheduling1000msFlag = 1u;
    }
}

void MyDelay_micro(uint32 deadLine){
    uint32 start = MODULE_STM0.TIM0.U;
    uint32 temp = 0;
    while((((MODULE_STM0.TIM0.U) - start)/ 100) <= deadLine) temp++;
}

void MyDelay_milli(uint32 deadLine){
    uint32 start = MODULE_STM0.TIM0.U;
    uint32 temp = 0;
    while((((MODULE_STM0.TIM0.U) - start)/ 100000) <= deadLine) temp++;
}


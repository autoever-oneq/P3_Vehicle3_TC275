/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "App_Scheduling.h"
#include "Driver_Stm.h"


/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    uint32 u32nuCnt1ms;
    uint32 u32nuCnt10ms;
    uint32 u32nuCnt100ms;
}TestCnt;


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void AppTask1ms(void);
static void AppTask10ms(void);
static void AppTask100ms(void);

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
TestCnt stTestCnt;

/***********************************************************************/
/*Function*/ 
/***********************************************************************/
static void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;
}

static void AppTask10ms(void)
{

    stTestCnt.u32nuCnt10ms++;
    can_TxTest();
}

static void AppTask100ms(void)
{
    stTestCnt.u32nuCnt100ms++;
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
    }
}

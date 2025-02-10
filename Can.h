#ifndef DRIVER_CAN
#define DRIVER_CAN

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxMultican_Can.h"
#include "Can_Message.h"
#include "Motor.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/
#define CAN0_SRCID IfxMultican_SrcId_0

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    IfxMultican_Can        can;          /**< \brief CAN driver handle */
    IfxMultican_Can_Node   canNode;   /**< \brief CAN Source Node */
    IfxMultican_Can_MsgObj canMsgTxObj;
    IfxMultican_Can_MsgObj canMsgRxObj;
} Can_Info;

typedef struct{
    VehicleControl vehicle_control;
    volatile uint8 vehicle_control_flag;
}Vehicle_Control_Info;

//typedef struct{
//    uint32 dataLow;
//    uint32 dataHigh;
//}Send_Info;


/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
extern Can_Info g_CanInfo;
extern Vehicle_Control_Info g_VehicleControlInfo;
//extern Send_Info g_sendInfo;
//extern volatile uint8 canInterruptFlag;
/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void init_db(void);
void init_can(void);

void transmit_message(void* msgptr, uint32 messageID);
void can_TxTest(void);

#endif /* DRIVER_STM */

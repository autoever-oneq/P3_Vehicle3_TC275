#ifndef DRIVER_CAN
#define DRIVER_CAN

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxMultican_Can.h"
//#include "Can_Message.h"

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


/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
extern Can_Info g_CanInfo;
extern volatile uint8 canInterruptFlag;
/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void init_db(void);
void init_can(void);
void can_TxTest(void);

#endif /* DRIVER_STM */

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Can.h"


/***********************************************************************/
/*Define*/ 
/***********************************************************************/


/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
Can_Info g_CanInfo; /**< \brief Demo information */
volatile uint8 canInterruptFlag = 0;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

IFX_INTERRUPT(CAN_RxInt0Handler, 0 , 101);
void CAN_RxInt0Handler(void){

    IfxMultican_Status readStatus;
    static uint32 u32data1 = 0u;
    static uint32 u32data2 = 0u;

    IfxCpu_enableInterrupts();
    IfxMultican_Message readmsg;
    /*
     * readmsg.data[1]: 상위 bit
     * readmsg.data[0]: 하위 bit
     */

    readStatus = IfxMultican_Can_MsgObj_readMessage(&g_CanInfo.canMsgRxObj, &readmsg);

    if (readStatus == IfxMultican_Status_newData){
        canInterruptFlag = 1;
        u32data1 = readmsg.data[0];
        u32data2 = readmsg.data[1];
    }

}

void init_can(void)
{
    /* create module config */
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    // CAN0_RX INT
    canConfig.nodePointer[CAN0_SRCID].priority = 101;
    canConfig.nodePointer[CAN0_SRCID].typeOfService = IfxSrc_Tos_cpu0;

    /* initialize module */
    IfxMultican_Can_initModule(&g_CanInfo.can, &canConfig);

    /* create CAN node config */
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_CanInfo.can);

    canNodeConfig.baudrate = 500000UL;     /*500kbps*/
    canNodeConfig.nodeId    = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);
    canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;
    canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;
    canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

    IfxMultican_Can_Node_init(&g_CanInfo.canNode, &canNodeConfig);

    // TX
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_CanInfo.canNode);

    canMsgObjConfig.msgObjId              = 0;
    canMsgObjConfig.messageId             = 0x100;
    canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;
    canMsgObjConfig.frame                 = IfxMultican_Frame_transmit;
    canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;
    canMsgObjConfig.control.extendedFrame = FALSE;
    canMsgObjConfig.control.matchingId    = TRUE;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgTxObj, &canMsgObjConfig);

    // RX
    canMsgObjConfig.msgObjId              = 10U;
    canMsgObjConfig.messageId             = 0x200;
    canMsgObjConfig.frame                 = IfxMultican_Frame_receive;
    canMsgObjConfig.control.extendedFrame = FALSE;

    // CAN0 INT 활성화
    canMsgObjConfig.rxInterrupt.enabled = TRUE;
    canMsgObjConfig.rxInterrupt.srcId = CAN0_SRCID;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgRxObj, &canMsgObjConfig);

}


void can_TxTest(void)
{
    const uint32 dataLow  = 0x12340000;
    const uint32 dataHigh = 0x9abc0000;

    /* Transmit Data */
    {
        IfxMultican_Message msg;
        IfxMultican_Message_init(&msg, 0x100, dataLow, dataHigh, IfxMultican_DataLengthCode_8);

        while (IfxMultican_Can_MsgObj_sendMessage(&g_CanInfo.canMsgTxObj, &msg) == IfxMultican_Status_notSentBusy)
        {}
    }
}

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
Message_Info g_MessageInfo;
/***********************************************************************/
/*Function*/
/***********************************************************************/

IFX_INTERRUPT(CAN_RxInt0Handler, 0, 101);
void CAN_RxInt0Handler (void)
{

    IfxMultican_Status readStatus;

    IfxCpu_enableInterrupts();
    IfxMultican_Message readmsg;
    /*
     * readmsg.data[1]: 상위 4byte
     * readmsg.data[0]: 하위 4byte
     */

    readStatus = IfxMultican_Can_MsgObj_readMessage(&g_CanInfo.canMsgRxObj, &readmsg);

    if (readStatus == IfxMultican_Status_newData)
    {

        switch(readmsg.id){
            case VEHICLE_CONTROL_ID:
            {
                g_MessageInfo.vehicle_control.LL = ((sint64)readmsg.data[1] << 32) | (sint64)readmsg.data[0];
                g_MessageInfo.vehicle_control_flag = 1;
                break;
            }
            default:
                break;
        }
    }

}

void init_message(void){
//    g_MessageInfo.vehicle_control.LL = 0;
    memset(&g_MessageInfo, 0, sizeof(g_MessageInfo));
}

void init_can (void)
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

    canNodeConfig.baudrate = 500000UL; /*500kbps*/
    canNodeConfig.nodeId = (IfxMultican_NodeId) ((int) IfxMultican_NodeId_0);
    canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;
    canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
    canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

    IfxMultican_Can_Node_init(&g_CanInfo.canNode, &canNodeConfig);

    // TX
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_CanInfo.canNode);

    canMsgObjConfig.msgObjId = 0;
    canMsgObjConfig.messageId = 0x100;
    canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
    canMsgObjConfig.frame = IfxMultican_Frame_transmit;
    canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
    canMsgObjConfig.control.extendedFrame = FALSE;
    canMsgObjConfig.control.matchingId = TRUE;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgTxObj, &canMsgObjConfig);

    // RX
    canMsgObjConfig.msgObjId = 10U;
    canMsgObjConfig.messageId = 0x200;
    canMsgObjConfig.frame = IfxMultican_Frame_receive;
    canMsgObjConfig.control.extendedFrame = FALSE;

    // CAN0 INT 활성화
    canMsgObjConfig.rxInterrupt.enabled = TRUE;
    canMsgObjConfig.rxInterrupt.srcId = CAN0_SRCID;
    IfxMultican_Can_MsgObj_init(&g_CanInfo.canMsgRxObj, &canMsgObjConfig);

}

// 메세지만 전달
void transmit_message (Message_Info *msgptr, uint32 messageID)
{
    IfxMultican_Message msg;

    /*
     * send_data[0]:하위 4 byte
     * send_data[1]:상위 4 byte
     * */

    uint32 send_data[2] = {0};

    switch (messageID)
    {
        case VEHICLE_STATUS_ID :
        {

            send_data[0] = (msgptr->vehicle_status.LL) & 0xFFFFFFFF ;
            send_data[1] = (msgptr->vehicle_status.LL >> 32) & 0xFFFFFFFF;

            IfxMultican_Message_init(&msg, VEHICLE_STATUS_ID, send_data[0], send_data[1], IfxMultican_DataLengthCode_8);
            break;
        }
        default :

            break;
    }

    while (IfxMultican_Can_MsgObj_sendMessage(&g_CanInfo.canMsgTxObj, &msg) == IfxMultican_Status_notSentBusy)
    {
    }
}

void can_TxTest (void)
{
//    uint32 send_data[2] = {0};
//    Motor_Rpm_Info motor_speed_info = {50.1234, 51.4321 };
//
//    unsigned int motor1_val = (unsigned int)(motor_speed_info.motor1_cur_rpm * 100); // 5012 0x1394
//    unsigned int motor2_val = (unsigned int)(motor_speed_info.motor2_cur_rpm * 100); // 5143 0x1417
//
//    /* Transmit Data */
//    {
//        IfxMultican_Message msg;
//
//        VehicleStatus vs = {0};
//        vs.MSG.motor1_cur_rpm = motor1_val; // [15:0]
//        vs.MSG.motor2_cur_rpm = motor2_val; // [31:16]
//
//        // vs.LL = (motor2_val << 16) | motor1_val
//        // 0x14171394
//
//        send_data[0] = (vs.LL) & 0xFFFFFFFF ;
//        send_data[1] = (vs.LL >> 32) & 0xFFFFFFFF;
//
//        IfxMultican_Message_init(&msg, VEHICLE_STATUS_ID, send_data[0], send_data[1], IfxMultican_DataLengthCode_8);
//
//        while (IfxMultican_Can_MsgObj_sendMessage(&g_CanInfo.canMsgTxObj, &msg) == IfxMultican_Status_notSentBusy)
//        {
//        }
//    }
}

//void can_TxTest (void)
//{
//    const uint32 dataLow = 0x01020304;
//    const uint32 dataHigh = 0x05060708;
//
//    /* Transmit Data */
//    {
//        IfxMultican_Message msg;
//        IfxMultican_Message_init(&msg, 0x100, dataLow, dataHigh, IfxMultican_DataLengthCode_8);
//
//        while (IfxMultican_Can_MsgObj_sendMessage(&g_CanInfo.canMsgTxObj, &msg) == IfxMultican_Status_notSentBusy)
//        {
//        }
//    }
//}

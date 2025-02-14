//Driver_Adc.c
/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Adc.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void Driver_VAdc4_Init(void);
static void Driver_Adc0_Init(void);
static void Driver_Adc1_Init(void);
static void Driver_Adc2_Init(void);

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/

App_VadcAutoScan g_VadcAutoScan;
IfxVadc_Adc_ChannelConfig adcChannelConfig[8];
IfxVadc_Adc_Channel   adcChannel[8];
uint32 adcDataResult[8] = {0u,};


/***********************************************************************/
/*Function*/ 
/***********************************************************************/
static void Driver_VAdc4_Init(void){
    /* VADC Configuration */
        /* create configuration for adc module*/
        IfxVadc_Adc_Config adcConfig;
        IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

        /* initialize module */
        IfxVadc_Adc_initModule(&g_VadcAutoScan.vadc, &adcConfig);

        /* create group config */
        IfxVadc_Adc_GroupConfig adcGroupConfig;
        IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &g_VadcAutoScan.vadc);

        /* with group 4 */
        adcGroupConfig.groupId = IfxVadc_GroupId_4;
        adcGroupConfig.master  = adcGroupConfig.groupId;

        /* enable scan source */
        adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;

        /* enable auto scan */
        adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

        /* enable all gates in "always" mode (no edge detection) */
        adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

        /* initialize the group */
        /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
        IfxVadc_Adc_initGroup(&g_VadcAutoScan.adcGroup, &adcGroupConfig);
}

static void Driver_Adc0_Init(void)
{
    //sar 4.7-> analog pin 0 config
    /*channel init*/
    uint32    chnIx = 7;
    IfxVadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &g_VadcAutoScan.adcGroup);

    adcChannelConfig[chnIx].channelId      = (IfxVadc_ChannelId)(chnIx);
    adcChannelConfig[chnIx].resultRegister = (IfxVadc_ChannelResult)(chnIx);  /* use dedicated result register */

    /* initialize the channel */
    IfxVadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);

    /* add to scan */
    unsigned channels = (1 << adcChannelConfig[chnIx].channelId);
    unsigned mask     = channels;
    IfxVadc_Adc_setScan(&g_VadcAutoScan.adcGroup, channels, mask);

}

static void Driver_Adc1_Init(void)
{
    //sar 4.6-> analog pin 1 config
    /*channel init*/
    uint32    chnIx = 6;
    IfxVadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &g_VadcAutoScan.adcGroup);

    adcChannelConfig[chnIx].channelId      = (IfxVadc_ChannelId)(chnIx);
    adcChannelConfig[chnIx].resultRegister = (IfxVadc_ChannelResult)(chnIx);  /* use dedicated result register */

    /* initialize the channel */
    IfxVadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);

    /* add to scan */
    unsigned channels = (1 << adcChannelConfig[chnIx].channelId);
    unsigned mask     = channels;
    IfxVadc_Adc_setScan(&g_VadcAutoScan.adcGroup, channels, mask);

}

static void Driver_Adc2_Init(void)
{
    //sar 4.5-> analog pin 2 config
    /*channel init*/
    uint32    chnIx = 5;
    IfxVadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &g_VadcAutoScan.adcGroup);

    adcChannelConfig[chnIx].channelId      = (IfxVadc_ChannelId)(chnIx);
    adcChannelConfig[chnIx].resultRegister = (IfxVadc_ChannelResult)(chnIx);  /* use dedicated result register */

    /* initialize the channel */
    IfxVadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);

    /* add to scan */
    unsigned channels = (1 << adcChannelConfig[chnIx].channelId);
    unsigned mask     = channels;
    IfxVadc_Adc_setScan(&g_VadcAutoScan.adcGroup, channels, mask);

}


void Driver_Adc_Init(void)
{
    /*ADC0 Converter Init*/
    Driver_VAdc4_Init();
    Driver_Adc0_Init();
    Driver_Adc1_Init();
    Driver_Adc2_Init();
    /*ADC0 Converter Start*/
    Driver_VAdc4_ConvStart();
}

void Driver_VAdc4_ConvStart(void)
{
    /* start autoscan */
    IfxVadc_Adc_startScan(&g_VadcAutoScan.adcGroup);
}

uint32 Driver_Adc0_DataObtain(void)
{
    uint32    chnIx =7;
    Ifx_VADC_RES conversionResult; /* wait for valid result */

    /* check results */
    do
    {
        conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
    } while (!conversionResult.B.VF);

    adcDataResult[chnIx] = conversionResult.B.RESULT;
    return adcDataResult[chnIx];
}

uint32 Driver_Adc1_DataObtain(void)
{
    uint32    chnIx =6;
    Ifx_VADC_RES conversionResult; /* wait for valid result */

    /* check results */
    do
    {
        conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
    } while (!conversionResult.B.VF);

    adcDataResult[chnIx] = conversionResult.B.RESULT;
    return adcDataResult[chnIx];
}

uint32 Driver_Adc2_DataObtain(void)
{
    uint32    chnIx = 5;
    Ifx_VADC_RES conversionResult; /* wait for valid result */

    /* check results */
    do
    {
        conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
    } while (!conversionResult.B.VF);

    adcDataResult[chnIx] = conversionResult.B.RESULT;
    return adcDataResult[chnIx];
}


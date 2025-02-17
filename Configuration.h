/**
 * \file Configuration.h
 * \brief Global configuration
 *
 * \version iLLD_Demos_1_0_1_4_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Config Application configuration
 * \ingroup IfxLld_Demo_AsclinShellInterface_SrcDoc
 *
 *
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Ifx_Cfg.h"
#include "ConfigurationIsr.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/* APPLICATION_KIT_TC237 혹은 SHIELD_BUDDY 중에 한가지만 선택*/
#define APPLICATION_KIT_TC237 1
#define SHIELD_BUDDY 2

/** \addtogroup IfxLld_Demo_AsclinShellInterface_SrcDoc_Config
 * \{ */

#define CFG_ASC0_BAUDRATE       (115200.0)                   /**< \brief Define the Baudrate */
#define CFG_ASC0_RX_BUFFER_SIZE (512)                        /**< \brief Define the Rx buffer size in byte. */
#define CFG_ASC0_TX_BUFFER_SIZE (6 * 1024)                   /**< \brief Define the Tx buffer size in byte. */

/*______________________________________________________________________________
** Help Macros
**____________________________________________________________________________*/
/**
 * \name Macros for Regression Runs
 * \{
 */

#ifndef REGRESSION_RUN_STOP_PASS
#define REGRESSION_RUN_STOP_PASS
#endif

#ifndef REGRESSION_RUN_STOP_FAIL
#define REGRESSION_RUN_STOP_FAIL
#endif

/** \} */

/** \} */

#endif

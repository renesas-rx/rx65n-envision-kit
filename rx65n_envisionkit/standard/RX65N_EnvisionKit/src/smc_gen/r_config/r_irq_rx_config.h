/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013-2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_irq_rx_config.h
* Version      : 1.70
* Device(s)    : Renesas RX Family
* Tool-Chain   : Renesas RX Standard Toolchain
* H/W Platform :
* Description  : User configurable options and setup definitions for the RX IRQ support module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 15.07.2013  1.00    First Release
*         : 04.04.2014  1.20    Using the definition of VECT_ICU_IRQ15 to check the number of IRQs.
*         : 30.09.2015  1.70    Using the definition of VECT_ICU_IRQ7  to check the number of IRQs.
***********************************************************************************************************************/
#ifndef IRQ_CONFIG_H
#define IRQ_CONFIG_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_irq_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Set this value to 1 to enable locking functions for the IRQ APIs, otherwise set to 0 to ignore locking.
 * If locking is enabled, the application must obtain a lock on the selected IRQ before making calls to the API
 * for that IRQ. If locking is disabled, any caller has unrestricted access to any IRQ function. Disabling is
 * provided for systems that require faster and smaller code, however care must be taken to prevent reentrancy or
 * conflict over resource usage.
 */
#define IRQ_CFG_REQUIRE_LOCK    (1)

/* Checking of arguments passed to IRQ API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining IRQ_CFG_PARAM_CHECKING.
 * To control parameter checking locally, set IRQ_CFG_PARAM_CHECKING to 1 to enable it, otherwise set to 0 skip checking.
 *
 * Use this option with caution.
 */

#define IRQ_CFG_PARAM_CHECKING  (BSP_CFG_PARAM_CHECKING_ENABLE)


/* *********************************************************************************************************************
 * IRQ enabling and port assignments
 *
 * Use these defines to select IRQs that are to be used in your system.
 * This will eliminate the generation of code for unused IRQs.
 * To use an IRQ, set its value to 1, otherwise set it to 0.
 *
 * IRQs enabled for use must have an I/O port and bit must assigned. Use these definitions to make the assignments.
 * These are decoded by the API to map to the port input data registers.
 * Set these as required according to the following format:
 * #define IRQ_PORT_IRQ*     (PORTm) where m is the port number and the IRQ number replaces *
 * #define IRQ_PORT_BIT_IRQ* (IRQ_BITn) where n is the bit number and the IRQ number replaces *
 * Port assignments here must match the port configuration settings performed externally for them.
***********************************************************************************************************************/
#define IRQ_CFG_USE_IRQ0    (0)
#if IRQ_CFG_USE_IRQ0 == 1
#define IRQ_PORT_IRQ0     (PORTm)
#define IRQ_PORT_BIT_IRQ0 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ1    (0)
#if IRQ_CFG_USE_IRQ1 == 1
#define IRQ_PORT_IRQ1     (PORTm)
#define IRQ_PORT_BIT_IRQ1 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ2    (0)
#if IRQ_CFG_USE_IRQ2 == 1
#define IRQ_PORT_IRQ2     (PORTm)
#define IRQ_PORT_BIT_IRQ2 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ3    (0)
#if IRQ_CFG_USE_IRQ3 == 1
#define IRQ_PORT_IRQ3     (PORTm)
#define IRQ_PORT_BIT_IRQ3 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ4    (0)
#if IRQ_CFG_USE_IRQ4 == 1
#define IRQ_PORT_IRQ4     (PORTm)
#define IRQ_PORT_BIT_IRQ4 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ5    (0)
#if IRQ_CFG_USE_IRQ5 == 1
#define IRQ_PORT_IRQ5     (PORTm)
#define IRQ_PORT_BIT_IRQ5 (IRQ_BITn)
#endif

#if defined(VECT_ICU_IRQ7)           /* For MCUs with 8 IRQs. */
#define IRQ_CFG_USE_IRQ6    (0)
#if IRQ_CFG_USE_IRQ6 == 1
#define IRQ_PORT_IRQ6     (PORTm)
#define IRQ_PORT_BIT_IRQ6 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ7    (0)
#if IRQ_CFG_USE_IRQ7 == 1
#define IRQ_PORT_IRQ7     (PORTm)
#define IRQ_PORT_BIT_IRQ7 (IRQ_BITn)
#endif
#endif /* #if defined(VECT_ICU_IRQ7)*/

#if defined(VECT_ICU_IRQ15)          /* For MCUs with 16 IRQs. */
#define IRQ_CFG_USE_IRQ8    (0)
#if IRQ_CFG_USE_IRQ8 == 1
#define IRQ_PORT_IRQ8     (PORTm)
#define IRQ_PORT_BIT_IRQ8 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ9    (0)
#if IRQ_CFG_USE_IRQ9 == 1
#define IRQ_PORT_IRQ9     (PORTm)
#define IRQ_PORT_BIT_IRQ9 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ10    (0)
#if IRQ_CFG_USE_IRQ10 == 1
#define IRQ_PORT_IRQ10     (PORTm)
#define IRQ_PORT_BIT_IRQ10 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ11    (0)
#if IRQ_CFG_USE_IRQ11 == 1
#define IRQ_PORT_IRQ11     (PORTm)
#define IRQ_PORT_BIT_IRQ11 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ12    (0)
#if IRQ_CFG_USE_IRQ12 == 1
#define IRQ_PORT_IRQ12     (PORTm)
#define IRQ_PORT_BIT_IRQ12 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ13    (0)
#if IRQ_CFG_USE_IRQ13 == 1
#define IRQ_PORT_IRQ13     (PORTm)
#define IRQ_PORT_BIT_IRQ13 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ14    (0)
#if IRQ_CFG_USE_IRQ14 == 1
#define IRQ_PORT_IRQ14     (PORTm)
#define IRQ_PORT_BIT_IRQ14 (IRQ_BITn)
#endif

#define IRQ_CFG_USE_IRQ15    (0)
#if IRQ_CFG_USE_IRQ15 == 1
#define IRQ_PORT_IRQ15     (PORTm)
#define IRQ_PORT_BIT_IRQ15 (IRQ_BITn)
#endif

#endif /* #if defined(VECT_ICU_IRQ15)*/


/***********************************************************************************************************************
Digital Filtering
***********************************************************************************************************************/
/* To enable digital noise filtering with the selected IRQ.
 * Set the value to 1 to enable the filter or 0 to disable it. */
#define IRQ_CFG_FILT_EN_IRQ0       (0)   /* Filtering disabled on IRQ 0. */
#define IRQ_CFG_FILT_EN_IRQ1       (0)   /* Filtering disabled on IRQ 1. */
#define IRQ_CFG_FILT_EN_IRQ2       (0)   /* Filtering disabled on IRQ 2. */
#define IRQ_CFG_FILT_EN_IRQ3       (0)   /* Filtering disabled on IRQ 3. */
#define IRQ_CFG_FILT_EN_IRQ4       (0)   /* Filtering disabled on IRQ 4. */
#define IRQ_CFG_FILT_EN_IRQ5       (0)   /* Filtering disabled on IRQ 5. */
#if defined(VECT_ICU_IRQ7)
#define IRQ_CFG_FILT_EN_IRQ6       (0)   /* Filtering disabled on IRQ 6. */
#define IRQ_CFG_FILT_EN_IRQ7       (0)   /* Filtering disabled on IRQ 7. */
#endif
#if defined(VECT_ICU_IRQ15)
#define IRQ_CFG_FILT_EN_IRQ8       (0)   /* Filtering disabled on IRQ 8. */
#define IRQ_CFG_FILT_EN_IRQ9       (0)   /* Filtering disabled on IRQ 9. */
#define IRQ_CFG_FILT_EN_IRQ10      (0)   /* Filtering disabled on IRQ 10. */
#define IRQ_CFG_FILT_EN_IRQ11      (0)   /* Filtering disabled on IRQ 11. */
#define IRQ_CFG_FILT_EN_IRQ12      (0)   /* Filtering disabled on IRQ 12. */
#define IRQ_CFG_FILT_EN_IRQ13      (0)   /* Filtering disabled on IRQ 13. */
#define IRQ_CFG_FILT_EN_IRQ14      (0)   /* Filtering disabled on IRQ 14. */
#define IRQ_CFG_FILT_EN_IRQ15      (0)   /* Filtering disabled on IRQ 15. */
#endif


/* Example settings for IRQ input pin digital filtering sample clock divisors for each IRQ.
 * Change as required by selecting a divisor value from the IRQ_CFG_PCLK_DIVxx definitions.
 * Filtering must be enabled for the corresponding IRQ for these settings to be effective.  */
#define IRQ_CFG_FILT_PLCK_IRQ0    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 0 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ1    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 1 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ2    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 2 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ3    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 3 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ4    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 4 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ5    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 5 = PCLK. */
#if defined(VECT_ICU_IRQ7)
#define IRQ_CFG_FILT_PLCK_IRQ6    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 6 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ7    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 7 = PCLK. */
#endif
#if defined(VECT_ICU_IRQ15)
#define IRQ_CFG_FILT_PLCK_IRQ8    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 8 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ9    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 9 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ10   (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 10 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ11   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 11 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ12   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 12 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ13   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 13 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ14   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 14 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ15   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 15 = PCLK. */
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

      
#endif /* IRQ_CONFIG_H */

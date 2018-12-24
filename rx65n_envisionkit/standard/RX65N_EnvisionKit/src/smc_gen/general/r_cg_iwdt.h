/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_iwdt.h
* Version      : 1.1.0
* Device(s)    : R5F565NEHxFB
* Description  : General header file for IWDT peripheral.
* Creation Date: 2017-11-02
***********************************************************************************************************************/

#ifndef IWDT_H
#define IWDT_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
IWDT Control Register (IWDTCR)
*/
/* Time-Out Period Selection (TOPS[1:0]) */
#define _0000_IWDT_TIMEOUT_1024                 (0x0000U) /* 1024 cycles of time out period */
#define _0001_IWDT_TIMEOUT_4096                 (0x0001U) /* 4096 cycles of time out period */
#define _0002_IWDT_TIMEOUT_8192                 (0x0002U) /* 8192 cycles of time out period */
#define _0003_IWDT_TIMEOUT_16384                (0x0003U) /* 16384 cycles of time out period */
/* Clock Division Ratio Selection (CKS[3:0]) */
#define _0000_IWDT_CLOCK_DIV1                   (0x0000U) /* 1 IWDTCLK division ratio */
#define _0020_IWDT_CLOCK_DIV16                  (0x0020U) /* 16 IWDTCLK division ratio */
#define _0030_IWDT_CLOCK_DIV32                  (0x0030U) /* 32 IWDTCLK division ratio */
#define _0040_IWDT_CLOCK_DIV64                  (0x0040U) /* 64 IWDTCLK division ratio */
#define _00F0_IWDT_CLOCK_DIV128                 (0x00F0U) /* 128 IWDTCLK division ratio */
#define _0050_IWDT_CLOCK_DIV256                 (0x0050U) /* 256 IWDTCLK division ratio */ 
/* Window End Position Selection (RPES[1:0]) */
#define _0000_IWDT_WINDOW_END_75                (0x0000U) /* Window end position 75% */
#define _0100_IWDT_WINDOW_END_50                (0x0100U) /* Window end position 50% */
#define _0200_IWDT_WINDOW_END_25                (0x0200U) /* Window end position 25% */
#define _0300_IWDT_WINDOW_END_0                 (0x0300U) /* Window end position 0% */
/* Window Start Position Selection (RPSS[1:0]) */
#define _0000_IWDT_WINDOW_START_25              (0x0000U) /* Window start position 25% */
#define _1000_IWDT_WINDOW_START_50              (0x1000U) /* Window start position 50% */
#define _2000_IWDT_WINDOW_START_75              (0x2000U) /* Window start position 75% */
#define _3000_IWDT_WINDOW_START_100             (0x3000U) /* Window start position 100% */

/*
IWDT Reset Control Register (IWDTRCR)
*/
/* Reset Interrupt Request Selection (RSTIRQS) */
#define _00_IWDT_NMI_INTERRUPT                  (0x00U) /* Non-maskable interrupt request output enabled */
#define _00_IWDT_INTERRUPT                      (0x00U) /* Interrupt request output enabled */
#define _80_IWDT_RESET_OUTPUT                   (0x80U) /* Reset output enabled */

/*
IWDT Count Stop Control Register (IWDTCSTPR)
*/
/* Sleep-Mode Count Stop Control (SLCSTP) */
#define _00_IWDT_COUNT_STOP_DISABLE             (0x00U) /* Count stop is disabled */
#define _80_IWDT_COUNT_STOP_ENABLE              (0x80U) /* Count stop is enabled */

/*
Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_IWDT_PRIORITY_LEVEL0                (0x00U) /* Level 0 (interrupt disabled) */
#define _01_IWDT_PRIORITY_LEVEL1                (0x01U) /* Level 1 */
#define _02_IWDT_PRIORITY_LEVEL2                (0x02U) /* Level 2 */
#define _03_IWDT_PRIORITY_LEVEL3                (0x03U) /* Level 3 */
#define _04_IWDT_PRIORITY_LEVEL4                (0x04U) /* Level 4 */
#define _05_IWDT_PRIORITY_LEVEL5                (0x05U) /* Level 5 */
#define _06_IWDT_PRIORITY_LEVEL6                (0x06U) /* Level 6 */
#define _07_IWDT_PRIORITY_LEVEL7                (0x07U) /* Level 7 */
#define _08_IWDT_PRIORITY_LEVEL8                (0x08U) /* Level 8 */
#define _09_IWDT_PRIORITY_LEVEL9                (0x09U) /* Level 9 */
#define _0A_IWDT_PRIORITY_LEVEL10               (0x0AU) /* Level 10 */
#define _0B_IWDT_PRIORITY_LEVEL11               (0x0BU) /* Level 11 */
#define _0C_IWDT_PRIORITY_LEVEL12               (0x0CU) /* Level 12 */
#define _0D_IWDT_PRIORITY_LEVEL13               (0x0DU) /* Level 13 */
#define _0E_IWDT_PRIORITY_LEVEL14               (0x0EU) /* Level 14 */
#define _0F_IWDT_PRIORITY_LEVEL15               (0x0FU) /* Level 15 (highest) */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif


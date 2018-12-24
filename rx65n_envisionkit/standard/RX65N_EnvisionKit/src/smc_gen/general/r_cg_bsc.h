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
* File Name    : r_cg_bsc.h
* Version      : 1.1.0
* Device(s)    : R5F565NEHxFB
* Description  : General header file for BUSES peripheral.
* Creation Date: 2017-11-02
***********************************************************************************************************************/

#ifndef BSC_H
#define BSC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    System Control Register 0 (SYSCR0)
*/
/* On-Chip ROM Enable (ROME) */
#define _5A00_BSC_ROME_DISABLED             (0x5A00U) /* The on-chip ROM is disabled */
#define _5A01_BSC_ROME_ENABLED              (0x5A01U) /* The on-chip ROM is enabled */
/* External Bus Enable (EXBE) */            
#define _5A00_BSC_EXBE_DISABLED             (0x5A00U) /* The external bus is disabled */
#define _5A02_BSC_EXBE_ENABLED              (0x5A02U) /* The external bus is enabled */

/*
    CSn Control Register (CSnCR) (n = 0 to 7)
*/
/* Operation Enable (EXENB) */
#define _0000_BSC_EXENB_DISABLED            (0x0000U) /* Operation is disabled */
#define _0001_BSC_EXENB_ENABLED             (0x0001U) /* Operation is enabled */
/* External Bus Width Select (BSIZE[1:0]) */
#define _0000_BSC_BSIZE_16                  (0x0000U) /* A 16-bit bus space is selected */
#define _0010_BSC_BSIZE_32                  (0x0010U) /* A 32-bit bus space is selected */
#define _0020_BSC_BSIZE_8                   (0x0020U) /* An 8-bit bus space is selected */
/* Endian Mode (EMODE) */                   
#define _0000_BSC_EMODE_SAME                (0x0000U) /* Endian of area n is the same as the endian of
                                                         operating mode */
#define _0100_BSC_EMODE_DIFF                (0x0100U) /* Endian of area n is not the endian of operating mode */
/* Address/Data Multiplexed I/O Interface Select (MPXEN) */
#define _0000_BSC_MPXEN_SEPARATE            (0x0000U) /* Separate bus interface is selected for area n */
#define _1000_BSC_MPXEN_MULTIPLEX           (0x1000U) /* Address/data multiplexed I/O interface is selected 
                                                         for area n */

/*
    CSn Recovery Cycle Register (CSnREC) (n = 0 to 7)
*/
/* Read Recovery (RRCV[3:0]) */
#define _0000_BSC_RRCV_0                    (0x0000U) /* No recovery cycle is inserted */
#define _0001_BSC_RRCV_1                    (0x0001U) /* 1 recovery cycle is inserted */
#define _0002_BSC_RRCV_2                    (0x0002U) /* 2 recovery cycles are inserted */
#define _0003_BSC_RRCV_3                    (0x0003U) /* 3 recovery cycles are inserted */
#define _0004_BSC_RRCV_4                    (0x0004U) /* 4 recovery cycles are inserted */
#define _0005_BSC_RRCV_5                    (0x0005U) /* 5 recovery cycles are inserted */
#define _0006_BSC_RRCV_6                    (0x0006U) /* 6 recovery cycles are inserted */
#define _0007_BSC_RRCV_7                    (0x0007U) /* 7 recovery cycles are inserted */
#define _0008_BSC_RRCV_8                    (0x0008U) /* 8 recovery cycles are inserted */
#define _0009_BSC_RRCV_9                    (0x0009U) /* 9 recovery cycles are inserted */
#define _000A_BSC_RRCV_10                   (0x000AU) /* 10 recovery cycles are inserted */
#define _000B_BSC_RRCV_11                   (0x000BU) /* 11 recovery cycles are inserted */
#define _000C_BSC_RRCV_12                   (0x000CU) /* 12 recovery cycles are inserted */
#define _000D_BSC_RRCV_13                   (0x000DU) /* 13 recovery cycles are inserted */
#define _000E_BSC_RRCV_14                   (0x000EU) /* 14 recovery cycles are inserted */
#define _000F_BSC_RRCV_15                   (0x000FU) /* 15 recovery cycles are inserted */
/* Write Recovery (WRCV[3:0]) */
#define _0000_BSC_WRCV_0                    (0x0000U) /* No recovery cycle is inserted */
#define _0100_BSC_WRCV_1                    (0x0100U) /* 1 recovery cycle is inserted */
#define _0200_BSC_WRCV_2                    (0x0200U) /* 2 recovery cycles are inserted */
#define _0300_BSC_WRCV_3                    (0x0300U) /* 3 recovery cycles are inserted */
#define _0400_BSC_WRCV_4                    (0x0400U) /* 4 recovery cycles are inserted */
#define _0500_BSC_WRCV_5                    (0x0500U) /* 5 recovery cycles are inserted */
#define _0600_BSC_WRCV_6                    (0x0600U) /* 6 recovery cycles are inserted */
#define _0700_BSC_WRCV_7                    (0x0700U) /* 7 recovery cycles are inserted */
#define _0800_BSC_WRCV_8                    (0x0800U) /* 8 recovery cycles are inserted */
#define _0900_BSC_WRCV_9                    (0x0900U) /* 9 recovery cycles are inserted */
#define _0A00_BSC_WRCV_10                   (0x0A00U) /* 10 recovery cycles are inserted */
#define _0B00_BSC_WRCV_11                   (0x0B00U) /* 11 recovery cycles are inserted */
#define _0C00_BSC_WRCV_12                   (0x0C00U) /* 12 recovery cycles are inserted */
#define _0D00_BSC_WRCV_13                   (0x0D00U) /* 13 recovery cycles are inserted */
#define _0E00_BSC_WRCV_14                   (0x0E00U) /* 14 recovery cycles are inserted */
#define _0F00_BSC_WRCV_15                   (0x0F00U) /* 15 recovery cycles are inserted */

/*
    CS Recovery Cycle Insertion Enable Register (CSRECEN)
*/
/* Separate Bus Recovery Cycle Insertion Enable 0 (RCVEN0) */
#define _0000_BSC_RCVEN0_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0001_BSC_RCVEN0_ENABLED            (0x0001U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 1 (RCVEN1) */
#define _0000_BSC_RCVEN1_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0002_BSC_RCVEN1_ENABLED            (0x0002U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 2 (RCVEN2) */
#define _0000_BSC_RCVEN2_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0004_BSC_RCVEN2_ENABLED            (0x0004U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 3 (RCVEN3) */
#define _0000_BSC_RCVEN3_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0008_BSC_RCVEN3_ENABLED            (0x0008U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 4 (RCVEN4) */
#define _0000_BSC_RCVEN4_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0010_BSC_RCVEN4_ENABLED            (0x0010U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 5 (RCVEN5) */
#define _0000_BSC_RCVEN5_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0020_BSC_RCVEN5_ENABLED            (0x0020U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 6 (RCVEN6) */
#define _0000_BSC_RCVEN6_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0040_BSC_RCVEN6_ENABLED            (0x0040U) /* Recovery cycle insertion is enabled */
/* Separate Bus Recovery Cycle Insertion Enable 7 (RCVEN7) */
#define _0000_BSC_RCVEN7_DISABLED           (0x0000U) /* Recovery cycle insertion is disabled */
#define _0080_BSC_RCVEN7_ENABLED            (0x0080U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 0 (RCVENM0) */
#define _0000_BSC_RCVENM0_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _0100_BSC_RCVENM0_ENABLED           (0x0100U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 1 (RCVENM1) */
#define _0000_BSC_RCVENM1_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _0200_BSC_RCVENM1_ENABLED           (0x0200U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 2 (RCVENM2) */
#define _0000_BSC_RCVENM2_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _0400_BSC_RCVENM2_ENABLED           (0x0400U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 3 (RCVENM3) */
#define _0000_BSC_RCVENM3_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _0800_BSC_RCVENM3_ENABLED           (0x0800U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 4 (RCVENM4) */
#define _0000_BSC_RCVENM4_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _1000_BSC_RCVENM4_ENABLED           (0x1000U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 5 (RCVENM5) */
#define _0000_BSC_RCVENM5_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _2000_BSC_RCVENM5_ENABLED           (0x2000U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 6 (RCVENM6) */
#define _0000_BSC_RCVENM6_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _4000_BSC_RCVENM6_ENABLED           (0x4000U) /* Recovery cycle insertion is enabled */
/* Multiplexed Bus Recovery Cycle Insertion Enable 7 (RCVENM7) */
#define _0000_BSC_RCVENM7_DISABLED          (0x0000U) /* Recovery cycle insertion is disabled */
#define _8000_BSC_RCVENM7_ENABLED           (0x8000U) /* Recovery cycle insertion is enabled */

/*
    CSn Mode Register (CSnMOD) (n = 0 to 7)
*/
/* Write Access Mode Select (WRMOD) */
#define _0000_BSC_WRMOD_BYTE                (0x0000U) /* Byte strobe mode */
#define _0001_BSC_WRMOD_SINGLE              (0x0001U) /* Single write strobe mode */
/* External Wait Enable (EWENB) */
#define _0000_BSC_EWENB_DISABLED            (0x0000U) /* External wait is disabled */
#define _0008_BSC_EWENB_ENABLED             (0x0008U) /* External wait is enabled */
/* Page Read Access Enable (PRENB) */
#define _0000_BSC_PRENB_DISABLED            (0x0000U) /* Page read access is disabled */
#define _0100_BSC_PRENB_ENABLED             (0x0100U) /* Page read access is enabled */
/* Page Write Access Enable (PWENB) */
#define _0000_BSC_PWENB_DISABLED            (0x0000U) /* Page write access is disabled */
#define _0200_BSC_PWENB_ENABLED             (0x0200U) /* Page write access is enabled */
/* Page Read Access Mode Select (PRMOD) */
#define _0000_BSC_PRMOD_NM                  (0x0000U) /* Normal access compatible mode */
#define _8000_BSC_PRMOD_EXT                 (0x8000U) /* External data read continuous assertion mode */

/*
    CSn Wait Control Register 1 (CSnWCR1) (n = 0 to 7)
*/
/* Page Write Cycle Wait Select (CSPWWAIT[2:0]) */
#define _00000000_BSC_CSPWWAIT_0            (0x00000000UL) /* No wait is inserted */
#define _00000001_BSC_CSPWWAIT_1            (0x00000001UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00000002_BSC_CSPWWAIT_2            (0x00000002UL) /* Wait with a length of 2 clock cycle is inserted */
#define _00000003_BSC_CSPWWAIT_3            (0x00000003UL) /* Wait with a length of 3 clock cycle is inserted */
#define _00000004_BSC_CSPWWAIT_4            (0x00000004UL) /* Wait with a length of 4 clock cycle is inserted */
#define _00000005_BSC_CSPWWAIT_5            (0x00000005UL) /* Wait with a length of 5 clock cycle is inserted */
#define _00000006_BSC_CSPWWAIT_6            (0x00000006UL) /* Wait with a length of 6 clock cycle is inserted */
#define _00000007_BSC_CSPWWAIT_7            (0x00000007UL) /* Wait with a length of 7 clock cycle is inserted */
/* Page Read Cycle Wait Select (CSPRWAIT[2:0]) */
#define _00000000_BSC_CSPRWAIT_0            (0x00000000UL) /* No wait is inserted */
#define _00000100_BSC_CSPRWAIT_1            (0x00000100UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00000200_BSC_CSPRWAIT_2            (0x00000200UL) /* Wait with a length of 2 clock cycle is inserted */
#define _00000300_BSC_CSPRWAIT_3            (0x00000300UL) /* Wait with a length of 3 clock cycle is inserted */
#define _00000400_BSC_CSPRWAIT_4            (0x00000400UL) /* Wait with a length of 4 clock cycle is inserted */
#define _00000500_BSC_CSPRWAIT_5            (0x00000500UL) /* Wait with a length of 5 clock cycle is inserted */
#define _00000600_BSC_CSPRWAIT_6            (0x00000600UL) /* Wait with a length of 6 clock cycle is inserted */
#define _00000700_BSC_CSPRWAIT_7            (0x00000700UL) /* Wait with a length of 7 clock cycle is inserted */
/* Normal Write Cycle Wait Select (CSWWAIT[4:0]) */
#define _00000000_BSC_CSWWAIT_0             (0x00000000UL) /* No wait is inserted */
#define _00010000_BSC_CSWWAIT_1             (0x00010000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00020000_BSC_CSWWAIT_2             (0x00020000UL) /* Wait with a length of 2 clock cycle is inserted */
#define _00030000_BSC_CSWWAIT_3             (0x00030000UL) /* Wait with a length of 3 clock cycle is inserted */
#define _00040000_BSC_CSWWAIT_4             (0x00040000UL) /* Wait with a length of 4 clock cycle is inserted */
#define _00050000_BSC_CSWWAIT_5             (0x00050000UL) /* Wait with a length of 5 clock cycle is inserted */
#define _00060000_BSC_CSWWAIT_6             (0x00060000UL) /* Wait with a length of 6 clock cycle is inserted */
#define _00070000_BSC_CSWWAIT_7             (0x00070000UL) /* Wait with a length of 7 clock cycle is inserted */
#define _00080000_BSC_CSWWAIT_8             (0x00080000UL) /* Wait with a length of 8 clock cycle is inserted */
#define _00090000_BSC_CSWWAIT_9             (0x00090000UL) /* Wait with a length of 9 clock cycle is inserted */
#define _000A0000_BSC_CSWWAIT_10            (0x000A0000UL) /* Wait with a length of 10 clock cycle is inserted */
#define _000B0000_BSC_CSWWAIT_11            (0x000B0000UL) /* Wait with a length of 11 clock cycle is inserted */
#define _000C0000_BSC_CSWWAIT_12            (0x000C0000UL) /* Wait with a length of 12 clock cycle is inserted */
#define _000D0000_BSC_CSWWAIT_13            (0x000D0000UL) /* Wait with a length of 13 clock cycle is inserted */
#define _000E0000_BSC_CSWWAIT_14            (0x000E0000UL) /* Wait with a length of 14 clock cycle is inserted */
#define _000F0000_BSC_CSWWAIT_15            (0x000F0000UL) /* Wait with a length of 15 clock cycle is inserted */
#define _00100000_BSC_CSWWAIT_16            (0x00100000UL) /* Wait with a length of 16 clock cycle is inserted */
#define _00110000_BSC_CSWWAIT_17            (0x00110000UL) /* Wait with a length of 17 clock cycle is inserted */
#define _00120000_BSC_CSWWAIT_18            (0x00120000UL) /* Wait with a length of 18 clock cycle is inserted */
#define _00130000_BSC_CSWWAIT_19            (0x00130000UL) /* Wait with a length of 19 clock cycle is inserted */
#define _00140000_BSC_CSWWAIT_20            (0x00140000UL) /* Wait with a length of 20 clock cycle is inserted */
#define _00150000_BSC_CSWWAIT_21            (0x00150000UL) /* Wait with a length of 21 clock cycle is inserted */
#define _00160000_BSC_CSWWAIT_22            (0x00160000UL) /* Wait with a length of 22 clock cycle is inserted */
#define _00170000_BSC_CSWWAIT_23            (0x00170000UL) /* Wait with a length of 23 clock cycle is inserted */
#define _00180000_BSC_CSWWAIT_24            (0x00180000UL) /* Wait with a length of 24 clock cycle is inserted */
#define _00190000_BSC_CSWWAIT_25            (0x00190000UL) /* Wait with a length of 25 clock cycle is inserted */
#define _001A0000_BSC_CSWWAIT_26            (0x001A0000UL) /* Wait with a length of 26 clock cycle is inserted */
#define _001B0000_BSC_CSWWAIT_27            (0x001B0000UL) /* Wait with a length of 27 clock cycle is inserted */
#define _001C0000_BSC_CSWWAIT_28            (0x001C0000UL) /* Wait with a length of 28 clock cycle is inserted */
#define _001D0000_BSC_CSWWAIT_29            (0x001D0000UL) /* Wait with a length of 29 clock cycle is inserted */
#define _001E0000_BSC_CSWWAIT_30            (0x001E0000UL) /* Wait with a length of 30 clock cycle is inserted */
#define _001F0000_BSC_CSWWAIT_31            (0x001F0000UL) /* Wait with a length of 31 clock cycle is inserted */
/* Normal Read Cycle Wait Select (CSRWAIT[4:0]) */
#define _00000000_BSC_CSRWAIT_0             (0x00000000UL) /* No wait is inserted */
#define _01000000_BSC_CSRWAIT_1             (0x01000000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _02000000_BSC_CSRWAIT_2             (0x02000000UL) /* Wait with a length of 2 clock cycle is inserted */
#define _03000000_BSC_CSRWAIT_3             (0x03000000UL) /* Wait with a length of 3 clock cycle is inserted */
#define _04000000_BSC_CSRWAIT_4             (0x04000000UL) /* Wait with a length of 4 clock cycle is inserted */
#define _05000000_BSC_CSRWAIT_5             (0x05000000UL) /* Wait with a length of 5 clock cycle is inserted */
#define _06000000_BSC_CSRWAIT_6             (0x06000000UL) /* Wait with a length of 6 clock cycle is inserted */
#define _07000000_BSC_CSRWAIT_7             (0x07000000UL) /* Wait with a length of 7 clock cycle is inserted */
#define _08000000_BSC_CSRWAIT_8             (0x08000000UL) /* Wait with a length of 8 clock cycle is inserted */
#define _09000000_BSC_CSRWAIT_9             (0x09000000UL) /* Wait with a length of 9 clock cycle is inserted */
#define _0A000000_BSC_CSRWAIT_10            (0x0A000000UL) /* Wait with a length of 10 clock cycle is inserted */
#define _0B000000_BSC_CSRWAIT_11            (0x0B000000UL) /* Wait with a length of 11 clock cycle is inserted */
#define _0C000000_BSC_CSRWAIT_12            (0x0C000000UL) /* Wait with a length of 12 clock cycle is inserted */
#define _0D000000_BSC_CSRWAIT_13            (0x0D000000UL) /* Wait with a length of 13 clock cycle is inserted */
#define _0E000000_BSC_CSRWAIT_14            (0x0E000000UL) /* Wait with a length of 14 clock cycle is inserted */
#define _0F000000_BSC_CSRWAIT_15            (0x0F000000UL) /* Wait with a length of 15 clock cycle is inserted */
#define _10000000_BSC_CSRWAIT_16            (0x10000000UL) /* Wait with a length of 16 clock cycle is inserted */
#define _11000000_BSC_CSRWAIT_17            (0x11000000UL) /* Wait with a length of 17 clock cycle is inserted */
#define _12000000_BSC_CSRWAIT_18            (0x12000000UL) /* Wait with a length of 18 clock cycle is inserted */
#define _13000000_BSC_CSRWAIT_19            (0x13000000UL) /* Wait with a length of 19 clock cycle is inserted */
#define _14000000_BSC_CSRWAIT_20            (0x14000000UL) /* Wait with a length of 20 clock cycle is inserted */
#define _15000000_BSC_CSRWAIT_21            (0x15000000UL) /* Wait with a length of 21 clock cycle is inserted */
#define _16000000_BSC_CSRWAIT_22            (0x16000000UL) /* Wait with a length of 22 clock cycle is inserted */
#define _17000000_BSC_CSRWAIT_23            (0x17000000UL) /* Wait with a length of 23 clock cycle is inserted */
#define _18000000_BSC_CSRWAIT_24            (0x18000000UL) /* Wait with a length of 24 clock cycle is inserted */
#define _19000000_BSC_CSRWAIT_25            (0x19000000UL) /* Wait with a length of 25 clock cycle is inserted */
#define _1A000000_BSC_CSRWAIT_26            (0x1A000000UL) /* Wait with a length of 26 clock cycle is inserted */
#define _1B000000_BSC_CSRWAIT_27            (0x1B000000UL) /* Wait with a length of 27 clock cycle is inserted */
#define _1C000000_BSC_CSRWAIT_28            (0x1C000000UL) /* Wait with a length of 28 clock cycle is inserted */
#define _1D000000_BSC_CSRWAIT_29            (0x1D000000UL) /* Wait with a length of 29 clock cycle is inserted */
#define _1E000000_BSC_CSRWAIT_30            (0x1E000000UL) /* Wait with a length of 30 clock cycle is inserted */
#define _1F000000_BSC_CSRWAIT_31            (0x1F000000UL) /* Wait with a length of 31 clock cycle is inserted */

/*
    CSn Wait Control Register 2 (CSnWCR2) (n = 0 to 7)
*/
/* Read-Access CS Extension Cycle Select (CSROFF[2:0]) */
#define _00000000_BSC_CSROFF_0              (0x00000000UL) /* No wait is inserted */
#define _00000001_BSC_CSROFF_1              (0x00000001UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00000002_BSC_CSROFF_2              (0x00000002UL) /* Wait with a length of 2 clock cycles are inserted */
#define _00000003_BSC_CSROFF_3              (0x00000003UL) /* Wait with a length of 3 clock cycles are inserted */
#define _00000004_BSC_CSROFF_4              (0x00000004UL) /* Wait with a length of 4 clock cycles are inserted */
#define _00000005_BSC_CSROFF_5              (0x00000005UL) /* Wait with a length of 5 clock cycles are inserted */
#define _00000006_BSC_CSROFF_6              (0x00000006UL) /* Wait with a length of 6 clock cycles are inserted */
#define _00000007_BSC_CSROFF_7              (0x00000007UL) /* Wait with a length of 7 clock cycles are inserted */
/* Write-Access CS Extension Cycle Select (CSWOFF[2:0]) */
#define _00000000_BSC_CSWOFF_0              (0x00000000UL) /* No wait is inserted */
#define _00000010_BSC_CSWOFF_1              (0x00000010UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00000020_BSC_CSWOFF_2              (0x00000020UL) /* Wait with a length of 2 clock cycles are inserted */
#define _00000030_BSC_CSWOFF_3              (0x00000030UL) /* Wait with a length of 3 clock cycles are inserted */
#define _00000040_BSC_CSWOFF_4              (0x00000040UL) /* Wait with a length of 4 clock cycles are inserted */
#define _00000050_BSC_CSWOFF_5              (0x00000050UL) /* Wait with a length of 5 clock cycles are inserted */
#define _00000060_BSC_CSWOFF_6              (0x00000060UL) /* Wait with a length of 6 clock cycles are inserted */
#define _00000070_BSC_CSWOFF_7              (0x00000070UL) /* Wait with a length of 7 clock cycles are inserted */
/* Write Data Output Extension Cycle Select (WDOFF[2:0]) */
#define _00000000_BSC_WDOFF_0               (0x00000000UL) /* No wait is inserted */
#define _00000100_BSC_WDOFF_1               (0x00000100UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00000200_BSC_WDOFF_2               (0x00000200UL) /* Wait with a length of 2 clock cycles are inserted */
#define _00000300_BSC_WDOFF_3               (0x00000300UL) /* Wait with a length of 3 clock cycles are inserted */
#define _00000400_BSC_WDOFF_4               (0x00000400UL) /* Wait with a length of 4 clock cycles are inserted */
#define _00000500_BSC_WDOFF_5               (0x00000500UL) /* Wait with a length of 5 clock cycles are inserted */
#define _00000600_BSC_WDOFF_6               (0x00000600UL) /* Wait with a length of 6 clock cycles are inserted */
#define _00000700_BSC_WDOFF_7               (0x00000700UL) /* Wait with a length of 7 clock cycles are inserted */
/* Address Cycle Wait Select (AWAIT[1:0]) */
#define _00000000_BSC_AWAIT_0               (0x00000000UL) /* No wait is inserted */
#define _00001000_BSC_AWAIT_1               (0x00001000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00002000_BSC_AWAIT_2               (0x00002000UL) /* Wait with a length of 2 clock cycles are inserted */
#define _00003000_BSC_AWAIT_3               (0x00003000UL) /* Wait with a length of 3 clock cycles are inserted */
/* RD Assert Wait Select (RDON[2:0]) */
#define _00000000_BSC_RDON_0                (0x00000000UL) /* No wait is inserted */
#define _00010000_BSC_RDON_1                (0x00010000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00020000_BSC_RDON_2                (0x00020000UL) /* Wait with a length of 2 clock cycles are inserted */
#define _00030000_BSC_RDON_3                (0x00030000UL) /* Wait with a length of 3 clock cycles are inserted */
#define _00040000_BSC_RDON_4                (0x00040000UL) /* Wait with a length of 4 clock cycles are inserted */
#define _00050000_BSC_RDON_5                (0x00050000UL) /* Wait with a length of 5 clock cycles are inserted */
#define _00060000_BSC_RDON_6                (0x00060000UL) /* Wait with a length of 6 clock cycles are inserted */
#define _00070000_BSC_RDON_7                (0x00070000UL) /* Wait with a length of 7 clock cycles are inserted */
/* WR Assert Wait Select (WRON[2:0]) */
#define _00000000_BSC_WRON_0                (0x00000000UL) /* No wait is inserted */
#define _00100000_BSC_WRON_1                (0x00100000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _00200000_BSC_WRON_2                (0x00200000UL) /* Wait with a length of 2 clock cycles are inserted */
#define _00300000_BSC_WRON_3                (0x00300000UL) /* Wait with a length of 3 clock cycles are inserted */
#define _00400000_BSC_WRON_4                (0x00400000UL) /* Wait with a length of 4 clock cycles are inserted */
#define _00500000_BSC_WRON_5                (0x00500000UL) /* Wait with a length of 5 clock cycles are inserted */
#define _00600000_BSC_WRON_6                (0x00600000UL) /* Wait with a length of 6 clock cycles are inserted */
#define _00700000_BSC_WRON_7                (0x00700000UL) /* Wait with a length of 7 clock cycles are inserted */
/* Write Data Output Wait Select (WDON[2:0]) */
#define _00000000_BSC_WDON_0                (0x00000000UL) /* No wait is inserted */
#define _01000000_BSC_WDON_1                (0x01000000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _02000000_BSC_WDON_2                (0x02000000UL) /* Wait with a length of 2 clock cycles are inserted */
#define _03000000_BSC_WDON_3                (0x03000000UL) /* Wait with a length of 3 clock cycles are inserted */
#define _04000000_BSC_WDON_4                (0x04000000UL) /* Wait with a length of 4 clock cycles are inserted */
#define _05000000_BSC_WDON_5                (0x05000000UL) /* Wait with a length of 5 clock cycles are inserted */
#define _06000000_BSC_WDON_6                (0x06000000UL) /* Wait with a length of 6 clock cycles are inserted */
#define _07000000_BSC_WDON_7                (0x07000000UL) /* Wait with a length of 7 clock cycles are inserted */
/* CS Assert Wait Select (CSON[2:0]) */
#define _00000000_BSC_CSON_0                (0x00000000UL) /* No wait is inserted */
#define _10000000_BSC_CSON_1                (0x10000000UL) /* Wait with a length of 1 clock cycle is inserted */
#define _20000000_BSC_CSON_2                (0x20000000UL) /* Wait with a length of 2 clock cycles are inserted */
#define _30000000_BSC_CSON_3                (0x30000000UL) /* Wait with a length of 3 clock cycles are inserted */
#define _40000000_BSC_CSON_4                (0x40000000UL) /* Wait with a length of 4 clock cycles are inserted */
#define _50000000_BSC_CSON_5                (0x50000000UL) /* Wait with a length of 5 clock cycles are inserted */
#define _60000000_BSC_CSON_6                (0x60000000UL) /* Wait with a length of 6 clock cycles are inserted */
#define _70000000_BSC_CSON_7                (0x70000000UL) /* Wait with a length of 7 clock cycles are inserted */

/*
    SDC Control Register (SDCCR)
*/
/* Operation Enable (EXENB) */
#define _00_BSC_EXENB_DISABLED              (0x00U) /* Operation is disabled */
#define _01_BSC_EXENB_ENABLED               (0x01U) /* Operation is enabled */
/* SDRAM Bus Width Select (BSIZE[1:0]) */
#define _00_BSC_BSIZE_16                    (0x00U) /* A 16-bit bus space is selected */
#define _10_BSC_BSIZE_32                    (0x10U) /* A 32-bit bus space is selected */
#define _20_BSC_BSIZE_8                     (0x20U) /* An 8-bit bus space is selected */

/*
    SDC Mode Register (SDCMOD)
*/
/* Endian mode (EMODE) */
#define _00_BSC_SD_EMODE_SAME               (0x00U) /* Endian of SDRAM address space is same as operating mode */
#define _01_BSC_SD_EMODE_DIFF               (0x01U) /* Endian of SDRAM address space is not endian of operating mode */

/*
    SDRAM Access Mode Register (SDAMOD)
*/
/* Continuous Access Enable (BE) */
#define _00_BSC_SD_CONTINUOUS_DISABLED      (0x00U) /* Continuous access is disabled */
#define _01_BSC_SD_CONTINUOUS_ENABLED       (0x01U) /* Continuous access is enabled */

/*
    SDRAM Self-Refresh Control Register (SDSELF)
*/
/* SDRAM Self-Refresh Enable (SFEN) */
#define _00_BSC_SD_SELF_REFRESH_DISABLED    (0x00U) /* Self-refresh is disabled */
#define _01_BSC_SD_SELF_REFRESH_ENABLED     (0x01U) /* Self-refresh is enabled */

/*
    SDRAM Refresh Control Register (SDRFCR)
*/
/* Auto-Refresh Cycle/ Self-Refresh Clearing Cycle Count Setting (REFW[3:0]) */
#define _0000_BSC_REFW_1                    (0x0000U) /* 1 cycle */
#define _1000_BSC_REFW_2                    (0x1000U) /* 2 cycles */
#define _2000_BSC_REFW_3                    (0x2000U) /* 3 cycles */
#define _3000_BSC_REFW_4                    (0x3000U) /* 4 cycles */
#define _4000_BSC_REFW_5                    (0x4000U) /* 5 cycles */
#define _5000_BSC_REFW_6                    (0x5000U) /* 6 cycles */
#define _6000_BSC_REFW_7                    (0x6000U) /* 7 cycles */
#define _7000_BSC_REFW_8                    (0x7000U) /* 8 cycles */
#define _8000_BSC_REFW_9                    (0x8000U) /* 9 cycles */
#define _9000_BSC_REFW_10                   (0x9000U) /* 10 cycles */
#define _A000_BSC_REFW_11                   (0xA000U) /* 11 cycles */
#define _B000_BSC_REFW_12                   (0xB000U) /* 12 cycles */
#define _C000_BSC_REFW_13                   (0xC000U) /* 13 cycles */
#define _D000_BSC_REFW_14                   (0xD000U) /* 14 cycles */
#define _E000_BSC_REFW_15                   (0xE000U) /* 15 cycles */
#define _F000_BSC_REFW_16                   (0xF000U) /* 16 cycles */

/*
    SDRAM Auto-Refresh Control Register (SDRFEN)
*/
/* Auto-Refresh Operation Enable (RFEN) */
#define _00_BSC_SD_AUTO_REFRESH_DISABLED    (0x00U) /* Auto-refresh is disabled */
#define _01_BSC_SD_AUTO_REFRESH_ENABLED     (0x01U) /* Auto-refresh is enabled */

/*
    SDRAM Initialization Sequence Control Register (SDICR)
*/
/* Initialization Sequence Start (INIRQ) */
#define _00_BSC_INIT_SEQUENCE_INVALID       (0x00U) /* Invalid */
#define _01_BSC_INIT_SEQUENCE_START         (0x01U) /* Initialization sequence starts */

/*
    SDRAM Initialization Register (SDIR)
*/
/* Initialization Auto-Refresh Interval (ARFI[3:0]) */
#define _0000_BSC_ARFI_3                    (0x0000U) /* 3 cycles */
#define _0001_BSC_ARFI_4                    (0x0001U) /* 4 cycles */
#define _0002_BSC_ARFI_5                    (0x0002U) /* 5 cycles */
#define _0003_BSC_ARFI_6                    (0x0003U) /* 6 cycles */
#define _0004_BSC_ARFI_7                    (0x0004U) /* 7 cycles */
#define _0005_BSC_ARFI_8                    (0x0005U) /* 8 cycles */
#define _0006_BSC_ARFI_9                    (0x0006U) /* 9 cycles */
#define _0007_BSC_ARFI_10                   (0x0007U) /* 10 cycles */
#define _0008_BSC_ARFI_11                   (0x0008U) /* 11 cycles */
#define _0009_BSC_ARFI_12                   (0x0009U) /* 12 cycles */
#define _000A_BSC_ARFI_13                   (0x000AU) /* 13 cycles */
#define _000B_BSC_ARFI_14                   (0x000BU) /* 14 cycles */
#define _000C_BSC_ARFI_15                   (0x000CU) /* 15 cycles */
#define _000D_BSC_ARFI_16                   (0x000DU) /* 16 cycles */
#define _000E_BSC_ARFI_17                   (0x000EU) /* 17 cycles */
#define _000F_BSC_ARFI_18                   (0x000FU) /* 18 cycles */
/* Initialization Auto-Refresh Count (ARFC[3:0]) */
#define _0010_BSC_ARFC_1                    (0x0010U) /* 1 time */
#define _0020_BSC_ARFC_2                    (0x0020U) /* 2 times */
#define _0030_BSC_ARFC_3                    (0x0030U) /* 3 times */
#define _0040_BSC_ARFC_4                    (0x0040U) /* 4 times */
#define _0050_BSC_ARFC_5                    (0x0050U) /* 5 times */
#define _0060_BSC_ARFC_6                    (0x0060U) /* 6 times */
#define _0070_BSC_ARFC_7                    (0x0070U) /* 7 times */
#define _0080_BSC_ARFC_8                    (0x0080U) /* 8 times */
#define _0090_BSC_ARFC_9                    (0x0090U) /* 9 times */
#define _00A0_BSC_ARFC_10                   (0x00A0U) /* 10 times */
#define _00B0_BSC_ARFC_11                   (0x00B0U) /* 11 times */
#define _00C0_BSC_ARFC_12                   (0x00C0U) /* 12 times */
#define _00D0_BSC_ARFC_13                   (0x00D0U) /* 13 times */
#define _00E0_BSC_ARFC_14                   (0x00E0U) /* 14 times */
#define _00F0_BSC_ARFC_15                   (0x00F0U) /* 15 times */
/* Initialization Precharge Cycle Count (PRC[2:0]) */
#define _0000_BSC_PRC_3                     (0x0000U) /* 3 cycles */
#define _0100_BSC_PRC_4                     (0x0100U) /* 4 cycles */
#define _0200_BSC_PRC_5                     (0x0200U) /* 5 cycles */
#define _0300_BSC_PRC_6                     (0x0300U) /* 6 cycles */
#define _0400_BSC_PRC_7                     (0x0400U) /* 7 cycles */
#define _0500_BSC_PRC_8                     (0x0500U) /* 8 cycles */
#define _0600_BSC_PRC_9                     (0x0600U) /* 9 cycles */
#define _0700_BSC_PRC_10                    (0x0700U) /* 10 cycles */

/*
    SDRAM Address Register (SDADR)
*/
/* Address Multiplex Select (MXC[1:0]) */
#define _00_BSC_MXC_BIT_SHIFT_8             (0x00U) /* 8-bit shift */
#define _01_BSC_MXC_BIT_SHIFT_9             (0x01U) /* 9-bit shift */
#define _02_BSC_MXC_BIT_SHIFT_10            (0x02U) /* 10-bit shift */
#define _03_BSC_MXC_BIT_SHIFT_11            (0x03U) /* 11-bit shift */

/*
    SDRAM Timing Register (SDTR)
*/
/* SDRAMC Column Latency (CL[2:0]) */
#define _00000001_BSC_CL_1                  (0x00000001UL) /* 1 cycle */
#define _00000002_BSC_CL_2                  (0x00000002UL) /* 2 cycles */
#define _00000003_BSC_CL_3                  (0x00000003UL) /* 3 cycles */
/* Write Recovery Interval (WR) */
#define _00000000_BSC_WR_1                  (0x00000000UL) /* 1 cycle */
#define _00000100_BSC_WR_2                  (0x00000100UL) /* 2 cycles */
/* Row Precharge Interval (RP[2:0]) */
#define _00000000_BSC_RP_1                  (0x00000000UL) /* 1 cycle */
#define _00000200_BSC_RP_2                  (0x00000200UL) /* 2 cycles */
#define _00000400_BSC_RP_3                  (0x00000400UL) /* 3 cycles */
#define _00000600_BSC_RP_4                  (0x00000600UL) /* 4 cycles */
#define _00000800_BSC_RP_5                  (0x00000800UL) /* 5 cycles */
#define _00000A00_BSC_RP_6                  (0x00000A00UL) /* 6 cycles */
#define _00000C00_BSC_RP_7                  (0x00000C00UL) /* 7 cycles */
#define _00000E00_BSC_RP_8                  (0x00000E00UL) /* 8 cycles */
/* Row Column Latency (RCD[1:0]) */
#define _00000000_BSC_RCD_1                 (0x00000000UL) /* 1 cycle */
#define _00001000_BSC_RCD_2                 (0x00001000UL) /* 2 cycles */
#define _00002000_BSC_RCD_3                 (0x00002000UL) /* 3 cycles */
#define _00003000_BSC_RCD_4                 (0x00003000UL) /* 4 cycles */
/* Row Active Interval (RAS[2:0]) */
#define _00000000_BSC_RAS_1                 (0x00000000UL) /* 1 cycle */
#define _00010000_BSC_RAS_2                 (0x00010000UL) /* 2 cycles */
#define _00020000_BSC_RAS_3                 (0x00020000UL) /* 3 cycles */
#define _00030000_BSC_RAS_4                 (0x00030000UL) /* 4 cycles */
#define _00040000_BSC_RAS_5                 (0x00040000UL) /* 5 cycles */
#define _00050000_BSC_RAS_6                 (0x00050000UL) /* 6 cycles */
#define _00060000_BSC_RAS_7                 (0x00060000UL) /* 7 cycles */

/*
    Bus Error Monitoring Enable Register (BEREN)
*/
/* Illegal Address Access Detection Enable (IGAEN) */
#define _00_BSC_IGAEN_DISABLE               (0x00U) /* Illegal address access detection is disabled */
#define _01_BSC_IGAEN_ENABLE                (0x01U) /* Illegal address access detection is enabled */
/* Timeout Detection Enable (TOEN) */
#define _00_BSC_TOEN_DISABLE                (0x00U) /* Bus timeout detection is disabled */
#define _02_BSC_TOEN_ENABLE                 (0x02U) /* Bus timeout detection is enabled */

/*
    Bus Priority Control Register (BUSPRI)
*/
/* Memory Bus 1 (RAM) Priority Control (BPRA[1:0]) */
#define _0000_BSC_BPRA_FIX                  (0x0000U) /* The order of priority is fixed */
#define _0001_BSC_BPRA_TOGGLE               (0x0001U) /* The order of priority is toggled */
/* Memory Bus 2 (ROM) Priority Control (BPRO[1:0]) */
#define _0000_BSC_BPRO_FIX                  (0x0000U) /* The order of priority is fixed */
#define _0004_BSC_BPRO_TOGGLE               (0x0004U) /* The order of priority is toggled */
/* Internal Peripheral Bus 1 Priority Control (BPIB[1:0]) */
#define _0000_BSC_BPIB_FIX                  (0x0000U) /* The order of priority is fixed */
#define _0010_BSC_BPIB_TOGGLE               (0x0010U) /* The order of priority is toggled */
/* Internal Peripheral Bus 2 and 3 Priority Control (BPGB[1:0]) */
#define _0000_BSC_BPGB_FIX                  (0x0000U) /* The order of priority is fixed */
#define _0040_BSC_BPGB_TOGGLE               (0x0040U) /* The order of priority is toggled */
/* Internal Peripheral Bus 4 and 5 Priority Control (BPHB[1:0]) */
#define _0000_BSC_BPHB_FIX                  (0x0000U) /* The order of priority is fixed */
#define _0100_BSC_BPHB_TOGGLE               (0x0100U) /* The order of priority is toggled */
/* Internal Peripheral Bus 6 Priority Control (BPFB[1:0]) */
#define _0000_BSC_BPFB_FIX                  (0x0000U) /* The order of priority is fixed */
#define _0400_BSC_BPFB_TOGGLE               (0x0400U) /* The order of priority is toggled */
/* External Bus Priority Control (BPEB[1:0]) */
#define _0000_BSC_BPEB_FIX                  (0x0000U) /* The order of priority is fixed */
#define _1000_BSC_BPEB_TOGGLE               (0x1000U) /* The order of priority is toggled */

/*
    CS Output Enable Register (PFCSE)
*/
/* CS0 Enable (CS0E) */
#define _00_BSC_CS0E_DISABLED               (0x00U) /* Disables CS0# output */
#define _01_BSC_CS0E_ENABLED                (0x01U) /* Enables CS0# output */
/* CS1 Enable (CS1E) */                     
#define _00_BSC_CS1E_DISABLED               (0x00U) /* Disables CS1# output */
#define _02_BSC_CS1E_ENABLED                (0x02U) /* Enables CS1# output */
/* CS2 Enable (CS2E) */                     
#define _00_BSC_CS2E_DISABLED               (0x00U) /* Disables CS2# output */
#define _04_BSC_CS2E_ENABLED                (0x04U) /* Enables CS2# output */
/* CS3 Enable (CS3E) */                     
#define _00_BSC_CS3E_DISABLED               (0x00U) /* Disables CS3# output */
#define _08_BSC_CS3E_ENABLED                (0x08U) /* Enables CS3# output */
/* CS4 Enable (CS4E) */                     
#define _00_BSC_CS4E_DISABLED               (0x00U) /* Disables CS4# output */
#define _10_BSC_CS4E_ENABLED                (0x10U) /* Enables CS4# output */
/* CS5 Enable (CS5E) */                     
#define _00_BSC_CS5E_DISABLED               (0x00U) /* Disables CS5# output */
#define _20_BSC_CS5E_ENABLED                (0x20U) /* Enables CS5# output */
/* CS6 Enable (CS6E) */                     
#define _00_BSC_CS6E_DISABLED               (0x00U) /* Disables CS6# output */
#define _40_BSC_CS6E_ENABLED                (0x40U) /* Enables CS6# output */
/* CS7 Enable (CS7E) */                     
#define _00_BSC_CS7E_DISABLED               (0x00U) /* Disables CS7# output */
#define _80_BSC_CS7E_ENABLED                (0x80U) /* Enables CS7# output */

/*
    CS Output Pin Select Register 0 (PFCSS0)
*/
/* CS0# Output Pin Select (CS0S) */
#define _00_BSC_CS0S_P60                    (0x00U) /* Set P60 as CS0# output pin */
#define _01_BSC_CS0S_PC7                    (0x01U) /* Set PC7 as CS0# output pin */
/* CS1# Output Pin Select (CS1S[1:0]) */
#define _00_BSC_CS1S_P61                    (0x00U) /* Set P61 as CS1# output pin */
#define _04_BSC_CS1S_P71                    (0x04U) /* Set P71 as CS1# output pin */
#define _08_BSC_CS1S_PC6                    (0x08U) /* Set PC6 as CS1# output pin */
/* CS2# Output Pin Select (CS2S[1:0]) */
#define _00_BSC_CS2S_P62                    (0x00U) /* Set P62 as CS2# output pin */
#define _10_BSC_CS2S_P72                    (0x10U) /* Set P72 as CS2# output pin */
#define _20_BSC_CS2S_PC5                    (0x20U) /* Set PC5 as CS2# output pin */
/* CS3# Output Pin Select (CS3S[1:0]) */
#define _00_BSC_CS3S_P63                    (0x00U) /* Set P63 as CS3# output pin */
#define _40_BSC_CS3S_P73                    (0x40U) /* Set P73 as CS3# output pin */
#define _80_BSC_CS3S_PC4                    (0x80U) /* Set PC4 as CS3# output pin */

/*
    CS Output Pin Select Register 1 (PFCSS1)
*/
/* CS4# Output Pin Select (CS4S[1:0]) */
#define _00_BSC_CS4S_P64                    (0x00U) /* Set P64 as CS4# output pin */
#define _01_BSC_CS4S_P74                    (0x01U) /* Set P74 as CS4# output pin */
#define _02_BSC_CS4S_P24                    (0x02U) /* Set P24 as CS4# output pin */
/* CS5# Output Pin Select (CS5S[1:0]) */
#define _00_BSC_CS5S_P65                    (0x00U) /* Set P65 as CS5# output pin */
#define _04_BSC_CS5S_P75                    (0x04U) /* Set P75 as CS5# output pin */
#define _08_BSC_CS5S_P25                    (0x08U) /* Set P25 as CS5# output pin */
/* CS6# Output Pin Select (CS6S[1:0]) */
#define _00_BSC_CS6S_P66                    (0x00U) /* Set P66 as CS6# output pin */
#define _10_BSC_CS6S_P76                    (0x10U) /* Set P76 as CS6# output pin */
#define _20_BSC_CS6S_P26                    (0x20U) /* Set P26 as CS6# output pin */
/* CS7# Output Pin Select (CS7S[1:0]) */
#define _00_BSC_CS7S_P67                    (0x00U) /* Set P67 as CS7# output pin */
#define _40_BSC_CS7S_P77                    (0x40U) /* Set P77 as CS7# output pin */
#define _80_BSC_CS7S_P27                    (0x80U) /* Set P27 as CS7# output pin */

/*
    Address Output Enable Register 0 (PFAOE0)
*/
/* Address A8 Output Enable (A8E) */
#define _00_BSC_A8E_DISABLED                (0x00U) /* Disables A8 output */
#define _01_BSC_A8E_ENABLED                 (0x01U) /* Enables A8 output */
/* Address A9 Output Enable (A9E) */
#define _00_BSC_A9E_DISABLED                (0x00U) /* Disables A9 output */
#define _02_BSC_A9E_ENABLED                 (0x02U) /* Enables A9 output */
/* Address A10 Output Enable (A10E) */
#define _00_BSC_A10E_DISABLED               (0x00U) /* Disables A10 output */
#define _04_BSC_A10E_ENABLED                (0x04U) /* Enables A10 output */
/* Address A11 Output Enable (A11E) */
#define _00_BSC_A11E_DISABLED               (0x00U) /* Disables A11 output */
#define _08_BSC_A11E_ENABLED                (0x08U) /* Enables A11 output */
/* Address A12 Output Enable (A12E) */
#define _00_BSC_A12E_DISABLED               (0x00U) /* Disables A12 output */
#define _10_BSC_A12E_ENABLED                (0x10U) /* Enables A12 output */
/* Address A13 Output Enable (A13E) */
#define _00_BSC_A13E_DISABLED               (0x00U) /* Disables A13 output */
#define _20_BSC_A13E_ENABLED                (0x20U) /* Enables A13 output */
/* Address A14 Output Enable (A14E) */
#define _00_BSC_A14E_DISABLED               (0x00U) /* Disables A14 output */
#define _40_BSC_A14E_ENABLED                (0x40U) /* Enables A14 output */
/* Address A15 Output Enable (A15E) */
#define _00_BSC_A15E_DISABLED               (0x00U) /* Disables A15 output */
#define _80_BSC_A15E_ENABLED                (0x80U) /* Enables A15 output */

/*
    Address Output Enable Register 1 (PFAOE1)
*/
/* Address A16 Output Enable (A16E) */
#define _00_BSC_A16E_DISABLED               (0x00U) /* Disables A16 output */
#define _01_BSC_A16E_ENABLED                (0x01U) /* Enables A16 output */
/* Address A17 Output Enable (A17E) */
#define _00_BSC_A17E_DISABLED               (0x00U) /* Disables A17 output */
#define _02_BSC_A17E_ENABLED                (0x02U) /* Enables A17 output */
/* Address A18 Output Enable (A18E) */
#define _00_BSC_A18E_DISABLED               (0x00U) /* Disables A18 output */
#define _04_BSC_A18E_ENABLED                (0x04U) /* Enables A18 output */
/* Address A19 Output Enable (A19E) */
#define _00_BSC_A19E_DISABLED               (0x00U) /* Disables A19 output */
#define _08_BSC_A19E_ENABLED                (0x08U) /* Enables A19 output */
/* Address A20 Output Enable (A20E) */
#define _00_BSC_A20E_DISABLED               (0x00U) /* Disables A20 output */
#define _10_BSC_A20E_ENABLED                (0x10U) /* Enables A20 output */
/* Address A21 Output Enable (A21E) */
#define _00_BSC_A21E_DISABLED               (0x00U) /* Disables A21 output */
#define _20_BSC_A21E_ENABLED                (0x20U) /* Enables A21 output */
/* Address A22 Output Enable (A22E) */
#define _00_BSC_A22E_DISABLED               (0x00U) /* Disables A22 output */
#define _40_BSC_A22E_ENABLED                (0x40U) /* Enables A22 output */
/* Address A23 Output Enable (A23E) */
#define _00_BSC_A23E_DISABLED               (0x00U) /* Disables A23 output */
#define _80_BSC_A23E_ENABLED                (0x80U) /* Enables A23 output */

/*
    External Bus Control Register 0 (PFBCR0)
*/
/* A0 to A7 Output Enable (ADRLE) */
#define _00_BSC_ADRLE_DISABLED              (0x00U) /* Configures PA0 to PA7 as the I/O port pins */
#define _01_BSC_ADRLE_ENABLED               (0x01U) /* Configures PA0 to PA7 as the external address bus A0~A7 */
/* A16 to A23 Output Enable (ADRHMS), A16 to A20 Output Enable (ADRHMS2) */
#define _00_BSC_ADRHM_PC0_PC7               (0x00U) /* Set PC0 to PC7 */
#define _04_BSC_ADRHM_PC0_P7n_PC7           (0x04U) /* Set PC0, PC1, P71, P72, P74, and PC5 to PC7 */
#define _02_BSC_ADRHM_P90_P93               (0x02U) /* Set P90 to P93. (No allocation of A20 to A23) */
#define _02_BSC_ADRHM_P90_P97               (0x02U) /* Set P90 to P97. */
/* BCLK Forced Output (BCLKO) */
#define _00_BSC_BCLKO_DISABLED              (0x00U) /* BCLK is output when EXBE = 1 and not output when EXBE = 0 */
#define _08_BSC_BCLKO_ENABLED               (0x08U) /* BCLK is output regardless of the setting of EXBE */
/* D8 to D15 Output Enable (DHE) */
#define _00_BSC_DHE_DISABLED                (0x00U) /* Configures PE0 to PE7 as the I/O port pins */
#define _10_BSC_DHE_ENABLED                 (0x10U) /* Configures PE0 to PE7 as the external data bus D8~D15 */
/* D16 to D31 Output Enable (DH32E) */
#define _00_BSC_DH32E_DISABLED              (0x00U) /* Configures PG7 to PG0, and P97 to P90 as I/O port */
#define _20_BSC_DH32E_ENABLED               (0x20U) /* Configures PG7 to PG0, and P97 to P90 as external data bus D31 to D16 */
/* WR1#/BC1# Output Enable (WR1BC1E) */
#define _00_BSC_WR1BC1E_DISABLED            (0x00U) /* Configures P51 as the I/O port pin */
#define _40_BSC_WR1BC1E_ENABLED             (0x40U) /* Configures P51 as the WR1# or BC1# pin */
/* WR2#/BC2# and WR3#/BC3# Output Enable */
#define _00_BSC_WR32BC32E_DISABLED          (0x00U) /* Configures P13 and P12 as I/O port pin */
#define _80_BSC_WR32BC32E_ENABLED           (0x80U) /* Configures P13 as WR#2 or BC2# pin, and set P12 as WR#3 or BC3# pin*/

/*
    External Bus Control Register 1 (PFBCR1)
*/
/* WAIT Select (WAITS[1:0]) */
#define _01_BSC_WAITS_P55                   (0x01U) /* Configures P55 as the WAIT# input pin */
#define _02_BSC_WAITS_PC5                   (0x02U) /* Configures PC5 as the WAIT# input pin */
#define _03_BSC_WAITS_P51                   (0x03U) /* Configures P51 as the WAIT# input pin */
/* ALE Output Enable (ALEOE) */             
#define _00_BSC_ALEOE_DISABLED              (0x00U) /* Disables ALE pin output */
#define _04_BSC_ALEOE_ENABLED               (0x04U) /* Enables ALE pin output */
/* SDRAM Pin Enable (MDSDE) */              
#define _00_BSC_MDSDE_DISABLED              (0x00U) /* Disables SDRAM pin output */
#define _10_BSC_MDSDE_ENABLED               (0x10U) /* Enables SDRAM pin output */
/* DQM1 Enable (DQM1E) */                   
#define _00_BSC_DQM1E_DISABLED              (0x00U) /* Disables DQM1 output */
#define _40_BSC_DQM1E_ENABLED               (0x40U) /* Enables DQM1 output */
/* SDCLK Enable (SDCLKE) */                 
#define _00_BSC_SDCLKE_DISABLED             (0x00U) /* Disables SDCLK output */
#define _80_BSC_SDCLKE_ENABLED              (0x80U) /* Enables SDCLK output */
/* ALE Select (ALES) */
#define _00_BSC_ALES_P54                    (0x00U) /* Configures P54 as the ALE pin */
#define _08_BSC_ALES_P10                    (0x08U) /* Configures P10 as the ALE pin */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_BSC_PRIORITY_LEVEL0             (0x00U) /* Level 0 (interrupt disabled) */
#define _01_BSC_PRIORITY_LEVEL1             (0x01U) /* Level 1 */
#define _02_BSC_PRIORITY_LEVEL2             (0x02U) /* Level 2 */
#define _03_BSC_PRIORITY_LEVEL3             (0x03U) /* Level 3 */
#define _04_BSC_PRIORITY_LEVEL4             (0x04U) /* Level 4 */
#define _05_BSC_PRIORITY_LEVEL5             (0x05U) /* Level 5 */
#define _06_BSC_PRIORITY_LEVEL6             (0x06U) /* Level 6 */
#define _07_BSC_PRIORITY_LEVEL7             (0x07U) /* Level 7 */
#define _08_BSC_PRIORITY_LEVEL8             (0x08U) /* Level 8 */
#define _09_BSC_PRIORITY_LEVEL9             (0x09U) /* Level 9 */
#define _0A_BSC_PRIORITY_LEVEL10            (0x0AU) /* Level 10 */
#define _0B_BSC_PRIORITY_LEVEL11            (0x0BU) /* Level 11 */
#define _0C_BSC_PRIORITY_LEVEL12            (0x0CU) /* Level 12 */
#define _0D_BSC_PRIORITY_LEVEL13            (0x0DU) /* Level 13 */
#define _0E_BSC_PRIORITY_LEVEL14            (0x0EU) /* Level 14 */
#define _0F_BSC_PRIORITY_LEVEL15            (0x0FU) /* Level 15 (highest) */

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


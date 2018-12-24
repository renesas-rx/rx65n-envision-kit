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
* File Name    : r_cg_poe.h
* Version      : 1.1.0
* Device(s)    : R5F565NEHxFB
* Description  : General header file for POE peripheral.
* Creation Date: 2017-11-02
***********************************************************************************************************************/

#ifndef POE_H
#define POE_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Input Level Control/Status Register 1 (ICSR1)
*/
/* POE0 Mode Select (POE0M) */
#define _0000_POE_POE0M_SEL_DEFAULT      (0x0000U) /* Accepts request on the falling edge of POE0# input */
#define _0001_POE_POE0M_SEL_8            (0x0001U) /* Accepts request when sampled 16 times at PCLK/8 and all low */
#define _0002_POE_POE0M_SEL_16           (0x0002U) /* Accepts request when sampled 16 times at PCLK/16 and all low */
#define _0003_POE_POE0M_SEL_128          (0x0003U) /* Accepts request when sampled 16 times at PCLK/128 and all low */
/* Port Interrupt Enable 1 (PIE1) */
#define _0000_POE_PIE1_DISABLE           (0x0000U) /* OEI1 interrupt requests by the input detection disabled */
#define _0100_POE_PIE1_ENABLE            (0x0100U) /* OEI1 interrupt requests by the input detection enabled */

/*
    Input Level Control/Status Register 2 (ICSR2)
*/
/* POE4 Mode Select (POE4M) */
#define _0000_POE_POE4M_SEL_DEFAULT      (0x0000U) /* Accepts request on the falling edge of POE4# input */
#define _0001_POE_POE4M_SEL_8            (0x0001U) /* Accepts request when sampled 16 times at PCLK/8 and all low */
#define _0002_POE_POE4M_SEL_16           (0x0002U) /* Accepts request when sampled 16 times at PCLK/16 and all low */
#define _0003_POE_POE4M_SEL_128          (0x0003U) /* Accepts request when sampled 16 times at PCLK/128 and all low */
/* Port Interrupt Enable 2 (PIE2) */
#define _0000_POE_PIE2_DISABLE           (0x0000U) /* OEI2 interrupt requests by the input detection disabled */
#define _0100_POE_PIE2_ENABLE            (0x0100U) /* OEI2 interrupt requests by the input detection enabled */

/*
    Input Level Control/Status Register 3 (ICSR3)
*/
/* POE8 Mode Select (POE8M) */
#define _0000_POE_POE8M_SEL_DEFAULT      (0x0000U) /* Accepts request on the falling edge of POE8# input */
#define _0001_POE_POE8M_SEL_8            (0x0001U) /* Accepts request when sampled 16 times at PCLK/8 and all low */
#define _0002_POE_POE8M_SEL_16           (0x0002U) /* Accepts request when sampled 16 times at PCLK/16 and all low */
#define _0003_POE_POE8M_SEL_128          (0x0003U) /* Accepts request when sampled 16 times at PCLK/128 and all low */
/* Port Interrupt Enable 3 (PIE3) */
#define _0000_POE_PIE3_DISABLE           (0x0000U) /* OEI3 interrupt requests by the input detection disabled */
#define _0100_POE_PIE3_ENABLE            (0x0100U) /* OEI3 interrupt requests by the input detection enabled */
/* POE8 High-Impedance Enable (POE8E) */
#define _0000_POE_POE8E_DISABLE          (0x0000U) /* Not places the POE8 pin in high-impedance state */
#define _0200_POE_POE8E_ENABLE           (0x0200U) /* Places the POE8 pin in high-impedance state */

/*
    Input Level Control/Status Register 4 (ICSR4)
*/
/* POE10 Mode Select (POE10M) */
#define _0000_POE_POE10M_SEL_DEFAULT     (0x0000U) /* Accepts request on the falling edge of POE10# input */
#define _0001_POE_POE10M_SEL_8           (0x0001U) /* Accepts request when sampled 16 times at PCLK/8 and all low */
#define _0002_POE_POE10M_SEL_16          (0x0002U) /* Accepts request when sampled 16 times at PCLK/16 and all low */
#define _0003_POE_POE10M_SEL_128         (0x0003U) /* Accepts request when sampled 16 times at PCLK/128 and all low */
/* Port Interrupt Enable 4 (PIE4) */
#define _0000_POE_PIE4_DISABLE           (0x0000U) /* OEI4 interrupt requests by the input detection disabled */
#define _0100_POE_PIE4_ENABLE            (0x0100U) /* OEI4 interrupt requests by the input detection enabled */
/* POE10 High-Impedance Enable (POE10E) */
#define _0000_POE_POE10E_DISABLE         (0x0000U) /* Not places the POE10 pin in high-impedance state */
#define _0200_POE_POE10E_ENABLE          (0x0200U) /* Places the POE10 pin in high-impedance state */

/*
    Input Level Control/Status Register 5 (ICSR5)
*/
/* POE11 Mode Select (POE11M) */
#define _0000_POE_POE11M_SEL_DEFAULT     (0x0000U) /* Accepts request on the falling edge of POE11# input */
#define _0001_POE_POE11M_SEL_8           (0x0001U) /* Accepts request when sampled 16 times at PCLK/8 and all low */
#define _0002_POE_POE11M_SEL_16          (0x0002U) /* Accepts request when sampled 16 times at PCLK/16 and all low */
#define _0003_POE_POE11M_SEL_128         (0x0003U) /* Accepts request when sampled 16 times at PCLK/128 and all low */
/* Port Interrupt Enable 5 (PIE5) */
#define _0000_POE_PIE5_DISABLE           (0x0000U) /* OEI5 interrupt requests by the input detection disabled */
#define _0100_POE_PIE5_ENABLE            (0x0100U) /* OEI5 interrupt requests by the input detection enabled */
/* POE11 High-Impedance Enable (POE11E) */
#define _0000_POE_POE11E_DISABLE         (0x0000U) /* Not places the POE11 pin in high-impedance state */
#define _0200_POE_POE11E_ENABLE          (0x0200U) /* Places the POE11 pin in high-impedance state */

/*
    Input Level Control/Status Register 6 (ICSR6)
*/
/* OSTST High-Impedance Enable (OSTSTE) */
#define _0000_POE_OSTSTE_DISABLE         (0x0000U) /* MTU0 and complementary PWM output pins high-impedance
                                                    are disabled */
#define _0200_POE_OSTSTE_ENABLE          (0x0200U) /* MTU0 and complementary PWM output pins high-impedance 
                                                    are enabled */

/*
    Output Level Control/Status Register 1 (OCSR1)
*/
/* Output Short Interrupt Enable 1 (OIE1) */
#define _0000_POE_OIE1_DISABLE           (0x0000U) /* OEI1 interrupt requests by the output level comparison disabled */
#define _0100_POE_OIE1_ENABLE            (0x0100U) /* OEI1 interrupt requests by the output level comparison enabled */
/* Output Short High-Impedance Enable 1 (OCE1) */
#define _0000_POE_OCE1_DISABLE           (0x0000U) /* Does not place the pins in high-impedance */
#define _0200_POE_OCE1_ENABLE            (0x0200U) /* Places the pins in high-impedance */

/*
    Output Level Control/Status Register 2 (OCSR2)
*/
/* Output Short Interrupt Enable 2 (OIE2) */
#define _0000_POE_OIE2_DISABLE           (0x0000U) /* OEI2 interrupt requests by the output level comparison disabled */
#define _0100_POE_OIE2_ENABLE            (0x0100U) /* OEI2 interrupt requests by the output level comparison enabled */
/* Output Short High-Impedance Enable 2 (OCE2) */
#define _0000_POE_OCE2_DISABLE           (0x0000U) /* Does not place the pins in high-impedance */
#define _0200_POE_OCE2_ENABLE            (0x0200U) /* Places the pins in high-impedance */

/*
    Active Level Setting Register 1 (ALR1)
*/
/* MTIOC3B Active Level Setting (OLSG0A) */
#define _0000_POE_MTIOC3B_LOW            (0x0000U) /* MTIOC3B active level low */
#define _0001_POE_MTIOC3B_HIGH           (0x0001U) /* MTIOC3B active level high */
/* MTIOC3D Active Level Setting (OLSG0B) */
#define _0000_POE_MTIOC3D_LOW            (0x0000U) /* MTIOC3D active level low */
#define _0002_POE_MTIOC3D_HIGH           (0x0002U) /* MTIOC3D active level high */
/* MTIOC4A Active Level Setting (OLSG1A) */
#define _0000_POE_MTIOC4A_LOW            (0x0000U) /* MTIOC4A active level low */
#define _0004_POE_MTIOC4A_HIGH           (0x0004U) /* MTIOC4A active level high */
/* MTIOC4C Active Level Setting (OLSG1B) */
#define _0000_POE_MTIOC4C_LOW            (0x0000U) /* MTIOC4C active level low */
#define _0008_POE_MTIOC4C_HIGH           (0x0008U) /* MTIOC4C active level high */
/* MTIOC4B Active Level Setting (OLSG2A) */
#define _0000_POE_MTIOC4B_LOW            (0x0000U) /* MTIOC4B active level low */
#define _0010_POE_MTIOC4B_HIGH           (0x0010U) /* MTIOC4B active level high */
/* MTIOC4D Active Level Setting (OLSG2B) */
#define _0000_POE_MTIOC4D_LOW            (0x0000U) /* MTIOC4D active level low */
#define _0020_POE_MTIOC4D_HIGH           (0x0020U) /* MTIOC4D active level high */
/* Active Level Setting Enable (OLSEN) */
#define _0000_POE_ACTIVELEL_DISABLE      (0x0000U) /* Active level disable */
#define _0080_POE_ACTIVELEL_ENABLE       (0x0080U) /* Active level enable */

/*
    Software Port Output Enable Register (SPOER)
*/
/* MTU3 and MTU4 Output High-Impedance Enable (MTUCH34HIZ) */
#define _00_POE_MTU34HIZ_DISABLE         (0x00U)   /* Does not place the pins in high-impedance */
#define _01_POE_MTU34HIZ_ENABLE          (0x01U)   /* Places the pins in high-impedance */
/* MTU6 and MTU7 Output High-Impedance Enable (MTUCH67HIZ) */
#define _00_POE_MTU67HIZ_DISABLE         (0x00U)   /* Does not place the pins in high-impedance */
#define _02_POE_MTU67HIZ_ENABLE          (0x02U)   /* Places the pins in high-impedance */
/* MTU0 Output High-Impedance Enable (MTUCH0HIZ) */
#define _00_POE_MTU0HIZ_DISABLE          (0x00U)   /* Does not place the pins in high-impedance */
#define _04_POE_MTU0HIZ_ENABLE           (0x04U)   /* Places the pins in high-impedance */

/*
    Port Output Enable Control Register 1 (POECR1)
*/
/* MTIOC0A High-Impedance Enable (MTU0AZE) */
#define _00_POE_MTU0AZE_DISABLE          (0x00U)   /* Does not place the pin in high-impedance */
#define _01_POE_MTU0AZE_ENABLE           (0x01U)   /* Places the pin in high-impedance */
/* MTIOC0B High-Impedance Enable (MTU0BZE) */
#define _00_POE_MTU0BZE_DISABLE          (0x00U)   /* Does not place the pin in high-impedance */
#define _02_POE_MTU0BZE_ENABLE           (0x02U)   /* Places the pin in high-impedance */
/* MTIOC0C High-Impedance Enable (MTU0CZE) */
#define _00_POE_MTU0CZE_DISABLE          (0x00U)   /* Does not place the pin in high-impedance */
#define _04_POE_MTU0CZE_ENABLE           (0x04U)   /* Places the pin in high-impedance */
/* MTIOC0D High-Impedance Enable (MTU0DZE) */
#define _00_POE_MTU0DZE_DISABLE          (0x00U)   /* Does not place the pin in high-impedance */
#define _08_POE_MTU0DZE_ENABLE           (0x08U)   /* Places the pin in high-impedance */

/*
    Port Output Enable Control Register 2 (POECR2)
*/
/* MTIOC7B/7D High-Impedance Enable (MTU7BDZE) */
#define _0000_POE_MTU7BDZE_DISABLE       (0x0000U) /* Does not place the pin in high-impedance */
#define _0001_POE_MTU7BDZE_ENABLE        (0x0001U) /* Places the pin in high-impedance */
/* MTIOC7A/7C High-Impedance Enable (MTU7ACZE) */
#define _0000_POE_MTU7ACZE_DISABLE       (0x0000U) /* Does not place the pin in high-impedance */
#define _0002_POE_MTU7ACZE_ENABLE        (0x0002U) /* Places the pin in high-impedance */
/* MTIOC6B/6D High-Impedance Enable (MTU6BDZE) */
#define _0000_POE_MTU6BDZE_DISABLE       (0x0000U) /* Does not place the pin in high-impedance */
#define _0004_POE_MTU6BDZE_ENABLE        (0x0004U) /* Places the pin in high-impedance */
/* MTIOC4B/4D High-Impedance Enable (MTU4BDZE) */
#define _0000_POE_MTU4BDZE_DISABLE       (0x0000U) /* Does not place the pin in high-impedance */
#define _0100_POE_MTU4BDZE_ENABLE        (0x0100U) /* Places the pin in high-impedance */
/* MTIOC4A/4C High-Impedance Enable (MTU4ACZE) */
#define _0000_POE_MTU4ACZE_DISABLE       (0x0000U) /* Does not place the pin in high-impedance */
#define _0200_POE_MTU4ACZE_ENABLE        (0x0200U) /* Places the pin in high-impedance */
/* MTIOC3B/3D High-Impedance Enable (MTU3BDZE) */
#define _0000_POE_MTU3BDZE_DISABLE       (0x0000U) /* Does not place the pin in high-impedance */
#define _0400_POE_MTU3BDZE_ENABLE        (0x0400U) /* Places the pin in high-impedance */

/*
    Port Output Enable Control Register 4 (POECR4)
*/
/* MTU3 and MTU4 High-Impedance POE4F Add (IC2ADDMT34ZE) */
#define _0000_POE_IC2ADDMT34ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0004_POE_IC2ADDMT34ZE_ENABLE    (0x0004U) /* Add the pins to high-impedance control conditions */
/* MTU3 and MTU4 High-Impedance POE8F Add (IC3ADDMT34ZE) */
#define _0000_POE_IC3ADDMT34ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0008_POE_IC3ADDMT34ZE_ENABLE    (0x0008U) /* Add the pins to high-impedance control conditions */
/* MTU3 and MTU4 High-Impedance POE10F Add (IC4ADDMT34ZE) */
#define _0000_POE_IC4ADDMT34ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0010_POE_IC4ADDMT34ZE_ENABLE    (0x0010U) /* Add the pins to high-impedance control conditions */
/* MTU3 and MTU4 High-Impedance POE11F Add (IC5ADDMT34ZE) */
#define _0000_POE_IC5ADDMT34ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0020_POE_IC5ADDMT34ZE_ENABLE    (0x0020U) /* Add the pins to high-impedance control conditions */
/* MTU6 and MTU7 High-Impedance POE0F Add (IC1ADDMT67ZE) */
#define _0000_POE_IC1ADDMT67ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0200_POE_IC1ADDMT67ZE_ENABLE    (0x0200U) /* Add the pins to high-impedance control conditions */
/* MTU6 and MTU7 High-Impedance POE8F Add (IC3ADDMT67ZE) */
#define _0000_POE_IC3ADDMT67ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0800_POE_IC3ADDMT67ZE_ENABLE    (0x0800U) /* Add the pins to high-impedance control conditions */
/* MTU6 and MTU7 High-Impedance POE10F Add (IC4ADDMT67ZE) */
#define _0000_POE_IC4ADDMT67ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _1000_POE_IC4ADDMT67ZE_ENABLE    (0x1000U) /* Add the pins to high-impedance control conditions */
/* MTU6 and MTU7 High-Impedance POE11F Add (IC5ADDMT67ZE) */
#define _0000_POE_IC5ADDMT67ZE_DISABLE   (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _2000_POE_IC5ADDMT67ZE_ENABLE    (0x2000U) /* Add the pins to high-impedance control conditions */
/* POECR4 default value */
#define _0402_POE_POECR4_DEFAULT         (0x0402U) /* POECR4 default value */

/*
    Port Output Enable Control Register 5 (POECR5)
*/
/* MTU0 High-Impedance POE0F Add (IC1ADDMT0ZE) */
#define _0000_POE_IC1ADDMT0ZE_DISABLE    (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0002_POE_IC1ADDMT0ZE_ENABLE     (0x0002U) /* Add the pins to high-impedance control conditions */
/* MTU0 High-Impedance POE4F Add (IC2ADDMT0ZE) */
#define _0000_POE_IC2ADDMT0ZE_DISABLE    (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0004_POE_IC2ADDMT0ZE_ENABLE     (0x0004U) /* Add the pins to high-impedance control conditions */
/* MTU0 High-Impedance POE10F Add (IC4ADDMT0ZE) */
#define _0000_POE_IC4ADDMT0ZE_DISABLE    (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0010_POE_IC4ADDMT0ZE_ENABLE     (0x0010U) /* Add the pins to high-impedance control conditions */
/* MTU0 High-Impedance POE11F Add (IC5ADDMT0ZE) */
#define _0000_POE_IC5ADDMT0ZE_DISABLE    (0x0000U) /* Does not add the pins to high-impedance control conditions */
#define _0020_POE_IC5ADDMT0ZE_ENABLE     (0x0020U) /* Add the pins to high-impedance control conditions */
/* POECR5 default value */
#define _0008_POE_POECR5_DEFAULT         (0x0008U) /* POECR5 default value */

/*
    MTU0 Pin Select Register 1 (M0SELR1)
*/
/* MTU0-A (MTIOC0A) Pin Select (M0ASEL) */
#define _00_POE_MTU0APIN_P34             (0x00U)   /* Controls high-impedance state of P34 when used as MTIOC0A pin */
#define _02_POE_MTU0APIN_PB3             (0x02U)   /* Controls high-impedance state of PB3 when used as MTIOC0A pin */
/* MTU0-B (MTIOC0B) Pin Select (M0BSEL) */
#define _00_POE_MTU0BPIN_P13             (0x00U)   /* Controls high-impedance state of P13 when used as MTIOC0B pin */
#define _10_POE_MTU0BPIN_P15             (0x10U)   /* Controls high-impedance state of P15 when used as MTIOC0B pin */
#define _20_POE_MTU0BPIN_PA1             (0x20U)   /* Controls high-impedance state of PA1 when used as MTIOC0B pin */

/*
    MTU0 Pin Select Register 2 (M0SELR2)
*/
/* MTU0-C (MTIOC0C) Pin Select (M0CSEL) */
#define _00_POE_MTU0CPIN_P32             (0x00U)   /* Controls high-impedance state of P32 when used as MTIOC0C pin */
#define _02_POE_MTU0CPIN_PB1             (0x02U)   /* Controls high-impedance state of PB1 when used as MTIOC0C pin */
/* MTU0-D (MTIOC0D) Pin Select (M0DSEL) */
#define _00_POE_MTU0DPIN_P33             (0x00U)   /* Controls high-impedance state of P33 when used as MTIOC0D pin */
#define _20_POE_MTU0DPIN_PA3             (0x20U)   /* Controls high-impedance state of PA3 when used as MTIOC0D pin */

/*
    MTU3 Pin Select Register (M3SELR)
*/
/* MTU3-B (MTIOC3B) Pin Select (M3BSEL) */
#define _00_POE_MTU3BPIN_PE1             (0x00U)   /* Controls high-impedance state of PE1 when used as MTIOC3B pin */
#define _01_POE_MTU3BPIN_P22             (0x01U)   /* Controls high-impedance state of P22 when used as MTIOC3B pin */
#define _02_POE_MTU3BPIN_P80             (0x02U)   /* Controls high-impedance state of P80 when used as MTIOC3B pin */
#define _03_POE_MTU3BPIN_PC5             (0x03U)   /* Controls high-impedance state of PC5 when used as MTIOC3B pin */
#define _04_POE_MTU3BPIN_PB7             (0x04U)   /* Controls high-impedance state of PB7 when used as MTIOC3B pin */
#define _05_POE_MTU3BPIN_P17             (0x05U)   /* Controls high-impedance state of P17 when used as MTIOC3B pin */
/* MTU3-D (MTIOC3D) Pin Select (M3DSEL) */
#define _00_POE_MTU3DPIN_PE0             (0x00U)   /* Controls high-impedance state of PE0 when used as MTIOC3D pin */
#define _10_POE_MTU3DPIN_P23             (0x10U)   /* Controls high-impedance state of P23 when used as MTIOC3D pin */
#define _20_POE_MTU3DPIN_PC4             (0x20U)   /* Controls high-impedance state of PC4 when used as MTIOC3D pin */
#define _30_POE_MTU3DPIN_P81             (0x30U)   /* Controls high-impedance state of P81 when used as MTIOC3D pin */
#define _40_POE_MTU3DPIN_PB6             (0x40U)   /* Controls high-impedance state of PB6 when used as MTIOC3D pin */
#define _50_POE_MTU3DPIN_P16             (0x50U)   /* Controls high-impedance state of P16 when used as MTIOC3D pin */

/*
    MTU4 Pin Select Register 1 (M4SELR1)
*/
/* MTU4-A (MTIOC4A) Pin Select (M4ASEL) */
#define _00_POE_MTU4APIN_PE2             (0x00U)   /* Controls high-impedance state of PE2 when used as MTIOC4A pin */
#define _01_POE_MTU4APIN_P21             (0x01U)   /* Controls high-impedance state of P21 when used as MTIOC4A pin */
#define _02_POE_MTU4APIN_PB3             (0x02U)   /* Controls high-impedance state of PB3 when used as MTIOC4A pin */
#define _03_POE_MTU4APIN_P82             (0x03U)   /* Controls high-impedance state of P82 when used as MTIOC4A pin */
#define _04_POE_MTU4APIN_PA0             (0x04U)   /* Controls high-impedance state of PA0 when used as MTIOC4A pin */
#define _05_POE_MTU4APIN_P24             (0x05U)   /* Controls high-impedance state of P24 when used as MTIOC4A pin */
/* MTU4-C (MTIOC4C) Pin Select (M4CSEL) */
#define _00_POE_MTU4CPIN_PE5             (0x00U)   /* Controls high-impedance state of PE5 when used as MTIOC4C pin */
#define _10_POE_MTU4CPIN_P87             (0x10U)   /* Controls high-impedance state of P87 when used as MTIOC4C pin */
#define _20_POE_MTU4CPIN_PB1             (0x20U)   /* Controls high-impedance state of PB1 when used as MTIOC4C pin */
#define _30_POE_MTU4CPIN_P83             (0x30U)   /* Controls high-impedance state of P83 when used as MTIOC4C pin */
#define _40_POE_MTU4CPIN_PE1             (0x40U)   /* Controls high-impedance state of PE1 when used as MTIOC4C pin */
#define _50_POE_MTU4CPIN_P25             (0x50U)   /* Controls high-impedance state of P25 when used as MTIOC4C pin */

/*
    MTU4 Pin Select Register 2 (M4SELR2)
*/
/* MTU4-B (MTIOC4B) Pin Select (M4BSEL) */
#define _00_POE_MTU4BPIN_PE3             (0x00U)   /* Controls high-impedance state of PE3 when used as MTIOC4B pin */
#define _01_POE_MTU4BPIN_P17             (0x01U)   /* Controls high-impedance state of P17 when used as MTIOC4B pin */
#define _02_POE_MTU4BPIN_P54             (0x02U)   /* Controls high-impedance state of P54 when used as MTIOC4B pin */
#define _03_POE_MTU4BPIN_PC2             (0x03U)   /* Controls high-impedance state of PC2 when used as MTIOC4B pin */
#define _04_POE_MTU4BPIN_PD1             (0x04U)   /* Controls high-impedance state of PD1 when used as MTIOC4B pin */
#define _05_POE_MTU4BPIN_P30             (0x05U)   /* Controls high-impedance state of P30 when used as MTIOC4B pin */
/* MTU4-D (MTIOC4D) Pin Select (M4DSEL) */
#define _00_POE_MTU4DPIN_PE4             (0x00U)   /* Controls high-impedance state of PE4 when used as MTIOC4D pin */
#define _10_POE_MTU4DPIN_P86             (0x10U)   /* Controls high-impedance state of P86 when used as MTIOC4D pin */
#define _20_POE_MTU4DPIN_P55             (0x20U)   /* Controls high-impedance state of P55 when used as MTIOC4D pin */
#define _30_POE_MTU4DPIN_PC3             (0x30U)   /* Controls high-impedance state of PC3 when used as MTIOC4D pin */
#define _40_POE_MTU4DPIN_PD2             (0x40U)   /* Controls high-impedance state of PD2 when used as MTIOC4D pin */
#define _50_POE_MTU4DPIN_P31             (0x50U)   /* Controls high-impedance state of P31 when used as MTIOC4D pin */

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

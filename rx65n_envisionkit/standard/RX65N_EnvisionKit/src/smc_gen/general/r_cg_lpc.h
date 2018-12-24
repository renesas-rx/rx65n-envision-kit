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
* File Name    : r_cg_lpc.h
* Version      : 1.1.0
* Device(s)    : R5F565NEHxFB
* Description  : General header file for LPC peripheral.
* Creation Date: 2017-11-02
***********************************************************************************************************************/

#ifndef LPC_H
#define LPC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/* 
    Standby Control Register (SBYCR) 
*/
/* Output Port Enable (OPE) */
#define _0000_LPC_OPE_HIGH_IMPEDANCE (0x0000U) /* Address bus and bus control signals set to high-impedance state */
#define _4000_LPC_OPE_SIGNAL_RETAIN  (0x4000U) /* Address bus and bus control signals retain the output state */
/* Software Standby (SSBY) */
#define _0000_LPC_SHIFT_SLEEP_MODE   (0x0000U) /* Shifts to sleep or all-module clock stop mode after WAIT */
#define _8000_LPC_SHIFT_SSBY_MODE    (0x8000U) /* Shifts to software standby mode after WAIT */

/* 
    Operating Power Control Register (OPCCR)
*/
/* Operating Power Control Mode Select (OPCM[2:0]) */
#define _00_LPC_HIGH_SPEED_MODE      (0x00U) /* High-speed operating mode */
#define _06_LPC_LOW_SPEED_MODE1      (0x06U) /* Low-speed operating mode 1 */
#define _07_LPC_LOW_SPEED_MODE2      (0x07U) /* Low-speed operating mode 2 */

/* 
    Deep Standby Control Register (DPSBYCR)
*/
/* Deep Cut (DEEPCUT[1:0]) */
#define _00_LPC_POWER_ON             (0x00U) /* Power is supplied to standby RAM, USB resume detecting unit */
#define _01_LPC_POWER_OFF            (0x01U) /* Power is not supplied to standby RAM, USB resume detecting unit */
#define _03_LPC_POWER_OFF_LVD        (0x03U) /* Power is not supplied to standby RAM, USB resume detecting unit in deep 
                                                software standby mode. LVD is stopped and the LPC function is enabled */
/* I/O Port Retention (IOKEEP) */
#define _00_LPC_IO_STATE_CANCELLED   (0x00U) /* Deep software standby and I/O port retention cancelled simultaneously */
#define _40_LPC_IO_STATE_RETAINED    (0x40U) /* I/O port state retained after deep software standby mode cancelled */
/* Deep Software Standby (DPSBY) */
#define _00_LPC_DPSBY_DISABLED       (0x00U) /* Transition to software standby mode made after WAIT executed */
#define _80_LPC_DPSBY_ENABLED        (0x80U) /* Transition to deep software standby mode made after WAIT executed */

/*
    Sleep Mode Return Clock Source Switching Register (RSTCKCR)
*/
/* Sleep Mode Return Clock Source Switching Enable (RSTCKEN) */
#define _00_LPC_RETURN_DISABLED      (0x00U) /* Clock source switching at sleep mode cancellation is disabled */
#define _80_LPC_RETURN_ENABLED       (0x80U) /* Clock source switching at sleep mode cancellation is enabled */
/* Sleep Mode Return Clock Source Select (RSTCKSEL[2:0]) */
#define _81_LPC_RETURN_CLK_HOCO      (0x81U) /* HOCO is selected */
#define _82_LPC_RETURN_CLK_MAIN_OSC  (0x82U) /* Main clock oscillator is selected */

/*
    Deep Standby Interrupt Enable Register 0 (DPSIER0)
*/
#define _00_LPC_DIRQ0E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ0-DS pin is disabled */
#define _01_LPC_DIRQ0E_ENABLED       (0x01U) /* Cancelling deep software standby mode by the IRQ0-DS pin is enabled */
#define _00_LPC_DIRQ1E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ1-DS pin is disabled */
#define _02_LPC_DIRQ1E_ENABLED       (0x02U) /* Cancelling deep software standby mode by the IRQ1-DS pin is enabled */
#define _00_LPC_DIRQ2E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ2-DS pin is disabled */
#define _04_LPC_DIRQ2E_ENABLED       (0x04U) /* Cancelling deep software standby mode by the IRQ2-DS pin is enabled */
#define _00_LPC_DIRQ3E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ3-DS pin is disabled */
#define _08_LPC_DIRQ3E_ENABLED       (0x08U) /* Cancelling deep software standby mode by the IRQ3-DS pin is enabled */
#define _00_LPC_DIRQ4E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ4-DS pin is disabled */
#define _10_LPC_DIRQ4E_ENABLED       (0x10U) /* Cancelling deep software standby mode by the IRQ4-DS pin is enabled */
#define _00_LPC_DIRQ5E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ5-DS pin is disabled */
#define _20_LPC_DIRQ5E_ENABLED       (0x20U) /* Cancelling deep software standby mode by the IRQ5-DS pin is enabled */
#define _00_LPC_DIRQ6E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ6-DS pin is disabled */
#define _40_LPC_DIRQ6E_ENABLED       (0x40U) /* Cancelling deep software standby mode by the IRQ6-DS pin is enabled */
#define _00_LPC_DIRQ7E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ7-DS pin is disabled */
#define _80_LPC_DIRQ7E_ENABLED       (0x80U) /* Cancelling deep software standby mode by the IRQ7-DS pin is enabled */

/*
    Deep Standby Interrupt Enable Register 1 (DPSIER1)
*/
#define _00_LPC_DIRQ8E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ8-DS pin is disabled */
#define _01_LPC_DIRQ8E_ENABLED       (0x01U) /* Cancelling deep software standby mode by the IRQ8-DS pin is enabled */
#define _00_LPC_DIRQ9E_DISABLED      (0x00U) /* Cancelling deep software standby mode by the IRQ9-DS pin is disabled */
#define _02_LPC_DIRQ9E_ENABLED       (0x02U) /* Cancelling deep software standby mode by the IRQ9-DS pin is enabled */
#define _00_LPC_DIRQ10E_DISABLED     (0x00U) /* Cancelling deep software standby mode by the IRQ10-DS pin is disabled */
#define _04_LPC_DIRQ10E_ENABLED      (0x04U) /* Cancelling deep software standby mode by the IRQ10-DS pin is enabled */
#define _00_LPC_DIRQ11E_DISABLED     (0x00U) /* Cancelling deep software standby mode by the IRQ11-DS pin is disabled */
#define _08_LPC_DIRQ11E_ENABLED      (0x08U) /* Cancelling deep software standby mode by the IRQ11-DS pin is enabled */
#define _00_LPC_DIRQ12E_DISABLED     (0x00U) /* Cancelling deep software standby mode by the IRQ12-DS pin is disabled */
#define _10_LPC_DIRQ12E_ENABLED      (0x10U) /* Cancelling deep software standby mode by the IRQ12-DS pin is enabled */
#define _00_LPC_DIRQ13E_DISABLED     (0x00U) /* Cancelling deep software standby mode by the IRQ13-DS pin is disabled */
#define _20_LPC_DIRQ13E_ENABLED      (0x20U) /* Cancelling deep software standby mode by the IRQ13-DS pin is enabled */
#define _00_LPC_DIRQ14E_DISABLED     (0x00U) /* Cancelling deep software standby mode by the IRQ14-DS pin is disabled */
#define _40_LPC_DIRQ14E_ENABLED      (0x40U) /* Cancelling deep software standby mode by the IRQ14-DS pin is enabled */
#define _00_LPC_DIRQ15E_DISABLED     (0x00U) /* Cancelling deep software standby mode by the IRQ15-DS pin is disabled */
#define _80_LPC_DIRQ15E_ENABLED      (0x80U) /* Cancelling deep software standby mode by the IRQ15-DS pin is enabled */

/*
    Deep Standby Interrupt Enable Register 2 (DPSIER2)
*/
#define _00_LPC_DLVD1IE_DISABLED     (0x00U) /* Disable cancelling deep software standby mode by voltage monitoring 1 */
#define _01_LPC_DLVD1IE_ENABLED      (0x01U) /* Enable cancelling deep software standby mode by voltage monitoring 1 */
#define _00_LPC_DLVD2IE_DISABLED     (0x00U) /* Disable cancelling deep software standby mode by voltage monitoring 2 */
#define _02_LPC_DLVD2IE_ENABLED      (0x02U) /* Enable cancelling deep software standby mode by voltage monitoring 2 */
#define _00_LPC_DRTCIIE_DISABLED     (0x00U) /* Disable cancelling deep software standby by RTC interval interrupt */
#define _04_LPC_DRTCIIE_ENABLED      (0x04U) /* Enable cancelling deep software standby by RTC interval interrupt */
#define _00_LPC_DRTCAIE_DISABLED     (0x00U) /* Disable cancelling deep software standby mode by RTC alarm interrupt */
#define _08_LPC_DRTCAIE_ENABLED      (0x08U) /* Enable cancelling deep software standby mode by RTC alarm interrupt */
#define _00_LPC_DNMIE_DISABLED       (0x00U) /* Cancelling deep software standby mode by NMI pin is disabled */
#define _10_LPC_DNMIE_ENABLED        (0x10U) /* Cancelling deep software standby mode by NMI pin is enabled */
#define _00_LPC_DRIICDIE_DISABLED    (0x00U) /* Cancelling deep software standby mode by SDA2-DS signal is disabled */
#define _20_LPC_DRIICDIE_ENABLED     (0x20U) /* Cancelling deep software standby mode by SDA2-DS signal is enabled */
#define _00_LPC_DRIICCIE_DISABLED    (0x00U) /* Cancelling deep software standby mode by SCL2-DS signal is disabled */
#define _40_LPC_DRIICCIE_ENABLED     (0x40U) /* Cancelling deep software standby mode by SCL2-DS signal is enabled */
#define _00_LPC_DUSBIE_DISABLED      (0x00U) /* Disable cancelling deep software standby mode by USB suspend/resume */
#define _80_LPC_DUSBIE_ENABLED       (0x80U) /* Enable cancelling deep software standby mode by USB suspend/resume */

/*
    Deep Standby Interrupt Enable Register 3 (DPSIER3)
*/
/* CRX1-DS Deep Standby Cancel Signal Enable (DCANIE) */
#define _00_LPC_DCANIE_DISABLED      (0x00U) /* Cancelling deep software standby mode by the CRX1-DS pin is disabled */
#define _01_LPC_DCANIE_ENABLED       (0x01U) /* Cancelling deep software standby mode by the CRX1-DS pin is enabled */

/*
    Deep Standby Interrupt Edge Register 0 (DPSIEGR0)
*/
#define _00_LPC_DIRQ0EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _01_LPC_DIRQ0EG_RISING       (0x01U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ1EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _02_LPC_DIRQ1EG_RISING       (0x02U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ2EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _04_LPC_DIRQ2EG_RISING       (0x04U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ3EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _08_LPC_DIRQ3EG_RISING       (0x08U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ4EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _10_LPC_DIRQ4EG_RISING       (0x10U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ5EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _20_LPC_DIRQ5EG_RISING       (0x20U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ6EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _40_LPC_DIRQ6EG_RISING       (0x40U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ7EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _80_LPC_DIRQ7EG_RISING       (0x80U) /* A cancel request is generated at a rising edge */

/*
    Deep Standby Interrupt Edge Register 1 (DPSIEGR1)
*/
#define _00_LPC_DIRQ8EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _01_LPC_DIRQ8EG_RISING       (0x01U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ9EG_FALLING      (0x00U) /* A cancel request is generated at a falling edge */
#define _02_LPC_DIRQ9EG_RISING       (0x02U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ10EG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _04_LPC_DIRQ10EG_RISING      (0x04U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ11EG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _08_LPC_DIRQ11EG_RISING      (0x08U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ12EG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _10_LPC_DIRQ12EG_RISING      (0x10U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ13EG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _20_LPC_DIRQ13EG_RISING      (0x20U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ14EG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _40_LPC_DIRQ14EG_RISING      (0x40U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DIRQ15EG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _80_LPC_DIRQ15EG_RISING      (0x80U) /* A cancel request is generated at a rising edge */

/*
    Deep Standby Interrupt Edge Register 2 (DPSIEGR2)
*/
#define _00_LPC_DLVD1EG_FALLING      (0x00U) /* A cancel request is generated when VCC < Vdet1 (fall) is detected */
#define _01_LPC_DLVD1EG_RISING       (0x01U) /* A cancel request is generated when VCC >= Vdet1 (rise) is detected */
#define _00_LPC_DLVD2EG_FALLING      (0x00U) /* A cancel request is generated when VCC < Vdet2 (fall) is detected */
#define _02_LPC_DLVD2EG_RISING       (0x02U) /* A cancel request is generated when VCC >= Vdet2 (rise) is detected */
#define _00_LPC_DNMIEG_FALLING       (0x00U) /* A cancel request is generated at a falling edge */
#define _10_LPC_DNMIEG_RISING        (0x10U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DRIICDEG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _20_LPC_DRIICDEG_RISING      (0x20U) /* A cancel request is generated at a rising edge */
#define _00_LPC_DRIICCEG_FALLING     (0x00U) /* A cancel request is generated at a falling edge */
#define _40_LPC_DRIICCEG_RISING      (0x40U) /* A cancel request is generated at a rising edge */

/*
    System Clock Control Register 3 (SCKCR3)
*/
#define _0100_LPC_CLOCKSOURCE_HOCO    (0x0100U) /* HOCO */
#define _0200_LPC_CLOCKSOURCE_MAINCLK (0x0200U) /* Main clock oscillator */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    RESERVED1,
    RESERVED2,
    HIGH_SPEED,
    LOW_SPEED2,
    LOW_SPEED1,
} operating_mode_t;

typedef enum
{
    RETURN_DISABLE,
    RESERVED3,
    RETURN_HOCO,
    RETURN_MAIN_CLOCK
} return_clock_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

